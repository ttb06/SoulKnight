#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Game.h"

#include <SDL.h>

extern Entity &player;

class WeaponComponent : public Component
{
private:
    TransformComponent transform;
    DirectionComponent dir;
    SDL_Texture *tex;
    ColliderComponent *range;
    SDL_Rect src, dest;
    int scale;

public:
    WeaponComponent(std::string id, int width, int height, int scl)
    {
        scale = scl;
        tex = Game::assets->GetTexture(id);

        src = {0, 0, width, height};
        // range->collider.x = transform->position.x + 5;
        // range->collider.y = transform->position.y + 5;
    }

    ~WeaponComponent(){};

    void init() override
    {
    }

    void update() override
    {
    }

    void draw() override
    {
        transform = player.getComponent<TransformComponent>();
        dir = player.getComponent<DirectionComponent>();
        double angle = dir.angle;
        SDL_Point center = {0, src.h/2};
        dest = {(int)transform.position.x - Game::camera.x, (int)transform.position.y - Game::camera.y, src.w * scale, src.h * scale};
        
        if (dir.flip == SDL_FLIP_HORIZONTAL)
        {
            center = {src.w, src.h/2};
            dest.x -= src.w;
            dest.y -= src.h;
        }

        // double angle = player.getComponent<DirectionComponent>().angle;
        // src = {0, 0, 12, 30};
        TextureManager::DrawAngle(tex, src, dest, dir.flip, center, angle);
        // std::cout << "[WeaponComponent.h]: Weapon position: " << dest.x << " " << dest.y << std::endl;
    }
};