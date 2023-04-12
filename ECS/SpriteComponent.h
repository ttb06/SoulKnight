#pragma once

#include <SDL.h>
#include "Components.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>

class SpriteComponent : public Component
{
private:
    TransformComponent *transform;
    SDL_Texture *texture;
    SDL_Rect srcRect, destRect;

    bool animated = false;
    int frames = 0;
    int speed = 100;

public:
    bool hasHitAnim = false;
    int animIndex = 0;
    
    std::map<const char *, Animation> animations;
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent() = default;

    SpriteComponent(const char *path)
    {
        setTex(path);
    }

    SpriteComponent(const char *path, bool isAnim, bool hit = false)
    {
        animated = isAnim;

        Animation idle = Animation(0, 4, 150);
        Animation run = Animation(1, 4, 100);

        animations.emplace("Idle", idle);
        animations.emplace("Run", run);

        hasHitAnim = hit;
        if (hasHitAnim == true)
        {
            Animation hit = Animation(2, 2, 100);
            animations.emplace("Hit", hit);
        }

        Play("Idle");
        setTex(path);
    }

    ~SpriteComponent()
    {
        SDL_DestroyTexture(texture);
    }

    void setTex(const char *path)
    {
        texture = TextureManager::loadTexture(path);
    }

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = transform->width;
        srcRect.h = transform->height;
    }

    void update() override
    {
        if (animated)
        {
            srcRect.x = srcRect.w * (int)((SDL_GetTicks() / speed) % frames);
        }

        srcRect.y = animIndex * transform->height;

        destRect.x = (int)transform->position.x - Game::camera.x;
        destRect.y = (int)transform->position.y - Game::camera.y;
        destRect.w = srcRect.w * transform->scale;
        destRect.h = srcRect.h * transform->scale;
    }

    void draw() override
    {
        TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
    }

    void Play(const char *animName)
    {
        frames = animations[animName].frames;
        speed = animations[animName].speed;
        animIndex = animations[animName].index;
    }
};