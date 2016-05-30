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

in vec4 Eye;

in VS_OUT
{
    vec3 _fragPos;
    vec3 _normal;
    vec2 _texCoords;
} fs_in;

uniform vec3 objectColor;

// Object material
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;
uniform vec3 cameraEye;

uniform DirectionalLight dLight;

out vec4 color;

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

    return (ambientColor + diffuseColor + specularColor);
}

////////////////////////////////////////////////////////////////////////////////
void main()
{
    vec4 result = CalcLight(dLight._base, dLight._direction) * vec4(objectColor, 1.0f);
    color = result;
}