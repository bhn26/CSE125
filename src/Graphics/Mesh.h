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

class Mesh
{
public:
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    /*  Functions  */
    // Constructor
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);


    // Render the mesh
    void Draw(Shader shader);

private:
    /*  Render data  */
    GLuint VAO, VBO, EBO;

    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void SetupMesh();
};



