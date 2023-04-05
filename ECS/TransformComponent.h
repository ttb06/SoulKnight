#pragma once

#include "Components.h"
#include "../Vector2D.h"

class TransformComponent : public Component
{
public:
    Vector2D position;
    Vector2D velocity;

    int width = 16;
    int height = 28;
    int scale = 2;

    int speed = 10;

    TransformComponent()
    {
        position.x = 0.0f;
        position.y = 0.0f;
    }

    TransformComponent(int x, int y)
    {
        setPosition(x, y);
    }

    TransformComponent(int sc)
    {
        setPosition(0.0f, 0.0f);
        scale = sc;
    }

    TransformComponent(int x, int y, int h, int w, int scl)
    {
        setPosition(x, y);
        height = h;
        width = w;
        scale = scl;
    }

    void init() override
    {
        velocity.x = 0;
        velocity.y = 0;
    }

    void update() override
    {
        if (velocity.x != 0 && velocity.y != 0)
        {
            position.x += velocity.x * speed / 1.41421356237;
            position.y += velocity.y * speed / 1.41421356237;
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