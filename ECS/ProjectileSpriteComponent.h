#pragma once

#include "Components.h"
#include "../Game.h"
#include "../AssetManager.h"

class ProjectileSpriteComponent : public Component
{
public:
    SDL_Texture *texture;
    double angle = 0;
    TransformComponent *transform;
    SDL_Rect srcRect, destRect;
    bool isDestroying = false;
    Uint32 destroyTime = 0;
    bool updated = false;

    int frames = 0;
    int speed = 50;
    int animIndex = 0;
    std::map<const char *, Animation> animations;

    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
    ProjectileSpriteComponent(std::string id)
    {

        Animation flying = Animation(0, PROJECTILES_FLYING_ANIM_FRAMES, PROJECTILES_TIME_FLYING_FRAME);
        Animation destroy = Animation(1, PROJECTILES_DESTROY_ANIM_FRAMES, PROJECTILES_TIME_DESTROY_FRAME);

        animations.emplace("Flying", flying);
        animations.emplace("Destroy", destroy);

        Play("Flying");
        setTex(id);
    }
    ~ProjectileSpriteComponent() {}

    void setTex(std::string id)
    {
        texture = Game::assets->GetTexture(id);
    }

    void Play(const char *animName)
    {
        frames = animations[animName].frames;
        speed = animations[animName].speed;
        animIndex = animations[animName].index;
    }

    void init() override
    {
        angle = entity->getComponent<TransformComponent>().velocity.getAngleDegrees();
        transform = &entity->getComponent<TransformComponent>();

        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = 16;
        srcRect.h = 16;
    }

    void update() override
    {
        srcRect.x = srcRect.w * (int)((SDL_GetTicks() / speed) % frames);
        srcRect.y = animIndex * 16;

        destRect.x = (int)transform->position.x - Game::camera.x;
        destRect.y = (int)transform->position.y - Game::camera.y;
        destRect.w = srcRect.w * transform->scale;
        destRect.h = srcRect.h * transform->scale;

        if (transform->velocity.x > 0)
        {
            spriteFlip = SDL_FLIP_NONE;
        }
        else if (transform->velocity.x < 0 && updated == false)
        {
            angle = 180 - angle;
            updated = true;
            spriteFlip = SDL_FLIP_VERTICAL;
        }

        if (animIndex == 1)
        {
            srcRect.x = srcRect.w * (int)(((SDL_GetTicks() - destroyTime) / speed) % frames);
        }

        if (isDestroying)
        {
            transform->velocity.Zero();
            if (destroyTime == 0)
            {
                destroyTime = SDL_GetTicks();
            }
            Play("Destroy");
            if (SDL_GetTicks() - destroyTime >= (PROJECTILES_DESTROY_ANIM_FRAMES - 1) * (PROJECTILES_TIME_DESTROY_FRAME - 1))
            {
                entity->destroy();
            }
        }
    }

    void destroy()
    {
        isDestroying = true;
    }
    void draw() override
    {
        SDL_Point center;
        center.x = destRect.w / 2;
        center.y = destRect.h / 2;
        TextureManager::DrawAngle(texture, srcRect, destRect, spriteFlip, center, angle);
    }
};