#pragma once

#include "ECS.h"
#include "../Game.h"
#include "Components.h"
#include <math.h>

extern Entity &player;

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
            case SDLK_UP:
                transform->velocity.y = -1;
                sprite->Play("Run");
                break;
            case SDLK_a:
            case SDLK_LEFT:
                transform->velocity.x = -1;
                sprite->Play("Run");
                sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                break;
            case SDLK_s:
            case SDLK_DOWN:
                transform->velocity.y = 1;
                sprite->Play("Run");
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                transform->velocity.x = 1;
                sprite->Play("Run");
                sprite->spriteFlip = SDL_FLIP_NONE;
                break;
            }

            DirectionComponent dir;
            dir = player.getComponent<DirectionComponent>();

            if (dir.vec.len() < 16 * Game::total_scale * 5 &&dir.vec.len() > 0)
            {
                sprite->spriteFlip = dir.flip;
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
            case SDLK_UP:
                transform->velocity.y = 0;
                // sprite->Play("Idle");
                break;
            case SDLK_a:
            case SDLK_LEFT:
                transform->velocity.x = 0;
                // sprite->Play("Idle");
                // sprite->spriteFlip = SDL_FLIP_NONE;
                break;
            case SDLK_s:
            case SDLK_DOWN:
                transform->velocity.y = 0;
                // sprite->Play("Idle");
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                transform->velocity.x = 0;
                // sprite->Play("Idle");
                break;
            }
        }
        if (transform->velocity.x == 0 && transform->velocity.y == 0)
            sprite->Play("Idle");
        // std::cout <<"[KeyboardControler.h]: vel: " << transform->velocity << std::endl;
    }
};