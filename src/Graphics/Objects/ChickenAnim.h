//
//  Chicken.hpp
//  egg scramble
//
//  Created by Phoebe on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#pragma once

#include "Entity.h"

#include "../Animation/skinning_technique.h"
#include "../Animation/AnimatedModel.h"

class ChickenAnim : public Entity
{
public:
    DirectionalLight m_directionalLight;
    Animation::AnimatedModel m_object;

    glm::mat4 m_toWorld;

    ChickenAnim();
    ~ChickenAnim();

    void Draw();
    void Update();
    void Scale(float s);
};

