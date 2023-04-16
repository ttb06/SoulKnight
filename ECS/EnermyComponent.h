#pragma once

#include "Components.h"
#include "../Game.h"
#include "math.h"

extern Entity &player;

class EnermyComponent : public Component
{
public:
    int maxHealth, curHealth;
    int collisionDamage = 0;
    Vector2D wayToPlayer;

    EnermyComponent(int mH, int cD = 0)
    {
        maxHealth = mH;
        curHealth = mH;
        collisionDamage = cD;
    }

    ~EnermyComponent(){};

    void init() override
    {
        if (!entity->hasComponent<TransformComponent>())
        {
            entity->addComponent<TransformComponent>();
        }
    }

    void update() override
    {
        wayToPlayer.x = player.getComponent<TransformComponent>().position.x - entity->getComponent<TransformComponent>().position.x;
        wayToPlayer.y = player.getComponent<TransformComponent>().position.y - entity->getComponent<TransformComponent>().position.y;

        std::cout<< "[EnermyComponent.h]: distance to player: " << wayToPlayer.len() << std::endl;

        if (wayToPlayer.len() > 500)
        {
            entity->getComponent<TransformComponent>().velocity.Zero();
        }
        else
        {
            double sinA = wayToPlayer.x / wayToPlayer.len();
            double cosA = wayToPlayer.y / wayToPlayer.len();

            entity->getComponent<TransformComponent>().velocity.x = sinA;
            entity->getComponent<TransformComponent>().velocity.y = cosA;
        }

        // if (entity->getComponent<TransformComponent>().velocity.x != 0 ||
        //     entity->getComponent<TransformComponent>().velocity.y != 0)
        // {
        //     entity->getComponent<SpriteComponent>().Play("Run");
        // }
        // else
        // {
        //     entity->getComponent<SpriteComponent>().Play("Idle");
        // }
    }
};