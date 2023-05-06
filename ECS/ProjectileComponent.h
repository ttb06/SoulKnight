#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"

class ProjectileComponent : public Component
{
public:
    ProjectileComponent(int rng, int sp, Vector2D vel, int dmg)
    {
        range = rng;
        speed = sp;
        velocity = vel;
        damage = dmg;
    }
    ~ProjectileComponent(){};

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        transform->velocity = velocity;
        std::cout << transform->position << std::endl;
    }

    void update() override
    {
        distance += speed;

        if (distance > range)
        {
            std::cout << "[ProjectileComponent.h]: Out of range" << std::endl;
            entity->destroy();
        }
    }
    int dame()
    {
        return damage;
    }
private:
    TransformComponent *transform;

    int range = 0;
    int speed = 0;
    int distance = 0;
    Vector2D velocity;
    Uint32 destroyTime;
    bool isDestroying = false;
    int damage = 0;
};