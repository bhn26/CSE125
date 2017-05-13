#pragma once

#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <memory>

#include "Graphics/Texture.h"
#include "skinning_technique.h"
#include "Basic/Utils.h"

struct aiScene;
template <typename TReal> class aiMatrix4x4t;
typedef aiMatrix4x4t<float> aiMatrix4x4;
struct aiMesh;
struct aiNode;

namespace Animation
{
    ///////////////////////////////////////////////////////////////////////
    // Assigns a row-major aiMatrix4x4 to a glm::mat4
    void Assign(glm::mat4& m, const aiMatrix4x4& aimatrix);

    ///////////////////////////////////////////////////////////////////////
    class Mesh
    {
        friend class AnimationPlayer;
        friend class AnimatedModel;

    public:
        ///////////////////////////////////////////////////////////////////////
        // Public Methods
        Mesh();
        ~Mesh();

        bool GenBuffers();
        bool InitFromScene(const aiScene* scene, const std::string& filename);
        void Draw(bool useShader = true) const;

        bool SameMesh(const aiScene* scene) const;
        unsigned int NumBones() const { return m_numBones; }
        SkinningTechnique* GetSkinningTechnique() const { return m_skinningTechnique.get(); }

    private:
        ///////////////////////////////////////////////////////////////////////
        // Constants
        static const int NUM_BONES_PER_VERTEX = 8; // we have max 5
        static const int INVALID_MATERIAL = 0xFFFFFFFF;

        static const int POSITION_LOCATION = 0;
        static const int TEX_COORD_LOCATION = 1;
        static const int NORMAL_LOCATION = 2;
        static const int BONE_ID_LOCATION = 3;
        static const int BONE_WEIGHT_LOCATION = 4;

        ///////////////////////////////////////////////////////////////////////
        // Structs
        struct BoneInfo
        {
            glm::mat4 m_boneOffset;
            glm::mat4 m_finalTransformation;

            BoneInfo() : m_boneOffset(glm::mat4(0.0f)), m_finalTransformation(glm::mat4(0.0f)) {}
        };

        struct VertexBoneData
        {
            unsigned int m_IDs[NUM_BONES_PER_VERTEX];
            float m_weights[NUM_BONES_PER_VERTEX];

            VertexBoneData() { Reset(); };
            void Reset()
            {
                ZERO_MEM(m_IDs);
                ZERO_MEM(m_weights);
            }
            void AddBoneData(unsigned int boneID, float weight);
        };

        struct VertexInfo
        {
            glm::vec3 m_position;
            glm::vec3 m_normal;
            glm::vec2 m_texCoords;
            VertexBoneData m_boneData;
        };

        struct MeshEntry
        {
            unsigned int m_numIndices = 0;
            unsigned int m_baseVertex = 0;
            unsigned int m_baseIndex = 0;
            unsigned int m_materialIndex = INVALID_MATERIAL;
        };

        struct Node
        {
            std::string m_name;
            std::vector<std::unique_ptr<Node>> m_children;
            glm::mat4 m_nodeTransform = glm::mat4(1.0f);
        };

        ///////////////////////////////////////////////////////////////////////
        // Member Methods
        void InitNodeHierarchy(std::unique_ptr<Node>& node, aiNode* ainode);
        void InitMesh(unsigned int meshIndex,
                      const aiMesh* aiMesh,
                      std::vector<VertexInfo>& vertices,
                      std::vector<unsigned int>& indices);
        void LoadBones(unsigned int MeshIndex,
                       const aiMesh* paiMesh,
                       std::vector<VertexInfo>& vertices);
        bool InitMaterials(const aiScene* scene, const std::string& filename);

        void Clear();

        ///////////////////////////////////////////////////////////////////////
        // Member variables
        GLuint m_VAO, m_VBO, m_EBO;

        std::vector<MeshEntry> m_meshes;
        std::unordered_map<int, Texture> m_textures;
        std::unordered_map<int, Material> m_materials;
        std::unique_ptr<Node> m_rootNode;

        std::shared_ptr<SkinningTechnique> m_skinningTechnique; // HACKY

        unsigned int m_numBones, m_numVertices, m_numIndices;
        std::unordered_map<std::string, unsigned int>
            m_boneMapping; // maps a bone name to its index
        std::vector<BoneInfo> m_boneInfo;
        glm::mat4 m_globalInverseTransform;
    };
}