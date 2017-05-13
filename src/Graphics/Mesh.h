#pragma once

/* A mesh class taken from learnopengl.com
 */

// Std. Includes
#include <string>
#include <vector>

// GL Includes
#include <assimp/types.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"

class Mesh
{
    friend class Model;
    struct Vertex
    {
        glm::vec3 m_position;  // Position
        glm::vec3 m_normal;    // Normal
        glm::vec2 m_texCoords; // TexCoords
    };

    struct Texture
    {
        GLuint m_id;
        std::string m_type;
        aiString m_path;
    };

public:
    /*  Mesh Data  */
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    std::vector<Texture> m_textures;
    Material m_material;

    /*  Functions  */
    // Constructor
    Mesh(std::vector<Vertex> vertices,
         std::vector<GLuint> indices,
         std::vector<Texture> textures,
         Material material);
    ~Mesh();

    GLuint VAO() const { return m_vao; }
    GLuint VBO() const { return m_vbo; }
    GLuint EBO() const { return m_ebo; }

    // Render the mesh
    void Draw(const Shader* shader);

private:
    /*  Render data  */
    GLuint m_vao, m_vbo, m_ebo;

    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void SetupMesh();
    void SetMaterial(const Shader* shader, const Material& material) const;
};
