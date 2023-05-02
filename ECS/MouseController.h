#pragma once

#include "ECS.h"
#include "../Game.h"
#include "Components.h"
#include <math.h>

extern Entity &player;

class MouseController : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {
        
    }
};