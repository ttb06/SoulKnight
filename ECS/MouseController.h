#pragma once

#include "ECS.h"
#include "../Game.h"
#include "Components.h"
#include <math.h>
#include <queue>
#include "../Constant.h"

extern Entity &player;

class MouseController : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;
    std::vector<std::pair<int, int>> path;
    bool isUsing = true;

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    bool bfs(int r, int c, int sx, int sy, int dx, int dy)
    {
        if (dx == sx && dy == sy)
        {
            path.clear();
            return true;
        }
        int d[maxN][maxN];
        std::pair<int, int> par[maxN][maxN];

        path.clear();
        for (int i = 0; i <= r; i++)
        {
            std::fill_n(d[i], c + 1, 0);
            std::fill_n(Game::visit[i], c + 1, false);
            std::fill_n(par[i], c + 1, std::make_pair(-1, -1));
        }

        std::queue<std::pair<int, int>> q;
        q.push({sx, sy});
        Game::visit[sx][sy] = true;
        while (!q.empty())
        {
            int x = q.front().first;
            int y = q.front().second;
            q.pop();

            // Nếu gặp được ô B thì kết thúc thủ tục BFS
            if (x == dx && y == dy)
            {
                int tmpx = dx, tmpy = dy;
                while (par[tmpx][tmpy].first != sx || par[tmpx][tmpy].second != sy)
                {
                    path.push_back(std::make_pair(tmpx, tmpy));
                    std::pair<int, int> tempPair = par[tmpx][tmpy];
                    tmpx = tempPair.first;
                    tmpy = tempPair.second;
                }
                path.push_back(std::make_pair(tmpx, tmpy));
                path.push_back(std::make_pair(sx, sy));

                return true;
            }

            for (int i = 0; i < 4; ++i)
            {
                int u = x + moveX[i];
                int v = y + moveY[i];

                if (u > r || u < 1)
                    continue;
                if (v > c || v < 1)
                    continue;
                if (Game::collisionMap[u][v] == bfs::block)
                    continue;

                if (!Game::visit[u][v])
                {
                    par[u][v] = std::make_pair(x, y);
                    d[u][v] = d[x][y] + 1;
                    Game::visit[u][v] = true;
                    q.push({u, v});
                }
            }
        }

        return false;
    }

    void update() override
    {
        if (!isUsing) return;
        if (Game::event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (Game::event.button.button == SDL_BUTTON_RIGHT)
            {
                transform->autoMove = true;
                int dx, dy;
                SDL_GetMouseState(&dx, &dy);
                dx += Game::camera.x;
                dy += Game::camera.y;
                dx /= (16 * Game::total_scale);
                dy /= (16 * Game::total_scale);
                go(dx, dy);
            }
        }
        if (transform->autoMove)
        {
            movePath();
        }
        if (path.empty())
        {
            transform->autoMove = false;
        }
    }

    void movePath()
    {
        if (!path.empty() && transform->autoMove == true)
        {
            transform->moveToGrid(path[path.size() - 1].first, path[path.size() - 1].second);

            if (transform->autoMove == false && path.size() != 1)
            {
                transform->autoMove = true;
                path.pop_back();
                std::cout << "Current move: " << path[path.size() - 1].first << " " << path[path.size() - 1].second << std::endl;
            }
        }
    }

    void go(int dx, int dy)
    {
        int sx = entity->getComponent<TransformComponent>().getPosGridX();
        int sy = entity->getComponent<TransformComponent>().getPosGridY();

        if (bfs(LV1_SIZE_X, LV1_SIZE_Y, sx, sy, dx, dy) && transform->autoMove)
        {
            std::cout << "[mouse] Current pos: " << sx << " " << sy << std::endl;
            std::cout << "[Mouse] path:  ";
            for (auto p : path)
            {
                std::cout << p.first << " " << p.second << "   ";
            }
            std::cout << std::endl;
            movePath();
        }
    }
};
