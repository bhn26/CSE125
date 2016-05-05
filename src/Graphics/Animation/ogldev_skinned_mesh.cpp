/*
    Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "ogldev_skinned_mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

// aiMatrix4x4 is a row-major matrix!!! Assign the transpose
void Assign(glm::mat4& m, const aiMatrix4x4& aimatrix)
{
    m[0][0] = aimatrix.a1;  m[1][0] = aimatrix.a2;  m[2][0] = aimatrix.a3;  m[3][0] = aimatrix.a4;
    m[0][1] = aimatrix.b1;  m[1][1] = aimatrix.b2;  m[2][1] = aimatrix.b3;  m[3][1] = aimatrix.b4;
    m[0][2] = aimatrix.c1;  m[1][2] = aimatrix.c2;  m[2][2] = aimatrix.c3;  m[3][2] = aimatrix.c4;
    m[0][3] = aimatrix.d1;  m[1][3] = aimatrix.d2;  m[2][3] = aimatrix.d3;  m[3][3] = aimatrix.d4;
}

void SkinnedMesh::VertexBoneData::AddBoneData(uint boneID, float weight)
{
    for (uint i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(IDs) ; i++)
    {
        if (weights[i] == 0.0)
        {
            IDs[i]     = boneID;
            weights[i] = weight;
            return;
        }        
    }
    
    // should never get here - more bones than we have space for
    assert(0);
}

SkinnedMesh::SkinnedMesh()
{
    m_VAO = 0;
    ZERO_MEM(m_Buffers);
    m_NumBones = 0;
    m_pScene = NULL;
}


SkinnedMesh::~SkinnedMesh()
{
    Clear();
}


void SkinnedMesh::Clear()
{
    for (uint i = 0 ; i < m_Textures.size() ; i++)
    {
        SAFE_DELETE(m_Textures[i]);
    }

    if (m_Buffers[0] != 0)
    {
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
    }
       
    if (m_VAO != 0)
    {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
}


bool SkinnedMesh::LoadMesh(const std::string& filename)
{
    // Release the previously loaded mesh (if it exists)
    Clear();
 
    // Create the VAO
    glGenVertexArrays(1, &m_VAO);   
    glBindVertexArray(m_VAO);
    
    // Create the buffers for the vertices attributes
    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    bool ret = false;    
  
    m_pScene = m_Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    
    if (m_pScene)
    {
        // What is this global inverse??
        Assign(m_GlobalInverseTransform, m_pScene->mRootNode->mTransformation);
        m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);
        ret = InitFromScene(m_pScene, filename);
    }
    else
    {
        printf("Error parsing '%s': '%s'\n", filename.c_str(), m_Importer.GetErrorString());
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);	

    return ret;
}


bool SkinnedMesh::InitFromScene(const aiScene* scene, const std::string& filename)
{  
    m_Meshes.resize(scene->mNumMeshes);
    m_Textures.resize(scene->mNumMaterials);

    std::vector<uint> indices;
    std::vector<VertexInfo> vertices;
       
    uint NumVertices = 0;
    uint NumIndices = 0;
    
    // Count the number of vertices and indices
    for (uint i = 0 ; i < m_Meshes.size() ; i++)
    {
        m_Meshes[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;        
        m_Meshes[i].numIndices    = scene->mMeshes[i]->mNumFaces * 3;
        m_Meshes[i].baseVertex    = NumVertices;
        m_Meshes[i].baseIndex     = NumIndices;
        
        NumVertices += scene->mMeshes[i]->mNumVertices;
        NumIndices  += m_Meshes[i].numIndices;
    }
    
    // Reserve space in the vectors for the vertex attributes and indices
    vertices.reserve(NumVertices);
    indices.reserve(NumIndices);
        
    // Initialize the meshes in the scene one by one
    for (uint i = 0 ; i < m_Meshes.size() ; i++)
    {
        const aiMesh* paiMesh = scene->mMeshes[i];
        InitMesh(i, paiMesh, vertices, indices);
    }

    if (!InitMaterials(scene, filename))
    {
        return false;
    }

    // Populate the buffers with vertex attributes and the indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexInfo) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (GLvoid*)0);

    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, texCoords));

    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, normal));

    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_UNSIGNED_INT, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, boneData.IDs));

    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, boneData.weights));

    return GLCheckError();
}

void SkinnedMesh::InitMesh(uint meshIndex, const aiMesh * aiMesh, std::vector<VertexInfo>& vertices, std::vector<unsigned int>& indices)
{
    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    // Populate the vertex attribute vectors
    for (uint i = 0; i < aiMesh->mNumVertices; i++)
    {
        VertexInfo vertex;
        const aiVector3D* pos = &(aiMesh->mVertices[i]);
        const aiVector3D* normal = &(aiMesh->mNormals[i]);
        const aiVector3D* texCoord = aiMesh->HasTextureCoords(0) ? &(aiMesh->mTextureCoords[0][i]) : &zero3D;

        vertex.position = glm::vec3(pos->x, pos->y, pos->z);
        vertex.normal = glm::vec3(normal->x, normal->y, normal->z);
        vertex.texCoords = glm::vec2(texCoord->x, texCoord->y);
        vertices.push_back(vertex);
    }

    printf("Mesh #%d has %d bones.\n", meshIndex, aiMesh->mNumBones);
    LoadBones(meshIndex, aiMesh, vertices);

    // Populate the index buffer
    for (uint i = 0; i < aiMesh->mNumFaces; i++)
    {
        const aiFace& Face = aiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        indices.push_back(Face.mIndices[0]);
        indices.push_back(Face.mIndices[1]);
        indices.push_back(Face.mIndices[2]);
    }
}

void SkinnedMesh::LoadBones(uint meshIndex, const aiMesh* mesh, std::vector<VertexInfo>& vertices)
{
    for (uint i = 0; i < mesh->mNumBones; i++)
    {
        uint boneIndex = 0;
        std::string boneName(mesh->mBones[i]->mName.data);

        if (m_BoneMapping.find(boneName) == m_BoneMapping.end())
        {
            // Allocate an index for a new bone
            boneIndex = m_NumBones;
            m_NumBones++;
            BoneInfo bi;
            m_BoneInfo.push_back(bi);
            Assign(m_BoneInfo[boneIndex].boneOffset, mesh->mBones[i]->mOffsetMatrix);
            m_BoneMapping[boneName] = boneIndex;
        }
        else
        {
            boneIndex = m_BoneMapping[boneName];
        }

        for (uint j = 0; j < mesh->mBones[i]->mNumWeights; j++)
        {
            uint vertexID = m_Meshes[meshIndex].baseVertex + mesh->mBones[i]->mWeights[j].mVertexId;
            float weight = mesh->mBones[i]->mWeights[j].mWeight;
            vertices[vertexID].boneData.AddBoneData(boneIndex, weight);
        }
    }
}

bool SkinnedMesh::InitMaterials(const aiScene* scene, const std::string& filename)
{
    // Extract the directory part from the file name
    std::string::size_type slashIndex = filename.find_last_of("/");
    std::string dir;

    if (slashIndex == std::string::npos)
    {
        dir = ".";
    }
    else if (slashIndex == 0)
    {
        dir = "/";
    }
    else
    {
        dir = filename.substr(0, slashIndex);
    }

    bool ret = true;

    // Initialize the materials
    for (uint i = 0 ; i < scene->mNumMaterials ; i++)
    {
        const aiMaterial* material = scene->mMaterials[i];

        m_Textures[i] = NULL;

        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString path;

            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                std::string p(path.data);
                
                if (p.substr(0, 2) == ".\\")
                {                    
                    p = p.substr(2, p.size() - 2);
                }
                               
                std::string fullPath = dir + "/" + p;
                    
                m_Textures[i] = new Texture(GL_TEXTURE_2D, fullPath.c_str());

                if (!m_Textures[i]->Load())
                {
                    printf("Error loading texture '%s'\n", fullPath.c_str());
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    ret = false;
                }
                else
                {
                    printf("%d - loaded texture '%s'\n", i, fullPath.c_str());
                }
            }
        }
    }

    return ret;
}


void SkinnedMesh::Render()
{
    glBindVertexArray(m_VAO);
    
    for (uint i = 0 ; i < m_Meshes.size() ; i++)
    {
        const uint materialIndex = m_Meshes[i].materialIndex;

        assert(materialIndex < m_Textures.size());
        
        if (m_Textures[materialIndex])
        {
            m_Textures[materialIndex]->Bind(GL_TEXTURE0);
        }

        glDrawElementsBaseVertex(GL_TRIANGLES, 
                                 m_Meshes[i].numIndices, 
                                 GL_UNSIGNED_INT, 
                                 (void*)(sizeof(uint) * m_Meshes[i].baseIndex), 
                                 m_Meshes[i].baseVertex);
    }

    // Make sure the VAO is not changed from the outside    
    glBindVertexArray(0);
}


uint SkinnedMesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{    
    for (uint i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++)
    {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
        {
            return i;
        }
    }

    return pNodeAnim->mNumPositionKeys-1;
    
    //assert(0);
    //return 0;
}


uint SkinnedMesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (uint i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++)
    {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
        {
            return i;
        }
    }

    return pNodeAnim->mNumRotationKeys-1;
    
    //assert(0);
    //return 0;
}


uint SkinnedMesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);
    
    for (uint i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++)
    {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
        {
            return i;
        }
    }

    return pNodeAnim->mNumScalingKeys-1;
    
    //assert(0);
    //return 0;
}


void SkinnedMesh::CalcInterpolatedPosition(aiVector3D& out, float animationTime, const aiNodeAnim* nodeAnim)
{
    if (nodeAnim->mNumPositionKeys == 1)
    {
        out = nodeAnim->mPositionKeys[0].mValue;
        return;
    }
            
    uint positionIndex = FindPosition(animationTime, nodeAnim);
    uint nextPositionIndex = (positionIndex + 1) % nodeAnim->mNumPositionKeys;
    assert(nextPositionIndex < nodeAnim->mNumPositionKeys);

    float deltaTime;
    if (nextPositionIndex == 0)
        deltaTime = (float)(m_pScene->mAnimations[0]->mDuration - nodeAnim->mPositionKeys[positionIndex].mTime);
    else
        deltaTime = (float)(nodeAnim->mPositionKeys[nextPositionIndex].mTime - nodeAnim->mPositionKeys[positionIndex].mTime);
    float factor = (animationTime - (float)nodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);

    const aiVector3D& start = nodeAnim->mPositionKeys[positionIndex].mValue;
    const aiVector3D& end = nodeAnim->mPositionKeys[nextPositionIndex].mValue;
    aiVector3D delta = end - start;
    out = start + factor * delta;
}


void SkinnedMesh::CalcInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* nodeAnim)
{
    // we need at least two values to interpolate...
    if (nodeAnim->mNumRotationKeys == 1)
    {
        out = nodeAnim->mRotationKeys[0].mValue;
        return;
    }
    
    uint rotationIndex = FindRotation(animationTime, nodeAnim);
    uint nextRotationIndex = (rotationIndex + 1) % nodeAnim->mNumRotationKeys;
    assert(nextRotationIndex < nodeAnim->mNumRotationKeys);

    float deltaTime;
    if (nextRotationIndex == 0)
        deltaTime = (float)(m_pScene->mAnimations[0]->mDuration - nodeAnim->mRotationKeys[rotationIndex].mTime);
    else
        deltaTime = (float)(nodeAnim->mRotationKeys[nextRotationIndex].mTime - nodeAnim->mRotationKeys[rotationIndex].mTime);
    float factor = (animationTime - (float)nodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);

    const aiQuaternion& startRotationQ = nodeAnim->mRotationKeys[rotationIndex].mValue;
    const aiQuaternion& endRotationQ   = nodeAnim->mRotationKeys[nextRotationIndex].mValue;    
    aiQuaternion::Interpolate(out, startRotationQ, endRotationQ, factor);
    out = out.Normalize();
}


void SkinnedMesh::CalcInterpolatedScaling(aiVector3D& out, float animationTime, const aiNodeAnim* nodeAnim)
{
    if (nodeAnim->mNumScalingKeys == 1)
    {
        out = nodeAnim->mScalingKeys[0].mValue;
        return;
    }

    uint scalingIndex = FindScaling(animationTime, nodeAnim);
    uint nextScalingIndex = (scalingIndex + 1) % nodeAnim->mNumScalingKeys;
    assert(nextScalingIndex < nodeAnim->mNumScalingKeys);

    float deltaTime;
    if (nextScalingIndex == 0)
        deltaTime = (float)(m_pScene->mAnimations[0]->mDuration - nodeAnim->mScalingKeys[scalingIndex].mTime);
    else
        deltaTime = (float)(nodeAnim->mScalingKeys[nextScalingIndex].mTime - nodeAnim->mScalingKeys[scalingIndex].mTime);
    float factor = (animationTime - (float)nodeAnim->mScalingKeys[scalingIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);

    const aiVector3D& start = nodeAnim->mScalingKeys[scalingIndex].mValue;
    const aiVector3D& end   = nodeAnim->mScalingKeys[nextScalingIndex].mValue;
    aiVector3D delta = end - start;
    out = start + factor * delta;
}


void SkinnedMesh::ReadNodeHeirarchy(float animationTime, const aiNode* node, const glm::mat4& parentTransform)
{    
    std::string NodeName(node->mName.data);
    
    const aiAnimation* pAnimation = m_pScene->mAnimations[0];       // TODO: Change this from 1 animation choice
        
    glm::mat4 nodeTransformation(0.0f);
    Assign(nodeTransformation, node->mTransformation);
     
    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);
    
    if (pNodeAnim)
    {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D scaling;
        CalcInterpolatedScaling(scaling, animationTime, pNodeAnim);
        glm::mat4 scalingM = glm::scale(glm::mat4(1.0f), glm::vec3(scaling.x, scaling.y, scaling.z));

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion rotationQ;
        CalcInterpolatedRotation(rotationQ, animationTime, pNodeAnim);
        glm::mat4 rotationM = static_cast<glm::mat4>(glm::quat(rotationQ.w, rotationQ.x, rotationQ.y, rotationQ.z));

        // Interpolate translation and generate translation transformation matrix
        aiVector3D translation;
        CalcInterpolatedPosition(translation, animationTime, pNodeAnim);
        glm::mat4 translationM = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z));

        // Combine the above transformations
        nodeTransformation = translationM * rotationM * scalingM;
    }
       
    glm::mat4 GlobalTransformation = parentTransform * nodeTransformation;
    
    if (m_BoneMapping.find(NodeName) != m_BoneMapping.end())
    {
        uint BoneIndex = m_BoneMapping[NodeName];
        m_BoneInfo[BoneIndex].finalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].boneOffset;
    }
    
    for (uint i = 0 ; i < node->mNumChildren ; i++)
    {
        ReadNodeHeirarchy(animationTime, node->mChildren[i], GlobalTransformation);
    }
}


void SkinnedMesh::BoneTransform(float timeInSeconds, std::vector<glm::mat4>& transforms)
{
    glm::mat4 identity(1.0f);
    
    float ticksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
    float timeInTicks = timeInSeconds * ticksPerSecond;
    float animationTime = fmod(timeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

    ReadNodeHeirarchy(animationTime, m_pScene->mRootNode, identity);

    transforms.resize(m_NumBones);

    for (uint i = 0 ; i < m_NumBones ; i++)
    {
        transforms[i] = m_BoneInfo[i].finalTransformation;
    }
}


const aiNodeAnim* SkinnedMesh::FindNodeAnim(const aiAnimation* animation, const std::string nodeName)
{
    for (uint i = 0 ; i < animation->mNumChannels ; i++)
    {
        const aiNodeAnim* nodeAnim = animation->mChannels[i];
        
        if (std::string(nodeAnim->mNodeName.data) == nodeName)
        {
            return nodeAnim;
        }
    }
    
    return NULL;
}