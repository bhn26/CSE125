#pragma once

/* A Model class from learnopengl.com
*/

// Std. Includes
#include <string>
#include <vector>
#include <assimp/types.h>
#include <assimp/scene.h>

#include "Mesh.h"

GLint TextureFromFile(const char* path, std::string directory);

class Model
{
public:
    /*  Functions   */
    // Constructor, expects a filepath to a 3D model.
    // "assets/model
    Model(const GLchar* path) { this->LoadModel(path); }

    // Draws the model, and thus all its meshes
    void Draw(const Shader* shader)
    {
        for (GLuint i = 0; i < this->meshes.size(); i++)
            this->meshes[i].Draw(shader);
    }

private:
    /*  Model Data  */
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<TextureData> textures_loaded;   // Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

     /*  Functions   */
    void LoadModel(std::string path);  // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.

    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

    // Checks all material textures of a given type and loads the textures if they're not loaded yet.
    // The required info is returned as a Texture struct.
    std::vector<TextureData> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
