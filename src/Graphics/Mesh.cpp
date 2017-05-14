#include "Mesh.h"

#include <fstream>
#include <sstream>
#include <iostream>

// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

Mesh::Mesh(std::vector<Vertex> v, std::vector<GLuint> i, std::vector<Mesh::Texture> t, Material m)
    : m_vertices(v), m_indices(i), m_textures(t), m_material(m)
{
    // Now that we have all the required data, set the vertex buffers and its attribute pointers.
    SetupMesh();
}

Mesh::~Mesh()
{
    if (m_vao)
        glDeleteBuffers(1, &m_vao);
    if (m_vbo)
        glDeleteBuffers(1, &m_vbo);
    if (m_ebo)
        glDeleteBuffers(1, &m_ebo);
}

void Mesh::Draw(const Shader* shader)
{
    if (shader)
    {
        shader->Use();

        // Bind appropriate textures
        GLuint diffuseNr = 1;
        GLuint specularNr = 1;

        // Textures
        if (!m_textures.size())
        {
            glUniform1i(shader->GetUniform("useTexture"), FALSE);
        }
        for (GLuint i = 0; i < m_textures.size(); i++)
        {
            glUniform1i(shader->GetUniform("useTexture"), TRUE); // I think this is right??
            glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
                                              // Retrieve texture number (the N in diffuse_textureN)
            std::stringstream ss;
            std::string number;
            std::string name = m_textures[i].m_type;
            if (name == "texture_diffuse")
                ss << diffuseNr++; // Transfer GLuint to stream
            else if (name == "texture_specular")
                ss << specularNr++; // Transfer GLuint to stream
            number = ss.str();
            // Now set the sampler to the correct texture unit
            glUniform1i(shader->GetUniform((name + number).c_str()), i);
            // And finally bind the texture
            glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
        }

        // Also set each mesh's shininess property to a default value (if you want you could extend
        // this to another mesh property and possibly change this value)
        glUniform1f(shader->GetUniform("material.shininess"), 16.0f);

        if (m_material.Valid()) // Test if there's a diffuse
        {
            SetMaterial(shader, m_material);
            glUniform1i(shader->GetUniform("useMaterial"), TRUE);
        }
        else
        {
            glUniform1i(shader->GetUniform("useMaterial"), FALSE);
        }
    }

    // Draw mesh
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Always good practice to set everything back to defaults once configured.
    // Textures
    for (GLuint i = 0; shader && i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Mesh::SetupMesh()
{
    // Create buffers/arrays
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to
    // a glm::vec3/2 array which again translates to 3/2 floats which translates to a byte array.
    glBufferData(
        GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_normal));

    // Vertex Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, m_texCoords));

    glBindVertexArray(0);
}

void Mesh::SetMaterial(const Shader* shader, const Material& material) const
{
    glUniform3fv(shader->GetUniform("material._diffuse"), 1, glm::value_ptr(material.m_diffuse));
    glUniform3fv(shader->GetUniform("material._specular"), 1, glm::value_ptr(material.m_specular));
    glUniform3fv(shader->GetUniform("material._ambient"), 1, glm::value_ptr(material.m_ambient));
    glUniform1f(shader->GetUniform("material._shininess"), material.m_shininess);
}
