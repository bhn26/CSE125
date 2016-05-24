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

    unsigned int NumBones() const
    {
        return m_NumBones;
    }
    
    void BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms);
    
    const aiScene* Scene() const { return m_pScene; }

private:
    //#define NUM_BONES_PER_VEREX 8 // we have max 5
    static const int NUM_BONES_PER_VERTEX = 8;

    struct BoneInfo
    {
        glm::mat4 boneOffset;
        glm::mat4 finalTransformation;        

        BoneInfo() : boneOffset(glm::mat4(0.0f)), finalTransformation(glm::mat4(0.0f))
        {
        }
    };
    
    struct VertexBoneData
    {
        unsigned int IDs[NUM_BONES_PER_VERTEX];
        float weights[NUM_BONES_PER_VERTEX];

        VertexBoneData()
        {
            Reset();
        };
        
        void Reset()
        {
            ZERO_MEM(IDs);
            ZERO_MEM(weights);        
        }
        
        void AddBoneData(unsigned int boneID, float weight);
    };

    struct VertexInfo
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
        VertexBoneData boneData;
    };

    void CalcInterpolatedScaling(aiVector3D& out, float animationTime, const aiNodeAnim* nodeAnim);
    void CalcInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* nodeAnim);
    void CalcInterpolatedPosition(aiVector3D& out, float animationTime, const aiNodeAnim* nodeAnim);

    unsigned int FindScaling(float animationTime, const aiNodeAnim* nodeAnim);
    unsigned int FindRotation(float animationTime, const aiNodeAnim* nodeAnim);
    unsigned int FindPosition(float animationTime, const aiNodeAnim* nodeAnim);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName);

    void ReadNodeHeirarchy(float animationTime, const aiNode* node, const glm::mat4& parentTransform);
    bool InitFromScene(const aiScene* scene, const std::string& filename);
    void InitMesh(unsigned int meshIndex,
        const aiMesh* aiMesh,
        std::vector<VertexInfo>& vertices,
        std::vector<unsigned int>& indices);
    void LoadBones(unsigned int MeshIndex, const aiMesh* paiMesh, std::vector<VertexInfo>& vertices);
    bool InitMaterials(const aiScene* scene, const std::string& filename);

    void Clear();

    const static int INVALID_MATERIAL = 0xFFFFFFFF;
  
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

    struct MeshEntry
    {
        MeshEntry()
        {
            numIndices    = 0;
            baseVertex    = 0;
            baseIndex     = 0;
            materialIndex = INVALID_MATERIAL;
        }
        
        unsigned int numIndices;
        unsigned int baseVertex;
        unsigned int baseIndex;
        unsigned int materialIndex;
    };
    
    std::vector<MeshEntry> m_Meshes;
    std::vector<Texture*> m_Textures;
     
    std::map<std::string,unsigned int> m_BoneMapping; // maps a bone name to its index
    unsigned int m_NumBones;
    std::vector<BoneInfo> m_BoneInfo;
    glm::mat4 m_GlobalInverseTransform;
    
    const aiScene* m_pScene;
    Assimp::Importer m_Importer;
};


#endif	/* OGLDEV_SKINNED_MESH_H */

