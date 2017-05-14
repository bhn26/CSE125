//
//  CubeMap.cpp
//  egg scramble
//

#include "CubeMap.h"
#include "../Scene.h"
#include "../Lights.h"
#include "../Shader.h"
#include "../Camera.h"
#include "../../client/Window.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

// Other Libs
#include <SOIL/SOIL.h>

CubeMap::CubeMap() : Entity(glm::scale(glm::mat4(1.0f), glm::vec3(10.0f)))
{
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

    // Setup skybox m_VAO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    
    // Cubemap (Skybox)
	m_faces.push_back("assets/cubemap/blue_right.png");
	m_faces.push_back("assets/cubemap/blue_left.png");
	m_faces.push_back("assets/cubemap/blue_up.png");
	m_faces.push_back("assets/cubemap/blue_down.png");
	m_faces.push_back("assets/cubemap/blue_back.png");
	m_faces.push_back("assets/cubemap/blue_front.png");
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
    for(GLuint i = 0; i < m_faces.size(); i++)
    {
        image = SOIL_load_image(m_faces[i], &width, &height, 0, SOIL_LOAD_RGB);
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
    
    m_textureID = textureID;
    return textureID;
}

void CubeMap::Draw() const
{
    if (Scene::Instance()->IsRenderingDepth())
        return;
    // Draw skybox as last
    glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
    // Use the appropriate m_shader (depth or model)
    UseShader();

    // skybox cube
    glBindVertexArray(m_VAO);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(m_shader->GetUniform("skybox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // Set depth function back to default
}

void CubeMap::SetShaderUniforms() const
{
    glUniformMatrix4fv(m_shader->GetUniform("view"), 1, false, glm::value_ptr(glm::mat4(glm::mat3(Scene::Instance()->GetViewMatrix()))));
    //glUniformMatrix4fv(m_shader->GetUniform("view"), 1, false, glm::value_ptr(Scene::Instance()->GetViewMatrix()));
    glUniformMatrix4fv(m_shader->GetUniform("model"), 1, false, glm::value_ptr(m_toWorld));
    glUniformMatrix4fv(m_shader->GetUniform("projection"), 1, false, glm::value_ptr(Scene::Instance()->GetPerspectiveMatrix()));
}
