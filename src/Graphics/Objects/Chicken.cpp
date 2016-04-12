//
//  Chicken.cpp
//  egg scramble
//
//  Created by Phoebe on 4/11/16.
//  Copyright © 2016 sunny side up. All rights reserved.
//

#include "Chicken.h"

Chicken::Chicken() {
    model = new Model("assets/chickens/objects/chicken.obj");
    shader.SetShaders("src/Graphics/Shaders/basic_shader.vert", "src/Graphics/Shaders/basic_shader.frag");
}

Chicken::~Chicken() {
    delete(model);
}

void Chicken::Draw(glm::mat4 C) {
    shader.Use();
    model->Draw(shader);
}