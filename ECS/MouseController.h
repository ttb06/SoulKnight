#pragma once

#include "ECS.h"
#include "../Game.h"
#include "Components.h"
#include <math.h>
#include <queue>
#include "../Constant.h"
#include <iomanip>

using std::setw;

extern Entity &player;

// int r, c;
// char a[maxN][maxN];
// int d[maxN][maxN];
// bool Game::visit[maxN][maxN];

class MouseController : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;
    std::vector<std::pair<int, int>> path;

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
        for (int i = 0; i < r; ++i)
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
                    tmpx = par[tmpx][tmpy].first;
                    tmpy = par[tmpx][tmpy].second;
                }
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
        if (transform->autoMove)
        {
            movePath();
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
            }
        }
        transform->autoMove = false;
    }

    void go()
    {
        int sx = entity->getComponent<TransformComponent>().getPosGridX();
        int sy = entity->getComponent<TransformComponent>().getPosGridY();

        // int sx = entity->getComponent<TransformComponent>().getPosX;
        // int sy = entity->getComponent<TransformComponent>().getPosY;

        int dx = 10;
        int dy = 10;

        if (bfs(60, 60, sx, sy, dx, dy) && transform->autoMove)
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
        // std::cout << "[mouse] an rumming" << std::endl;
    }
};
