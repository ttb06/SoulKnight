#pragma once

#include <string>
#include <SDL.h>
#include "Components.h"
#include "../Game.h"

class ColliderComponent : public Component
{
public:
    SDL_Rect collider;
    std::string tag;

    TransformComponent *transform;

    ColliderComponent(std::string t)
    {
        tag = t;
    }

    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }

        transform = &entity->getComponent<TransformComponent>();
        
        Game::colliders.push_back(this);
    }

    void update() override
    {
        collider.x = (int)transform->position.x;
        collider.y = (int)transform->position.y;
        collider.w = transform->width * transform->scale;
        collider.h = transform->height * transform->scale;
    }
};