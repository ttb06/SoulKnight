#pragma once

#include "ECS.h"
#include "../TextureManager.h"
#include <SDL.h>
#include <SDL_ttf.h>

// heads-up display

class HUDComponent : public Component
{
private:
    SDL_Texture *hpTex;
    std::string hpPath;
    SpriteComponent *sprite;
    Vector2D pos;
    bool isDead = false;

public:
    bool isImmortal = false;
    int maxHealth;
    int curHealth;
    int maxArmor;
    int curArmor;
    int attackDamage = 0;

    Uint32 lastDameTakenTime = 0;

    HUDComponent(){};

    // for enermies
    HUDComponent(int mh)
    {
        maxArmor = 0;
        curArmor = 0;
        maxHealth = mh;
        curHealth = mh;
    }

    // for main
    HUDComponent(int mh, int a)
    {
        pos = Vector2D(0, 0);
        maxHealth = mh;
        curHealth = mh;
        maxArmor = a;
        curArmor = a;
        sprite = &entity->getComponent<SpriteComponent>();
    };

    HUDComponent(int mh, int a, Vector2D _pos, int dam)
    {
        attackDamage = dam;
        pos = _pos;
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
        // if (!entity->hasComponent<UILabel>())
        // {
        //     entity->addComponent<UILabel>();
        // }
    }

    void update() override
    {
        Uint32 time = SDL_GetTicks();
        if (time - lastDameTakenTime > IMMORTAL_TIME)
        {
            isImmortal = false;
        }
        else
        {
            isImmortal = true;
        }
        if (curHealth <= 0)
            isDead = true;
    }

    void draw() override
    {
        SDL_Color curHealthColor = {222, 58, 57, 255}, maxHealthColor = {106, 80, 55, 255};
        SDL_Color curArmorColor = {141, 141, 141, 255}, maxArmorColor = {106, 80, 55, 255};
        SDL_Color HUDBar = {107, 79, 56, 255};

        // draw
        int sizeBar = 7;
        SDL_Color white = {255, 255, 255, 255};
        int xPos = pos.x, yPos = pos.y;
        // TextureManager::drawRect(5, 5, std::max(maxHealth, maxArmor) * 32 + 5, 70, HUDBar);
        TextureManager::drawRect(xPos + 10, yPos + 10, 32 * sizeBar, 32, maxHealthColor);
        TextureManager::drawRect(xPos + 10, yPos + 10, 32 * sizeBar * curHealth / maxHealth, 32, curHealthColor);
        TextureManager::drawRect(xPos + 10, yPos + 100, 32 * sizeBar, 32, maxArmorColor);
        TextureManager::drawRect(xPos + 10, yPos + 100, 32 * sizeBar * curArmor / maxArmor, 32, curArmorColor);
    }

    void getDamage(int dam)
    {
        if (!isImmortal)
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
                
            Uint32 time = SDL_GetTicks();
            lastDameTakenTime = time;
        }
    }
};