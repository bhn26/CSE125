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

class Mesh
{
    friend class Model;
    struct Vertex
    {
        glm::vec3 position;     // Position
        glm::vec3 normal;       // Normal
        glm::vec2 texCoords;    // TexCoords
    };

    struct Texture
    {
        GLuint id;
        std::string type;
        aiString path;
    };

public:
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    /*  Functions  */
    // Constructor
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	~Mesh();

	GLuint VAO() const { return this->vao; }
	GLuint VBO() const { return this->vbo; }
	GLuint EBO() const { return this->ebo; }

    // Render the mesh
    void Draw(const Shader* shader);

private:
    /*  Render data  */
    GLuint vao, vbo, ebo;

    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void SetupMesh();
};



