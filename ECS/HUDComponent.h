#pragma once

#include "ECS.h"
#include "../TextureManager.h"
#include <SDL.h>
#include <SDL_ttf.h>

// heads-up display

class HUDComponent : public Component
{
private:
    SDL_Texture *hpTex, *armorTex;

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
    Uint32 lastTimeRestoreArmor = 0;
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

    HUDComponent(int mh, int a, Vector2D _pos, int dam, std::string hp, std::string ar)
    {
        attackDamage = dam;
        pos = _pos;
        maxHealth = mh;
        curHealth = mh;
        maxArmor = a;
        curArmor = a;
        sprite = &entity->getComponent<SpriteComponent>();
        hpTex = Game::assets->GetTexture(hp);
        armorTex = Game::assets->GetTexture(ar);
    };

    ~HUDComponent(){};

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

        if (SDL_GetTicks() - lastDameTakenTime > TIME_RESTORE_ARMOR)
        {
            if (SDL_GetTicks() - lastTimeRestoreArmor > TIME_RESTORE_ARMOR)
            lastTimeRestoreArmor = SDL_GetTicks();
            {
                if (curArmor < maxArmor)
                    curArmor += 1;
            }
        }
    }

    void draw() override
    {
        int maxHeartHp = (maxHealth + 1) / 2;
        double curHeartHp = 1.0 * (curHealth) / 2;
        int curX = pos.x, curY = pos.y;
        for (int i = 0; i < maxHeartHp; i++)
        {
            curX = i * 20 * Game::total_scale + pos.x;
            SDL_Rect src = {0, 0, 16, 16};
            SDL_Rect dest = {curX, curY, 16 * Game::total_scale, 16 * Game::total_scale};
            TextureManager::Draw(hpTex, src, dest, SDL_FLIP_NONE);
        }
        curX = pos.x - 20 * Game::total_scale;
        for (int i = 0; i < (int)curHeartHp; i++)
        {
            curX = i * 20 * Game::total_scale + pos.x;
            SDL_Rect src = {32, 0, 16, 16};
            SDL_Rect dest = {curX, curY, 16 * Game::total_scale, 16 * Game::total_scale};
            TextureManager::Draw(hpTex, src, dest, SDL_FLIP_NONE);
        }
        if (curHeartHp - (int)curHeartHp == 0.5)
        {
            curX += 20 * Game::total_scale;
            SDL_Rect src = {16, 0, 16, 16};
            SDL_Rect dest = {curX, curY, 16 * Game::total_scale, 16 * Game::total_scale};
            TextureManager::Draw(hpTex, src, dest, SDL_FLIP_NONE);
        }

        int maxHeartArmor = (maxArmor + 1) / 2;
        double curHeartArmor = 1.0 * (curArmor) / 2;
        curX = pos.x, curY = pos.y + 20 * Game::total_scale;
        for (int i = 0; i < maxHeartArmor; i++)
        {
            curX = i * 20 * Game::total_scale + pos.x;
            SDL_Rect src = {0, 0, 16, 16};
            SDL_Rect dest = {curX, curY, 16 * Game::total_scale, 16 * Game::total_scale};
            TextureManager::Draw(armorTex, src, dest, SDL_FLIP_NONE);
        }
        curX = pos.x - 20 * Game::total_scale;
        for (int i = 0; i < (int)curHeartArmor; i++)
        {
            curX = i * 20 * Game::total_scale + pos.x;
            SDL_Rect src = {32, 0, 16, 16};
            SDL_Rect dest = {curX, curY, 16 * Game::total_scale, 16 * Game::total_scale};
            TextureManager::Draw(armorTex, src, dest, SDL_FLIP_NONE);
        }
        if (curHeartArmor - (int)curHeartArmor == 0.5 && curArmor >= 0)
        {
            curX += 20 * Game::total_scale;
            SDL_Rect src = {16, 0, 16, 16};
            SDL_Rect dest = {curX, curY, 16 * Game::total_scale, 16 * Game::total_scale};
            TextureManager::Draw(armorTex, src, dest, SDL_FLIP_NONE);
        }
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