//
//  CubeMap.h
//  egg scramble
//

#ifndef __egg_scramble__CubeMap__
#define __egg_scramble__CubeMap__

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/mat3x3.hpp> // glm::mat3
#include <glm/vec3.hpp>
#include <vector>
#include <memory>

#include "Shader.h"
#include "Camera.h"
#include "../Window.h"

class CubeMap
{
public:
    CubeMap();
    ~CubeMap();
    
    glm::mat4 toWorld;
    glm::mat3 normalMatrix;
    GLuint textureID;
    
    GLuint cubeMapVBO, cubeMapVAO, cubeMapEBO;
    std::shared_ptr<Shader> shader;
    std::vector<const GLchar*> faces;
    
    GLuint LoadCubeMap();
    void Draw(Camera* camera);
};


#endif /* defined(__egg_scramble__CubeMap__) */
