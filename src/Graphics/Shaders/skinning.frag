#version 330

const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

////////////////////////////////////////////////////////////////////////////////
struct Material
{
    vec3 _diffuse;
    vec3 _specular;
    vec3 _ambient;
    float _shininess;
};

struct VSOutput
{
    vec2 TexCoord;
    vec3 Normal;
    vec3 WorldPos;
};


struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct DirectionalLight
{
    BaseLight Base;
    vec3 Direction;
};

struct Attenuation
{
    float Constant;
    float Linear;
    float Exp;
};

struct PointLight2
{
    BaseLight Base;
    vec3 Position;
    Attenuation Atten;
};

struct SpotLight
{
    PointLight2 Base;
    vec3 Direction;
    float Cutoff;
};

////////////////////////////////////////////////////////////////////////////////
uniform int gNumPointLights;
uniform int gNumSpotLights;

uniform DirectionalLight gDirectionalLight;
uniform PointLight2 gPointLights[MAX_POINT_LIGHTS];
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];

uniform sampler2D gColorMap;
uniform sampler2D shadowMap;

uniform vec3 gEyeWorldPos;
uniform float gMatSpecularIntensity;
uniform float gSpecularPower;

uniform Material material;

uniform bool useTexture;
uniform bool renderingDepth;

in VS_OUT
{
    vec3 _fragPos;
    vec3 _normal;
    vec2 _texCoords;
    vec4 _fragPosLightSpace;
} fs_in;

////////////////////////////////////////////////////////////////////////////////
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 

    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // Check whether current frag pos is in shadow
    float bias = 0.003;
    float shadow = currentDepth - bias > closestDepth  ? 0.9 : 0.0;

    return shadow;
}

////////////////////////////////////////////////////////////////////////////////
vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, VSOutput In)
{
    vec4 AmbientColor = vec4(Light.Color * Light.AmbientIntensity, 1.0);
    float DiffuseFactor = dot(In.Normal, -LightDirection);

    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0.0) {
        DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0);

        vec3 VertexToEye = normalize(gEyeWorldPos - In.WorldPos);
        vec3 LightReflect = normalize(reflect(LightDirection, In.Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
        if (SpecularFactor > 0.0) {
            SpecularFactor = pow(SpecularFactor, gSpecularPower);
            SpecularColor = vec4(Light.Color * gMatSpecularIntensity * SpecularFactor, 1.0);
        }
    }
    
    vec4 red = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    float shadow = ShadowCalculation(fs_in._fragPosLightSpace);
    //if (shadow > 0.0f)
    //    return (AmbientColor + DiffuseColor + SpecularColor) * (shadow) * red;
    //return (AmbientColor + DiffuseColor + SpecularColor);
    return (AmbientColor + (1.0 - shadow) * (DiffuseColor + SpecularColor));
}

////////////////////////////////////////////////////////////////////////////////
vec4 CalcDirectionalLight(VSOutput In)
{
    return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, In);  
}

////////////////////////////////////////////////////////////////////////////////
vec4 CalcPointLight(PointLight2 l, VSOutput In)
{
    vec3 LightDirection = In.WorldPos - l.Position;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);

    vec4 Color = CalcLightInternal(l.Base, LightDirection, In);
    float Attenuation =  l.Atten.Constant +
                         l.Atten.Linear * Distance +
                         l.Atten.Exp * Distance * Distance;

    //return Color / Attenuation;
    return Color;
}

////////////////////////////////////////////////////////////////////////////////
vec4 CalcSpotLight(SpotLight l, VSOutput In)
{
    vec3 LightToPixel = normalize(In.WorldPos - l.Base.Position);
    float SpotFactor = dot(LightToPixel, l.Direction);

    if (SpotFactor > l.Cutoff) {
        vec4 Color = CalcPointLight(l.Base, In);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));
    }
    else {
        return vec4(0,0,0,0);
    }
}

out vec4 FragColor;

////////////////////////////////////////////////////////////////////////////////
void main()
{
    if (renderingDepth)
    {
        // Don't do anything
    }
    else
    {
        VSOutput In;
        In.TexCoord = TexCoord0;
        In.Normal   = normalize(Normal0);
        In.WorldPos = WorldPos0;
    
        vec4 TotalLight = CalcDirectionalLight(In);

        for (int i = 0 ; i < gNumPointLights ; i++) {
            TotalLight += CalcPointLight(gPointLights[i], In);
        }
    
        for (int i = 0 ; i < gNumSpotLights ; i++) {
            TotalLight += CalcSpotLight(gSpotLights[i], In);
        }
    
        if (useTexture)
            FragColor = texture(gColorMap, In.TexCoord.xy) * TotalLight;
        else
            FragColor = vec4(material._diffuse, 1.0f) * TotalLight;
    }
}
