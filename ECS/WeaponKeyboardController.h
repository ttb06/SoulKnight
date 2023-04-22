#pragma once

#include "Components.h"

class WeaponKeyboardController : public Component
{
private:
public:
    WeaponComponent *weapon;
    WeaponSpriteComponent *sprite;

    WeaponKeyboardController(){};
    ~WeaponKeyboardController(){};

    void init() override
    {
        weapon = &entity->getComponent<WeaponComponent>();
        sprite = &entity->getComponent<WeaponSpriteComponent>();
    }

    void update() override
    {
        std::cout << weapon->attackCounter << std::endl;
        if (Game::event.type == SDL_KEYDOWN)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_SPACE:
            {
                if (weapon->isAtacking == false)
                {
                    weapon->lastTime_NormalAttack = SDL_GetTicks();
                    weapon->isAtacking = true;
                    sprite->Play("Hit");
                    weapon->attackCounter ++;
                }
            }
            break;
            }
        }

        if (Game::event.type == SDL_KEYUP)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_SPACE:
            {
                sprite->Play("Idle");
            }
            break;
            }
        }
    }
};