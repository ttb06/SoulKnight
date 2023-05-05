#pragma once

#include "Components.h"
#include "ECS.h"
#include "../Game.h"
#include "math.h"

extern Entity &player;

class EnermyComponent : public Component
{
public:
    int maxHealth, curHealth;
    int collisionDamage = 0;
    Vector2D wayToPlayer;
    int lastTakenDamage = -1;
    SpriteComponent *sprite;

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
        if (!entity->hasComponent<SpriteComponent>())
        {
            entity->addComponent<SpriteComponent>();
        }
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void takeDamage(int dame)
    {
        curHealth -= dame;
        if (curHealth <= 0)
        {
            entity->destroy();
            Game::assets->AddSkull(entity->getComponent<TransformComponent>().position.x, entity->getComponent<TransformComponent>().position.y, "skull");
        }
    }

    void update() override
    {
        wayToPlayer.x = player.getComponent<TransformComponent>().position.x - entity->getComponent<TransformComponent>().position.x;
        wayToPlayer.y = player.getComponent<TransformComponent>().position.y - entity->getComponent<TransformComponent>().position.y;

        if (wayToPlayer.len() > MAX_DISTANCE_FROM_ENERMY_TILE * 16 * Game::total_scale)// distance to detect player
        {
            entity->getComponent<TransformComponent>().velocity.Zero();
        }
        else
        {
            double sinA = wayToPlayer.x / wayToPlayer.len();
            double cosA = wayToPlayer.y / wayToPlayer.len();
            // entity->getComponent<TransformComponent>().velocity = wayToPlayer.normalize();
            entity->getComponent<TransformComponent>().velocity.x = sinA;
            entity->getComponent<TransformComponent>().velocity.y = cosA;
        }
    }
};