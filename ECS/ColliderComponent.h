#pragma once

#include <string>
#include <SDL.h>
#include "Components.h"
#include "../TextureManager.h"

class ColliderComponent : public Component
{
public:
    SDL_Rect collider, initCollider;
    std::string tag;
    bool initedCollider = false;
    int size;

    SDL_Texture *tex;
    SDL_Rect srcR, destR;

    TransformComponent *transform;

    ColliderComponent(std::string t)
    {
        tag = t;
        initCollider.x = initCollider.y = 0;
    }

    ColliderComponent(std::string t, int xPos, int yPos, int s, bool isPlayer = false)
    {
        tag = t;
        if (!isPlayer)
        {
            collider.x = xPos;
            collider.y = yPos;
            collider.w = collider.h = s;
        }
        else
        {
            initCollider.x = xPos;
            initCollider.y = yPos;
            size = s;
            initCollider.w = initCollider.h = s;
            initedCollider = true;
        }
    }
    ColliderComponent(std::string t, int xPos, int yPos, int w, int h, bool isPlayer = false)
    {
        tag = t;
        collider.x = xPos;
        collider.y = yPos;
        collider.w = w;
        collider.h = h;
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
        if (tag != "terrain" && tag != "room")
        {
            if (!initedCollider)
            {
                collider.x = (int)transform->position.x;
                collider.y = (int)transform->position.y;
                collider.w = transform->width * transform->scale;
                collider.h = transform->height * transform->scale;
            }
            else
            {
                collider.x = (int)transform->position.x + initCollider.x * transform->scale;
                collider.y = (int)transform->position.y + initCollider.y * transform->scale;
                collider.w = size * transform->scale;
                collider.h = size * transform->scale;
            }
        }

        destR.x = collider.x - Game::camera.x;
        destR.y = collider.y - Game::camera.y;
    }

    void draw() override
    {
        if (tag == "terrain")
            TextureManager::Draw(tex, srcR, destR, SDL_FLIP_NONE);
        if (tag == "room")
        {
            std::cout << collider.x << "    " << collider.y << std::endl;
            SDL_RenderFillRect(Game::renderer, &collider);
        }
        if (tag == "player")
        {
            SDL_Rect destt = collider;
            destt.x -= Game::camera.x;
            destt.y -= Game::camera.y;
            // TextureManager::Draw(tex, srcR, destt, SDL_FLIP_NONE);
        }
    }
};