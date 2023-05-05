#pragma once

#include "Components.h"
#include "../Vector2D.h"
#include "../Constant.h"
#include "../Game.h"
class TransformComponent : public Component
{
public:
    Vector2D position;
    Vector2D velocity;

    bool autoMove = false;

    int width = 16;
    int height = 16;
    int scale = 2;

    int speed = 5;

    TransformComponent()
    {
        position.Zero();
    }

    TransformComponent(int x, int y)
    {
        setPosition(x, y);
    }

    TransformComponent(int sc)
    {
        position.x = SCREEN_WIDTH / 2 - 16;
        position.y = SCREEN_HEIGHT / 2 - 28;
        scale = sc;
    }

    TransformComponent(int w, int h, int sc)
    {
        height = h;
        width = w;
        position.x = SCREEN_WIDTH / 2 - 16;
        position.y = SCREEN_HEIGHT / 2 - 28;
        scale = sc;
    }

    TransformComponent(int x, int y, int w, int h, int scl)
    {
        position.x = x + SCREEN_WIDTH / 2 - w;
        position.y = y + SCREEN_HEIGHT / 2 - h;
        height = h;
        width = w;
        scale = scl;
    }

    TransformComponent(int x, int y, int w, int h, int scl, int spd)
    {
        setPosition(x, y);
        height = h;
        width = w;
        scale = scl;
        speed = spd;
    }

    void init() override
    {
    }

    void update() override
    {
        position.x += velocity.x * speed;
        position.y += velocity.y * speed;
    }

    void setPosition(int x, int y)
    {
        position.x = x;
        position.y = y;
    }
    //move collider
    void moveTo(int tx, int ty)
    {
        if (autoMove == true)
        {
            Vector2D destPos(tx, ty);
            Vector2D distance;
            distance.x = position.x - destPos.x;
            distance.y = position.y + 12 * Game::total_scale - destPos.y;
            std::cout << "[transform] current distance: " << distance.len() << "  " << distance.x << " " << distance.y << std::endl;
            if (distance.len() < speed)
            {
                position.x = destPos.x;
                position.y = destPos.y - 12 * Game::total_scale;

                velocity.Zero();
                autoMove = false;
            }
            else
            {
                velocity.x = (double)tx - position.x;
                velocity.y = (double)ty - (position.y + 12 * Game::total_scale);
                velocity.normalize();
            }
        }
    }

    //move collider
    void moveToGrid(int tx, int ty)
    {
        moveTo(tx * 16 * Game::total_scale, ty * 16 * Game::total_scale);
    }

    // return position with the grid of map for bfs
    int getPosGridX()
    {
        return (int)position.x / (16 * Game::total_scale);
    }

    int getPosGridY()
    {
        return (int)(position.y + 12 * Game::total_scale) / (16 * Game::total_scale);
    }
};