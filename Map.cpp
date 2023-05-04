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

    // for (int y = 0; y < sizeY; y++)
    // {
    //     for (int x = 0; x < sizeX; x++)
    //     {
    //         if (y * sizeX + x - 1 >= 0 && temp[y * sizeX + x - 1] == 0)
    //             Game::neighbor[y * sizeX + x].push_back(y * sizeX + x - 1);
            
    //         if ((y-1) * sizeX + x >= 0 && temp[(y-1) * sizeX + x] == 0)
    //             Game::neighbor[y * sizeX + x].push_back((y-1) * sizeX + x);
            
    //         if ((y+1) * sizeX + x <= sizeX * sizeY - 1 && temp[(y+1) * sizeX + x] == 0)
    //             Game::neighbor[y * sizeX + x].push_back((y+1) * sizeX + x);

    //         if (y * sizeX + x + 1 <= sizeX * sizeY - 1 && temp[y * sizeX + x + 1] == 0)
    //             Game::neighbor[y * sizeX + x].push_back(y * sizeX + x + 1);
            
    //         // std::cout << temp[y * sizeX + x] << " ";
    //     }
    // }
    // std::cout << Game::neighbor[351][1];
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