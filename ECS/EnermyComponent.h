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
    int room = -1;
    bool canShootProjectiles = false;
    Uint32 countDown = 2000;
    Uint32 lastShootPrj = 0;
    std::string prjtex;
    bool isActive = false;

    EnermyComponent(int mH, int cD, std::string id, int r)
    {
        maxHealth = mH;
        curHealth = mH;
        collisionDamage = cD;
        prjtex = id;
        room = r;
    }

    // EnermyComponent(int mH, int cD = 0, bool sP, int countDown)
    // {
    //     maxHealth = mH;
    //     curHealth = mH;
    //     collisionDamage = cD;
    //     canShootProjectiles = sP;
    // }

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
        // transform = &entity->getComponent<TransformComponent>();
        // sprite = &entity->getComponent<SpriteComponent>();
    }

    void takeDamage(int dame)
    {
        curHealth -= dame;
        if (curHealth <= 0)
        {
            entity->destroy();
            Game::roomEnermies[room]--;
            Game::assets->AddSkull(entity->getComponent<TransformComponent>().position.x, entity->getComponent<TransformComponent>().position.y, "skull");
        }
    }

    void shoot()
    {
        if (wayToPlayer.len() > 0)
        {
            Vector2D wtp(player.getComponent<TransformComponent>().position.x - entity->getComponent<TransformComponent>().position.x, player.getComponent<TransformComponent>().position.y - entity->getComponent<TransformComponent>().position.y);
            Vector2D pos = entity->getComponent<TransformComponent>().position;
            pos.x += entity->getComponent<TransformComponent>().width / 2;
            pos.y += entity->getComponent<TransformComponent>().height / 2;
            Game::assets->CreatProjectile(pos, wtp, (int)1000, BULLET_SPEED, prjtex);
        }
    }

    void update() override
    {
        if (!isActive) return;
        wayToPlayer.x = player.getComponent<TransformComponent>().position.x - entity->getComponent<TransformComponent>().position.x;
        wayToPlayer.y = player.getComponent<TransformComponent>().position.y - entity->getComponent<TransformComponent>().position.y;

        if (wayToPlayer.len() > MAX_DISTANCE_FROM_ENERMY_TILE * 16 * Game::total_scale) // distance to detect player
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
        // shoot prj tile
        if (SDL_GetTicks() - lastShootPrj > countDown)
        {
            lastShootPrj = SDL_GetTicks();
            shoot();
        }
    }
    void deactive()
    {
        entity->getComponent<TransformComponent>().velocity.Zero();
        isActive = false;
    }
    void active()
    {
        isActive = true;
    }
};