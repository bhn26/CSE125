#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;
//layout (location = 3) in uint[8] BoneIDs;
//layout (location = 4) in float[8] Weights;


out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;

const int MAX_BONES = 100;

uniform mat4 gWVP;
uniform mat4 perspective;
uniform mat4 view;
uniform mat4 gWorld;
uniform mat4 gBones[MAX_BONES];

void main()
{       
    mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
    BoneTransform     += gBones[BoneIDs[1]] * Weights[1];
    BoneTransform     += gBones[BoneIDs[2]] * Weights[2];
    BoneTransform     += gBones[BoneIDs[3]] * Weights[3];
    //BoneTransform     += gBones[BoneIDs[4]] * Weights[4];   // Need a 5th bone

    mat4 model = mat4(vec4(0.1f, 0.0f, 0.0f, 0.0f),
                    vec4(0.0f, 0.1f, 0.0f, 0.0f),
                    vec4(0.0f, 0.0f, 0.1f, 0.0f),
                    vec4(0.0f, 0.0f, 0.0f, 1.0f));

    vec4 PosL    = BoneTransform * vec4(Position, 1.0);
    gl_Position  = gWVP * PosL;
    //gl_Position = gWVP * vec4(Position, 1.0);
    TexCoord0    = TexCoord;
    vec4 NormalL = BoneTransform * vec4(Normal, 0.0);
    Normal0      = (gWorld * NormalL).xyz;
    //Normal0 =
    //WorldPos0    = (gWorld * PosL).xyz;
    WorldPos0 = vec3(gWorld * vec4(Position, 1.0));
}
