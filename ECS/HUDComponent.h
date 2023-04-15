#pragma once

#include "ECS.h"
#include "../TextureManager.h"
#include <SDL.h>

// heads-up display

class HUDComponent : public Component
{
private:
    SDL_Texture *hpTex;
    std::string hpPath;
    SpriteComponent *sprite;

public:
    int maxHealth;
    int curHealth;
    int maxArmor;
    int curArmor;
    Uint32 lastDameTakenTime = 0;

    HUDComponent(){};

    HUDComponent(int mh, int a)
    {
        maxHealth = mh;
        curHealth = mh;
        maxArmor = a;
        curArmor = a;
        sprite = &entity->getComponent<SpriteComponent>();
    };

    ~HUDComponent()
    {
        SDL_DestroyTexture(hpTex);
    };

    void init() override
    {
        // hpTex = TextureManager::loadTexture(hpPath.c_str);
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {
    }

    void draw() override
    {
        SDL_Color curHealthColor = {222, 58, 57, 255}, maxHealthColor = {106, 80, 55, 255};
        SDL_Color curArmorColor = {141, 141, 141, 255}, maxArmorColor = {106, 80, 55, 255};
        SDL_Color HUDBar = {107, 79, 56, 255};

        int sizeBar = 7;
        // TextureManager::drawRect(5, 5, std::max(maxHealth, maxArmor) * 32 + 5, 70, HUDBar);
        TextureManager::drawRect(10, 10, 32 * sizeBar, 32, maxHealthColor);
        TextureManager::drawRect(10, 10, 32 * sizeBar * curHealth / maxHealth, 32, curHealthColor);
        TextureManager::drawRect(10, 100, 32 * sizeBar, 32, maxArmorColor);
        TextureManager::drawRect(10, 100, 32 * sizeBar * curArmor / maxArmor, 32, curArmorColor);
    }

    void getDamage(int dam)
    {

        Uint32 time = SDL_GetTicks();
        if (time - lastDameTakenTime > 1000)
        {
            if (curArmor > 0)
            {
                curArmor -= dam;
                curArmor = std::max(curArmor, 0);
            }
            else
            {
                curHealth -= dam;
                curHealth = std::max(curHealth, 0);
            }
            if (sprite->hasHitAnim)
                sprite->Play("Hit");
            lastDameTakenTime = time;
        }
    }
};