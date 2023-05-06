#pragma once

#include <SDL.h>
#include "Components.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include "../AssetManager.h"

extern Entity &player;

class WeaponSpriteComponent : public Component
{
private:
    WeaponComponent weapon;
    SDL_Texture *texture;
    SDL_Rect weaponRect;
    Uint32 startSlashTime = 0;

    int scale = 1;
    int frames = 0;
    int speed = 100;

public:
    int animIndex = 0;
    SDL_Rect srcRect, destRect;

    std::map<const char *, Animation> animations;
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    WeaponSpriteComponent() = default;

    WeaponSpriteComponent(std::string id, int width, int height, int scl)
    {
        weaponRect = {0, 60 - height, width, height};

        Animation idle = Animation(0, 1, 100);
        Animation hit = Animation(1, 6, 50);

        animations.emplace("Idle", idle);
        animations.emplace("Hit", hit);

        Play("Idle");
        setTex(id);
    }

    ~WeaponSpriteComponent()
    {
    }

    void setTex(std::string id)
    {
        texture = Game::assets->GetTexture(id);
    }

    void init() override
    {
        // srcRect = weaponRect;
    }

    void update() override
    {
        weapon = entity->getComponent<WeaponComponent>();
        startSlashTime = weapon.lastTime_NormalAttack;

        if ((SDL_GetTicks() - startSlashTime) / speed > 5)
        {
            Play("Idle");
            entity->getComponent<WeaponComponent>().isAtacking = false;
        }

        if (entity->getComponent<WeaponComponent>().isAtacking == true)
        {
            Play("Hit");
        }

        srcRect.x = srcRect.w * (int)(((SDL_GetTicks() - startSlashTime) / speed) % frames);

        srcRect.y = 0;
        srcRect.w = weaponRect.w;
        srcRect.h = weaponRect.h;

        // destRect.x = weapon.dest.x;
        // destRect.y = weapon.dest.y;

        if (animIndex == 0)
        {
            srcRect = weaponRect;
            srcRect.x = 0;
            srcRect.y = 30 - weaponRect.h / 2;

            destRect = weapon.dest;

            // if (player.getComponent<DirectionComponent>().isEnermyAhead == true)
            {
                if (weapon.dir.flip == SDL_FLIP_NONE)
                    weapon.angle -= 15;
                else
                    weapon.angle += 15;
            }
        }

        if (animIndex == 1)
        {
            srcRect.x = srcRect.w * (int)(((SDL_GetTicks() - startSlashTime) / speed) % frames);
            srcRect.y = 60;
            srcRect.w = weaponRect.w;
            srcRect.h = 60;

            destRect.x = weapon.dest.x;
            destRect.y = weapon.dest.y;
            weapon.center.y = 90;
            destRect.w = 29 * Game::total_scale;
            destRect.h = 60 * Game::total_scale;

            destRect.y -= 90;
        }

        if (weapon.attackCounter % 2 == 0)
            if (weapon.dir.flip == SDL_FLIP_HORIZONTAL)
            {
                weapon.dir.flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
            }
            else
                weapon.dir.flip = SDL_FLIP_VERTICAL;

        // if (player.getComponent<DirectionComponent>().vec.len() > 16 * Game::total_scale * 5)
        // {
        //     weapon.dir.flip = player.getComponent<SpriteComponent>().spriteFlip;
        //     if (player.getComponent<TransformComponent>().velocity.len() != 0)
        //     {
        //         weapon.angle = player.getComponent<TransformComponent>().velocity.getAngleDegrees();
        //         std::cout << weapon.angle << std::endl;
        //     }
        // }
    }

    void draw() override
    {
        TextureManager::DrawAngle(texture, srcRect, destRect, weapon.dir.flip, weapon.center, weapon.angle);
    }

    void Play(const char *animName)
    {
        frames = animations[animName].frames;
        speed = animations[animName].speed;
        animIndex = animations[animName].index;
    }
};