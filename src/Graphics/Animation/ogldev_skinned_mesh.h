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

#ifndef OGLDEV_SKINNED_MESH_H
#define OGLDEV_SKINNED_MESH_H

#include <map>
#include <vector>
#include <glm/glm.hpp>

#include <GL/glew.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include "ogldev_util.h"
#include "ogldev_texture.h"

class SkinnedMesh
{
public:
    SkinnedMesh();

    ~SkinnedMesh();

    bool LoadMesh(const std::string& Filename);

    void Render();

    uint NumBones() const
    {
        return m_NumBones;
    }
    
    void BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms);
    
private:
    #define NUM_BONES_PER_VEREX 8 // we have max 5

    struct BoneInfo
    {
        glm::mat4 BoneOffset;
        glm::mat4 FinalTransformation;        

        BoneInfo() : BoneOffset(glm::mat4(0.0f)), FinalTransformation(glm::mat4(0.0f))
        {
        }
    };
    
    struct VertexBoneData
    {        
        uint IDs[NUM_BONES_PER_VEREX];
        float Weights[NUM_BONES_PER_VEREX];

        VertexBoneData()
        {
            Reset();
        };
        
        void Reset()
        {
            ZERO_MEM(IDs);
            ZERO_MEM(Weights);        
        }
        
        void AddBoneData(uint BoneID, float Weight);
    };

    struct VertexInfo
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
        VertexBoneData boneData;
    };

    void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
    void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);    
    uint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
    uint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
    uint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);
    void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
    bool InitFromScene(const aiScene* pScene, const std::string& Filename);
    void InitMesh(uint MeshIndex,
                  const aiMesh* paiMesh,
                  std::vector<glm::vec3>& Positions,
                  std::vector<glm::vec3>& Normals,
                  std::vector<glm::vec2>& TexCoords,
                  std::vector<VertexBoneData>& Bones,
                  std::vector<unsigned int>& Indices);
    void InitMesh(uint meshIndex,
        const aiMesh* paiMesh,
        std::vector<VertexInfo>& vertices,
        std::vector<unsigned int>& indices);
    void LoadBones(uint MeshIndex, const aiMesh* paiMesh, std::vector<VertexBoneData>& Bones);
    void LoadBones(uint MeshIndex, const aiMesh* paiMesh, std::vector<VertexInfo>& vertices);
    bool InitMaterials(const aiScene* pScene, const std::string& Filename);
    void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF
  
enum VB_TYPES {
    INDEX_BUFFER,
    POS_VB,
    NORMAL_VB,
    TEXCOORD_VB,
    BONE_VB,
    NUM_VBs            
};

    GLuint m_VAO;
    GLuint m_Buffers[NUM_VBs];

    struct MeshEntry {
        MeshEntry()
        {
            NumIndices    = 0;
            BaseVertex    = 0;
            BaseIndex     = 0;
            MaterialIndex = INVALID_MATERIAL;
        }
        
        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };
    
    std::vector<MeshEntry> m_Entries;
    std::vector<Texture*> m_Textures;
     
    std::map<std::string,uint> m_BoneMapping; // maps a bone name to its index
    uint m_NumBones;
    std::vector<BoneInfo> m_BoneInfo;
    glm::mat4 m_GlobalInverseTransform;
    
    const aiScene* m_pScene;
    Assimp::Importer m_Importer;
};


#endif	/* OGLDEV_SKINNED_MESH_H */

