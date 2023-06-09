#pragma once

#include "Components.h"
#include <SDL2/SDL.h>

class UpdateSpriteComponent : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;

    UpdateSpriteComponent()
    {
        // transform = &entity->getComponent<TransformComponent>();
        // sprite = &entity->getComponent<SpriteComponent>();
    }

    ~UpdateSpriteComponent() {}

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {
        // run
        if (entity->hasComponent<HUDComponent>())
        {
            if (entity->getComponent<HUDComponent>().curHealth <= 0)
            {
                sprite->Play("Dead");
                entity->getComponent<TransformComponent>().velocity.Zero();
            }
            else if (entity->getComponent<HUDComponent>().isImmortal == true)
            {
                sprite->Play("Hit");
            }
            else if (transform->velocity.len() > 0)
            {
                sprite->Play("Run");
            }
            else
            {
                if (entity->getComponent<MouseController>().path.empty() || !entity->getComponent<TransformComponent>().autoMove)
                    sprite->Play("Idle");
            }
        }
        else
        {
            if (transform->velocity.len() > 0)
            {
                sprite->Play("Run");
            }
            else
            {
                sprite->Play("Idle");
            }
        }

        // flip
        if (entity->hasComponent<DirectionComponent>() && entity->getComponent<DirectionComponent>().isEnermyAhead)
        {
            if (entity->getComponent<DirectionComponent>().vec.x > 0)
                sprite->spriteFlip = SDL_FLIP_NONE;
            else
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
        }
        else if (transform->velocity.x != 0)
        {
            if (transform->velocity.x > 0)
                sprite->spriteFlip = SDL_FLIP_NONE;
            else
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
        }
    }
};