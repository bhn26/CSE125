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
uniform vec3 cameraEye;
uniform DirectionalLight dLight;

in VS_OUT
{
    vec3 _fragPos;
    vec3 _normal;
    vec2 _texCoords;
} fs_in;

////////////////////////////////////////////////////////////////////////////////
vec4 CalcLight(BaseLight light, vec3 lightDirection)
{
    float specularIntensity = 1.0f;
    float specularPower = 1.0f;

    vec4 ambientColor = vec4(light._color * light._ambientIntensity, 1.0);
    float diffuseFactor = dot(fs_in._normal, -lightDirection);
    //vec3 normal = vec3(0.0f, 1.0f, 0.0f);
    //float diffuseFactor = dot(normal, -lightDirection);
    //diffuseFactor = 1.0f;

    vec4 diffuseColor  = vec4(0, 0, 0, 0);
    vec4 specularColor = vec4(0, 0, 0, 0);

    if (diffuseFactor > 0.0)
    {
        diffuseColor = vec4(light._color * light._diffuseIntensity * diffuseFactor, 1.0);
        //diffuseColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);

        vec3 vertexToEye = normalize(cameraEye - fs_in._fragPos);
        vec3 lightReflect = normalize(reflect(lightDirection, fs_in._normal));
        float specularFactor = dot(vertexToEye, lightReflect);
        if (specularFactor > 0.0)
        {
            specularFactor = pow(specularFactor, specularPower);
            specularColor = vec4(light._color * specularIntensity * specularFactor, 1.0);
        }
    }
    else
    {
        //diffuseColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }

    return (ambientColor + diffuseColor + specularColor);
}

////////////////////////////////////////////////////////////////////////////////
void main()
{
    vec4 totalLight = CalcLight(dLight._base, dLight._direction);
    color = vec4(texture(texture_diffuse1, fs_in._texCoords)) * totalLight;
}