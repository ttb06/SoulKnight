#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Game.h"
#include "../Vector2D.h"
#include "math.h"

extern Entity &player;
extern Entity &weapon;
// // extern Manager manager;

extern std::vector<Entity *> &enermies;
// auto &enermies(manager.getGroup(Game::groupEnermies));

class DirectionComponent : public Component
{
private:
public:
    bool isEnermyAhead = true;
    double angle;
    Vector2D vec;
    SDL_RendererFlip flip;

    DirectionComponent() {}

    ~DirectionComponent() {}

    void init() override
    {
        vec = Vector2D(1e9, 1e9);
        angle = 0;
        flip = SDL_FLIP_NONE;
    }

    void update() override
    {
        Vector2D curVec;
        vec = Vector2D(1e9, 1e9);
        for (auto &e : enermies)
        {
            curVec.x = e->getComponent<TransformComponent>().position.x + e->getComponent<TransformComponent>().width / 2 - player.getComponent<TransformComponent>().position.x;
            curVec.y = e->getComponent<TransformComponent>().position.y + e->getComponent<TransformComponent>().height / 2 - player.getComponent<TransformComponent>().position.y;
            if (vec.len() > curVec.len())
            {
                vec = curVec;
                angle = vec.getAngleDegrees();
                if (vec.x >= 0)
                    flip = SDL_FLIP_NONE;
                else
                    flip = SDL_FLIP_HORIZONTAL;

                if (flip == SDL_FLIP_HORIZONTAL)
                {
                    angle = 0 - angle;
                }
            }
        }

        if (vec.len() > MAX_DISTANCE_FROM_ENERMY_TILE * Game::total_scale * 5)
        {
            vec = player.getComponent<TransformComponent>().velocity;
            isEnermyAhead = false;
            if (vec.len() > 0)
            {
                angle = vec.getAngleDegrees();
                if (vec.x >= 0)
                    flip = SDL_FLIP_NONE;
                else
                    flip = SDL_FLIP_HORIZONTAL;

                if (flip == SDL_FLIP_HORIZONTAL)
                {
                    angle = 0 - angle;
                }
            }
        }
    }

    void draw() override
    {
    }
};