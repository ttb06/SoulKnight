#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS\ECS.h"
#include "ECS\Components.h"

// manager has been defined somewhere else
extern Manager manager;

Map::Map(std::string tID, int ms, int ts)
{
    texID = tID;
    mapScale = ms;
    tileSize = ts;
    scaledSize = ms * ts;
}

Map::~Map()
{
}
int count = 0;
void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
    char c;
    std::ifstream mapFile;
    mapFile.open(path);

    int srcX, srcY;

    std::string line;
    // load map
    for (int y = 0; y < sizeY; y++)
    {
        getline(mapFile, line);
        int cnt = 0;
        for (int x = 0; x < sizeX; x++)
        {
            std::string index = "";
            for (; cnt < line.size(); cnt++)
            {
                if (line[cnt] != ',')
                    index.push_back(line[cnt]);
                else
                    break;
            }
            cnt++;

            srcY = (atoi(index.c_str()) / 10) * tileSize;
            srcX = (atoi(index.c_str()) % 10) * tileSize;

            if (srcY == 0 && srcX == 0)
                continue;
            AddTile(srcX, srcY, x * scaledSize, y * scaledSize);
        }
    }

    getline(mapFile, line);
    // load Higher map
    for (int y = 0; y < sizeY; y++)
    {
        getline(mapFile, line);
        int cnt = 0;
        for (int x = 0; x < sizeX; x++)
        {
            std::string index = "";
            for (; cnt < line.size(); cnt++)
            {
                if (line[cnt] != ',')
                    index.push_back(line[cnt]);
                else
                    break;
            }
            cnt++;

            srcY = (atoi(index.c_str()) / 10) * tileSize;
            srcX = (atoi(index.c_str()) % 10) * tileSize;

            if (srcY == 0 && srcX == 0)
                continue;
            AddHigherTile(srcX, srcY, x * scaledSize, y * scaledSize);
        }
    }

    getline(mapFile, line);

    // load Collision map into Map and Game::collisionMap for path finding with bfs
    for (int y = 0; y < sizeY; y++)
    {
        getline(mapFile, line);
        int cnt = 0;
        for (int x = 0; x < sizeX; x++)
        {
            std::string index = "";
            for (; cnt < line.size(); cnt++)
            {
                if (line[cnt] != ',')
                    index.push_back(line[cnt]);
                else
                    break;
            }
            cnt++;

            Game::collisionMap[x][y] = atoi(index.c_str());
            if (atoi(index.c_str()) != 0)
            {
                auto &tCol(manager.addEntity());
                tCol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
                tCol.addGroup(Game::groupColliders);
            }
        }
    }

    getline(mapFile, line);
    // load anim map
    for (int y = 0; y < sizeY; y++)
    {
        getline(mapFile, line);
        int cnt = 0;
        for (int x = 0; x < sizeX; x++)
        {
            std::string index = "";
            for (; cnt < line.size(); cnt++)
            {
                if (line[cnt] != ',')
                    index.push_back(line[cnt]);
                else
                    break;
            }
            cnt++;

            srcY = (atoi(index.c_str()) / 10) * tileSize;
            srcX = (atoi(index.c_str()) % 10) * tileSize;

            if (srcY == 0 && srcX == 0)
                continue;
            AddAnimTile(srcX, srcY, x * scaledSize, y * scaledSize);
        }
    }

    // get enermies coordinate
    //  /10: cach quai ban (projectiles)
    //  %10: asset quai
    getline(mapFile, line);
    for (int y = 0; y < sizeY; y++)
    {
        getline(mapFile, line);
        int cnt = 0;
        for (int x = 0; x < sizeX; x++)
        {
            std::string index = "";
            for (; cnt < line.size(); cnt++)
            {
                if (line[cnt] != ',')
                    index.push_back(line[cnt]);
                else
                    break;
            }
            cnt++;

            srcY = (atoi(index.c_str()) / 10) * tileSize;
            srcX = (atoi(index.c_str()) % 10) * tileSize;

            if (srcY == 0 && srcX == 0)
                continue;
            std::cout << "[Map] created enermy " << x << "   " << y << std::endl;
            Vector2D pos(x * scaledSize, y * scaledSize);
            Game::assets->CreateEnermy(pos, 1, 32, 36, "big_demon", 7, 1, "fire_projectile");
        }
    }
    mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xPos, int yPos)
{
    auto &tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, tileSize, mapScale, texID);
    tile.addGroup(Game::groupMap);
}

void Map::AddHigherTile(int srcX, int srcY, int xPos, int yPos)
{
    auto &tile(manager.addEntity());

    tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, tileSize, mapScale, texID);
    tile.addGroup(Game::groupHigherMap);
}

void Map::AddAnimTile(int srcX, int srcY, int xPos, int yPos)
{
    auto &tile(manager.addEntity());

    tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, tileSize, mapScale, texID, true);
    tile.addGroup(Game::groupAnimMap);
}