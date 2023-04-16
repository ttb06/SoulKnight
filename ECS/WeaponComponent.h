#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Game.h"

#include <SDL.h>

extern Entity& player;

class WeaponComponent : public Component
{
private:
    TransformComponent transform;
    SDL_Texture *tex;
    ColliderComponent *range;
    SDL_Rect src, dest;
    int scale;

public:
    WeaponComponent(std::string id, int width, int height, int scl)
    {
        scale = scl;
        SDL_Texture *tex = Game::assets->GetTexture(id);
        src = {0, 0, width, height};
        // range->collider.x = transform->position.x + 5;
        // range->collider.y = transform->position.y + 5;
    }

    ~WeaponComponent(){};

    void init() override
    {
        transform = player.getComponent<TransformComponent>();
    }

    void update() override
    {
        dest = {(int)transform.position.x - Game::camera.x, (int)transform.position.y - Game::camera.y, src.w * scale, src.h * scale};
    }

    void draw() override
    {
        // src = {0, 0, 12, 30};
        // TextureManager::Draw(tex, src, dest, SDL_FLIP_NONE);
        std::cout << "[WeaponComponent.h]: weapon position: " << dest.x << " " << dest.y << std::endl;
    }
};