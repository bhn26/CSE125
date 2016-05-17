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

#include "Entity.h"

class CubeMap : public Entity
{
public:
    CubeMap(GLfloat size);
    ~CubeMap();
    
	static GLfloat size;
    GLuint textureID;
    std::vector<const GLchar*> faces;
    
    GLuint LoadCubeMap();
    virtual void Draw() const override;
    virtual void Update() override {}

    // Inherited via Entity
    virtual void Spawn(float x, float y, float z) override;
};


#endif /* defined(__egg_scramble__CubeMap__) */
