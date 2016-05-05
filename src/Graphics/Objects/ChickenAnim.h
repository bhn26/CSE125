//
//  Chicken.hpp
//  egg scramble
//
//  Created by Phoebe on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#ifndef ChickenAnim_h
#define ChickenAnim_h

#include <stdio.h>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>

#include "../../Window.h"
#include "../Camera.h"
#include "Entity.h"

#include "../Animation/skinning_technique.h"
#include "../Animation/ogldev_skinned_mesh.h"

class ChickenAnim : public Entity
{
public:
    SkinningTechnique* m_pEffect;
    DirectionalLight m_directionalLight;
    SkinnedMesh m_mesh;
    glm::vec3 m_position;

    glm::mat4 m_toWorld;
    
    ChickenAnim();
    ~ChickenAnim();
    
    void Draw();
    void Update();
};

#endif /* ChickenAnim_h */
