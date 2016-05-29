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
    std::string m_walk, m_attack, m_jump, m_dance;
public:
    Animation::DirectionalLight m_directionalLight;
    Animation::AnimatedModel m_model;

    glm::mat4 m_toWorld;

    ChickenAnim();
    ~ChickenAnim() {}

    void Draw() const override;
    void Update(float deltaTime) override;
    void Scale(float s);

    void Walk();
    void Attack();
    void Jump();
    void Dance();
};

