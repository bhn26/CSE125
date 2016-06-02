#include "AnimationMesh.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

///////////////////////////////////////////////////////////////////////
// aiMatrix4x4 is a row-major matrix!!! Assign the transpose
void Animation::Assign(glm::mat4& m, const aiMatrix4x4& aimatrix)
{
    m[0][0] = aimatrix.a1;  m[1][0] = aimatrix.a2;  m[2][0] = aimatrix.a3;  m[3][0] = aimatrix.a4;
    m[0][1] = aimatrix.b1;  m[1][1] = aimatrix.b2;  m[2][1] = aimatrix.b3;  m[3][1] = aimatrix.b4;
    m[0][2] = aimatrix.c1;  m[1][2] = aimatrix.c2;  m[2][2] = aimatrix.c3;  m[3][2] = aimatrix.c4;
    m[0][3] = aimatrix.d1;  m[1][3] = aimatrix.d2;  m[2][3] = aimatrix.d3;  m[3][3] = aimatrix.d4;
}

///////////////////////////////////////////////////////////////////////
void Animation::Mesh::VertexBoneData::AddBoneData(unsigned int boneID, float weight)
{
    for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; i++)
    {
        if (weights[i] == 0.0)
        {
            IDs[i] = boneID;
            weights[i] = weight;
            return;
        }
    }

    // should never get here - more bones than we have space for
    assert(0);
}

///////////////////////////////////////////////////////////////////////
Animation::Mesh::Mesh() : m_VAO(0), m_VBO(0), m_EBO(0), m_numBones(0), m_rootNode(std::unique_ptr<Node>(new Node()))
{
    m_skinningTechnique = std::shared_ptr<SkinningTechnique>(new SkinningTechnique());

    if (!m_skinningTechnique->Init())
    {
        printf("Error initializing the lighting technique\n");
    }
}


///////////////////////////////////////////////////////////////////////
Animation::Mesh::~Mesh()
{
    Clear();
}


///////////////////////////////////////////////////////////////////////
void Animation::Mesh::Clear()
{
    if (m_VBO != 0)
    {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    if (m_EBO != 0)
    {
        glDeleteBuffers(1, &m_EBO);
        m_EBO = 0;
    }

    if (m_VAO != 0)
    {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
}

///////////////////////////////////////////////////////////////////////
bool Animation::Mesh::GenBuffers()
{
    // Create the buffers for the vertices attributes and VAO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    return glGetError() == GL_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////
bool Animation::Mesh::InitFromScene(const aiScene* scene, const std::string& filename)
{
    Clear();
    GenBuffers();

    m_meshes.resize(scene->mNumMeshes);

    std::vector<unsigned int> indices;
    std::vector<VertexInfo> vertices;

    m_numVertices = 0;
    m_numIndices = 0;

    Assign(m_GlobalInverseTransform, scene->mRootNode->mTransformation);
    m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);

    // Count the number of vertices and indices
    for (unsigned int i = 0; i < m_meshes.size(); i++)
    {
        m_meshes[i].materialIndex = scene->mMeshes[i]->mMaterialIndex;
        m_meshes[i].numIndices = scene->mMeshes[i]->mNumFaces * 3;
        m_meshes[i].baseVertex = m_numVertices;
        m_meshes[i].baseIndex = m_numIndices;

        m_numVertices += scene->mMeshes[i]->mNumVertices;
        m_numIndices += m_meshes[i].numIndices;
    }

    // Reserve space in the vectors for the vertex attributes and indices
    vertices.reserve(m_numVertices);
    indices.reserve(m_numIndices);

    InitNodeHierarchy(m_rootNode, scene->mRootNode);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < m_meshes.size(); i++)
    {
        const aiMesh* paiMesh = scene->mMeshes[i];
        InitMesh(i, paiMesh, vertices, indices);
    }

    if (!InitMaterials(scene, filename))
        return false;

    // Populate the buffers with vertex attributes and the indices
    glBindVertexArray(m_VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

        // Load data
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexInfo) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(POSITION_LOCATION);
        glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo),
            (GLvoid*)0);

        glEnableVertexAttribArray(TEX_COORD_LOCATION);
        glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInfo),
            (GLvoid*)offsetof(VertexInfo, texCoords));

        glEnableVertexAttribArray(NORMAL_LOCATION);
        glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo),
            (GLvoid*)offsetof(VertexInfo, normal));

        glEnableVertexAttribArray(BONE_ID_LOCATION);
        glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_UNSIGNED_INT, sizeof(VertexInfo),    // Cannot pass more than 4!!
            (GLvoid*)offsetof(VertexInfo, boneData.IDs));

        glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
        glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexInfo),    // Cannot pass more than 4!!
            (GLvoid*)offsetof(VertexInfo, boneData.weights));
    glBindVertexArray(0);       // Make sure the VAO is not changed from the outside

    return glGetError() == GL_NO_ERROR;
}

///////////////////////////////////////////////////////////////////////
void Animation::Mesh::InitNodeHierarchy(std::unique_ptr<Node>& node, aiNode* ainode)
{
    node->_name = std::string(ainode->mName.C_Str());
    Assign(node->_nodeTransform, ainode->mTransformation);
    for (unsigned int i = 0; i < ainode->mNumChildren; i++)
    {
        node->_children.push_back(std::unique_ptr<Node>(new Node()));
        InitNodeHierarchy(node->_children[i], ainode->mChildren[i]);
    }
}

///////////////////////////////////////////////////////////////////////
void Animation::Mesh::InitMesh(unsigned int meshIndex, const aiMesh* aiMesh,
    std::vector<VertexInfo>& vertices, std::vector<unsigned int>& indices)
{
    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    // Populate the vertex attribute vectors
    for (unsigned int i = 0; i < aiMesh->mNumVertices; i++)
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
    for (unsigned int i = 0; i < aiMesh->mNumFaces; i++)
    {
        const aiFace& Face = aiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        indices.push_back(Face.mIndices[0]);
        indices.push_back(Face.mIndices[1]);
        indices.push_back(Face.mIndices[2]);
    }
}

///////////////////////////////////////////////////////////////////////
void Animation::Mesh::LoadBones(unsigned int meshIndex, const aiMesh* mesh, std::vector<VertexInfo>& vertices)
{
    for (unsigned int i = 0; i < mesh->mNumBones; i++)
    {
        unsigned int boneIndex = 0;
        std::string boneName(mesh->mBones[i]->mName.data);

        if (m_boneMapping.find(boneName) == m_boneMapping.end())
        {
            // Allocate an index for a new bone
            boneIndex = m_numBones;
            m_numBones++;
            BoneInfo bi;
            m_boneInfo.push_back(bi);
            Assign(m_boneInfo[boneIndex].boneOffset, mesh->mBones[i]->mOffsetMatrix);
            m_boneMapping[boneName] = boneIndex;
        }
        else
        {
            boneIndex = m_boneMapping[boneName];
        }

        for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
        {
            unsigned int vertexID = m_meshes[meshIndex].baseVertex + mesh->mBones[i]->mWeights[j].mVertexId;
            float weight = mesh->mBones[i]->mWeights[j].mWeight;
            vertices[vertexID].boneData.AddBoneData(boneIndex, weight);
        }
    }
}

///////////////////////////////////////////////////////////////////////
bool Animation::Mesh::InitMaterials(const aiScene* scene, const std::string& filename)
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
    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        const aiMaterial* aimaterial = scene->mMaterials[i];

        if (aimaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString path;

            if (aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
            {
                std::string p(path.data);

                if (p.substr(0, 2) == ".\\")
                {
                    p = p.substr(2, p.size() - 2);
                }

                std::string fullPath = dir + "/" + p;

                m_textures[i] = Texture(GL_TEXTURE_2D, fullPath.c_str());

                if (!m_textures[i].Load())
                {
                    printf("Error loading texture '%s'\n", fullPath.c_str());
                    m_textures.erase(m_textures.find(i));       // Remove
                    ret = false;
                }
                else
                {
                    printf("%d - loaded texture '%s'\n", i, fullPath.c_str());
                }
            }
        }
        else        // Use a material instead of texture
        {
            Material& material = m_materials[i];
            aiColor3D diffuse;
            aiColor3D specular;
            aiColor3D ambient;
            aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
            aimaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
            aimaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);

            material._diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
            material._specular = glm::vec3(specular.r, specular.g, specular.b);
            material._ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
            aimaterial->Get(AI_MATKEY_SHININESS, material._shininess);
        }
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////
void Animation::Mesh::Draw(bool useShader) const
{
    glBindVertexArray(m_VAO);

    for (unsigned int i = 0; i < m_boneInfo.size(); i++)
    {
        m_skinningTechnique->SetBoneTransform(i, m_boneInfo[i].finalTransformation);
    }

    for (unsigned int i = 0; i < m_meshes.size(); i++)
    {
        const unsigned int materialIndex = m_meshes[i].materialIndex;

        if (m_textures.find(materialIndex) != m_textures.end())
        {
            m_textures.at(materialIndex).Bind(GL_TEXTURE0);
            m_skinningTechnique->SetUseTexture(true);
        }
        else if (m_materials.find(materialIndex) != m_materials.end())
        {
            m_skinningTechnique->SetMaterial(m_materials.at(materialIndex));
            m_skinningTechnique->SetUseTexture(false);
        }

        glDrawElementsBaseVertex(GL_TRIANGLES,
            m_meshes[i].numIndices,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int) * m_meshes[i].baseIndex),
            m_meshes[i].baseVertex);
    }

    // Make sure the VAO is not changed from the outside    
    glBindVertexArray(0);
}

bool Animation::Mesh::SameMesh(const aiScene* scene) const
{
    if (scene->mNumMeshes != m_meshes.size())
        return false;
    int numVerts = 0;
    int numInd = 0;
    for (unsigned int i = 0; i < m_meshes.size(); i++)
    {
        numVerts += scene->mMeshes[i]->mNumVertices;
        numInd += m_meshes[i].numIndices;
    }

    if (m_numVertices != numVerts || m_numIndices != numInd)
        return false;

    return true;
}
