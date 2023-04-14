#pragma once

#include "ECS.h"
#include "../Game.h"
#include "Components.h"
#include <math.h>

class KeyboardController : public Component
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
        if (Game::event.type == SDL_KEYDOWN)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_w:
                transform->velocity.y = -1;
                sprite->Play("Run");
                break;
            case SDLK_a:
                transform->velocity.x = -1;
                sprite->Play("Run");
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                break;
            case SDLK_s:
                transform->velocity.y = 1;
                sprite->Play("Run");
                break;
            case SDLK_d:
                transform->velocity.x = 1;
                sprite->Play("Run");
                sprite->spriteFlip = SDL_FLIP_NONE;
                break;
            }

            if (abs(transform->velocity.x) == 1 && abs(transform->velocity.y) != 0)
            {
                transform->velocity.x = transform->velocity.x * sin(M_PI_4);
            }
            
            if (abs(transform->velocity.x) != 0 && abs(transform->velocity.y) == 1)
            {
                transform->velocity.y = transform->velocity.y * sin(M_PI_4);
            }
        }
        if (Game::event.type == SDL_KEYUP)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_w:
                transform->velocity.y = 0;
                sprite->Play("Idle");
                break;
            case SDLK_a:
                transform->velocity.x = 0;
                sprite->Play("Idle");
                // sprite->spriteFlip = SDL_FLIP_NONE;
                break;
            case SDLK_s:
                transform->velocity.y = 0;
                sprite->Play("Idle");
                break;
            case SDLK_d:
                transform->velocity.x = 0;
                sprite->Play("Idle");
                break;
            }
        }
        std::cout << transform->velocity << std::endl;
    }
};