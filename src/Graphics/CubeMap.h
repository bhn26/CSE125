//
//  CubeMap.h
//  egg scramble
//

#ifndef __egg_scramble__CubeMap__
#define __egg_scramble__CubeMap__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <memory>

class Camera;
class Shader;

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
