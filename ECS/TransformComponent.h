#pragma once

#include "Components.h"
#include "../Vector2D.h"
#include "../Constant.h"

class TransformComponent : public Component
{
public:
    Vector2D position;
    Vector2D velocity;

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
        position.x = x + SCREEN_WIDTH / 2 - 16;
        position.y = y + SCREEN_HEIGHT / 2 - 28;
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
        velocity.Zero();
    }

    void update() override
    {
        int speedDiag = (int)(0.70710678118*speed);
        if (velocity.x != 0 && velocity.y != 0)
        {
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }
        else
        {
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }
    }

    void setPosition(int x, int y)
    {
        position.x = x;
        position.y = y;
    }
};