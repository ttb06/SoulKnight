#pragma once

#include "ECS.h"
#include "../Game.h"
#include "Components.h"
#include <math.h>
#include <queue>
#include "../Constant.h"

extern Entity &player;
const int maxN = 105;

// int r, c;
// char a[maxN][maxN];
// int d[maxN][maxN];
// bool visit[maxN][maxN];
// int moveX[] = {0, 0, 1, -1};
// int moveY[] = {1, -1, 0, 0};

// void bfs(int sx, int sy)
// {
//     for (int i = 1; i <= r; ++i)
//     {
//         std::fill_n(d[i], c + 1, 0);
//         std::fill_n(visit[i], c + 1, false);
//     }

//     std::queue<std::pair<int, int>> q;
//     q.push({sx, sy});
//     visit[sx][sy] = true;
//     while (!q.empty())
//     {
//         int x = q.front().first;
//         int y = q.front().second;
//         q.pop();

//         // Nếu gặp được ô B thì kết thúc thủ tục BFS
//         if (Game::collisionMap[x][y] != 0 && Game::collisionMap[x][y] != 10)//10 is the collision tile in map
//             return;

//         for (int i = 0; i < 4; ++i)
//         {
//             int u = x + moveX[i];
//             int v = y + moveY[i];

//             if (u > r || u < 1)
//                 continue;
//             if (v > c || v < 1)
//                 continue;
//             if (Game::collisionMap[u][v] != 0)
//                 continue;

//             if (!visit[u][v])
//             {
//                 d[u][v] = d[x][y] + 1;
//                 visit[u][v] = true;
//                 q.push({u, v});
//             }
//         }
//     }
// }

class MouseController : public Component
{
public:
    TransformComponent *transform;
    SpriteComponent *sprite;

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {
    }
};