#pragma once

#include <string>
#include <SDL.h>
#include "Components.h"
#include "../TextureManager.h"

class ColliderComponent : public Component
{
public:
    SDL_Rect collider;
    std::string tag;

    SDL_Texture *tex;
    SDL_Rect srcR, destR;

    TransformComponent *transform;

    ColliderComponent(std::string t)
    {
        tag = t;
    }

    ColliderComponent(std::string t, int xPos, int yPos, int size)
    {
        tag = t;
        
        collider.x = xPos;
        collider.y = yPos;
        collider.w = collider.h = size;
    }

    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
        // if (tag == "player")
        //     std::cout << tag << std::endl;
        transform = &entity->getComponent<TransformComponent>();
        tex = TextureManager::loadTexture("assets/ColTex.png");
        srcR = {0, 0, 16, 16};
        destR = {collider.x, collider.y, collider.w, collider.h};
    }

    void update() override
    {
        if (tag != "terrain")
        {
            collider.x = (int)transform->position.x;
            collider.y = (int)transform->position.y;
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
        }

        destR.x = collider.x - Game::camera.x;
        destR.y = collider.y - Game::camera.y;
    }

    void draw() override
    {
        TextureManager::Draw(tex, srcR, destR, SDL_FLIP_NONE);
    }
};