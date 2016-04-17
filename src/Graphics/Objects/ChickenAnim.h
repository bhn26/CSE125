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

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/mat3x3.hpp> // glm::mat3
#include <glm/vec3.hpp>
#include <memory>

#include "../../Window.h"
#include "../Camera.h"
#include "Entity.h"

#include "skinning_technique.h"
#include "ogldev_skinned_mesh.h"

class ChickenAnim : public Entity {
public:
    SkinningTechnique* m_pEffect;
    DirectionalLight m_directionalLight;
    SkinnedMesh m_mesh;
    Vector3f m_position;
    PersProjInfo m_persProjInfo;
    
    ChickenAnim();
    ~ChickenAnim();
    
    void Draw(Camera * camera);
    void Update();
};

#endif /* ChickenAnim_h */
