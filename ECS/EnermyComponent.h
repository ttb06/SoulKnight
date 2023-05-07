#pragma once

#include "Components.h"
#include "ECS.h"
#include "../Game.h"
#include "math.h"

extern Entity &player;
// extern AssetManager *Game::assets;
class EnermyComponent : public Component
{
public:
    TransformComponent *transform;
    int maxHealth, curHealth;
    int collisionDamage = 0;
    Vector2D wayToPlayer;
    int lastTakenDamage = -1;
    SpriteComponent *sprite;
    int room = -1;
    bool canShootProjectiles = false;
    Uint32 countDown = 2000;
    Uint32 lastShootPrj = 0;
    std::string prjtex;

    EnermyComponent(int mH, int cD = 0, std::string id = "")
    {
        maxHealth = mH;
        curHealth = mH;
        collisionDamage = cD;
        prjtex = id;
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
        transform = &entity->getComponent<TransformComponent>();
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

    void shoot()
    {
        if (wayToPlayer.len() > 0)
        {
            // Game::assets->CreatProjectile(transform.position, wayToPlayer, (int)1e9, BULLET_SPEED, prjtex);
            Vector2D wtp (player.getComponent<TransformComponent>().position.x - transform->position.x,player.getComponent<TransformComponent>().position.y - transform->position.y);
            Game::assets->CreatProjectile(transform->position, wtp, (int)1000, BULLET_SPEED, prjtex);
        }
    }

    void update() override
    {
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
};