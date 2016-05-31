#version 330 core

struct BaseLight
{
    vec3 _color;
    float _ambientIntensity;
    float _diffuseIntensity;
};

struct DirectionalLight
{
    BaseLight _base;
    vec3 _direction;
};

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D shadowMap;

uniform vec3 cameraEye;
uniform DirectionalLight dLight;

in VS_OUT
{
    vec3 _fragPos;
    vec3 _normal;
    vec2 _texCoords;
    vec3 _fragPosLightSpace;
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
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

////////////////////////////////////////////////////////////////////////////////
vec4 CalcLight(BaseLight light, vec3 lightDirection)
{
    float specularIntensity = 1.0f;
    float specularPower = 1.0f;

    vec4 ambientColor = vec4(light._color * light._ambientIntensity, 1.0);
    float diffuseFactor = dot(fs_in._normal, -lightDirection);

    vec4 diffuseColor  = vec4(0, 0, 0, 0);
    vec4 specularColor = vec4(0, 0, 0, 0);

    if (diffuseFactor > 0.0)
    {
        diffuseColor = vec4(light._color * light._diffuseIntensity * diffuseFactor, 1.0);

        vec3 vertexToEye = normalize(cameraEye - fs_in._fragPos);
        vec3 lightReflect = normalize(reflect(lightDirection, fs_in._normal));
        float specularFactor = dot(vertexToEye, lightReflect);
        if (specularFactor > 0.0)
        {
            specularFactor = pow(specularFactor, specularPower);
            specularColor = vec4(light._color * specularIntensity * specularFactor, 1.0);
        }
    }
    float shadow = ShadowCalculation(fs_in._fragPosLightSpace);
    return (ambientColor + (1.0f - shadow)*(diffuseColor + specularColor));
}

////////////////////////////////////////////////////////////////////////////////
void main()
{
    vec4 totalLight = CalcLight(dLight._base, dLight._direction);
    color = vec4(texture(texture_diffuse1, fs_in._texCoords)) * totalLight;
}