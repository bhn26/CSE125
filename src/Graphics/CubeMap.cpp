//
//  CubeMap.cpp
//  egg scramble
//

#include "CubeMap.h"
#include "Scene.h"
#include "PointLight.h"
#include "Shader.h"
#include "Camera.h"
#include "../Window.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

// Other Libs
#include <SOIL.h>

GLfloat skyboxVertices[] = {
    // Positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    
    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

CubeMap::CubeMap() : toWorld(glm::scale(glm::mat4(1.0f), glm::vec3(50.0f)))
{
    // Setup skybox VAO
    glGenVertexArrays(1, &cubeMapVAO);
    glGenBuffers(1, &cubeMapVBO);
    glBindVertexArray(cubeMapVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    
    // Cubemap (Skybox)
    faces.push_back("assets/cubemap/right.jpg");
    faces.push_back("assets/cubemap/left.jpg");
    faces.push_back("assets/cubemap/top.jpg");
    faces.push_back("assets/cubemap/bottom.jpg");
    faces.push_back("assets/cubemap/back.jpg");
    faces.push_back("assets/cubemap/front.jpg");
}

CubeMap::~CubeMap()
{
}

GLuint CubeMap::LoadCubeMap(){
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    int width, height;
    unsigned char* image;
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        if (!image)
        {
            fprintf(stderr, "Error: %s\n", SOIL_last_result);
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    this->textureID = textureID;
    return textureID;
}

void CubeMap::Draw(Camera* camera)
{
    // Draw skybox as last
    glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
    shader->Use();
    glm::mat4 projection = glm::perspective(camera->Zoom(), (float)Window::width/(float)Window::height, 0.1f, 1000.0f);
    
    GLint viewLoc = shader->GetUniform("view");
    GLint modelLocation = shader->GetUniform("model");
    GLint projectionLocation = shader->GetUniform("projection");
    
    
    glUniformMatrix4fv(viewLoc, 1, false, glm::value_ptr(Scene::GetViewMatrix()));
    glUniformMatrix4fv(modelLocation, 1, false, glm::value_ptr(this->toWorld));
    glUniformMatrix4fv(projectionLocation, 1, false, glm::value_ptr(projection));
    
    // skybox cube
    glBindVertexArray(cubeMapVAO);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(shader->GetUniform("skybox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // Set depth function back to default
}