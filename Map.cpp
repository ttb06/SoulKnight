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

            AddTile(srcX, srcY, x * scaledSize, y * scaledSize);
        }
    }

    getline(mapFile, line);
    // load Collision map
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
            if (atoi(index.c_str()) != 0)
            {
                auto &tCol(manager.addEntity());
                tCol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
                tCol.addGroup(Game::groupColliders);
            }
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

            AddHigherTile(srcX, srcY, x * scaledSize, y * scaledSize);
        }
    }
}

void Map::AddTile(int srcX, int srcY, int xPos, int yPos)
{
    auto &tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, tileSize, mapScale, texID);
    tile.addGroup(Game::groupMap);
}

void Map::AddHigherTile(int srcX, int srcY, int xPos, int yPos)
{
    auto &tile (manager.addEntity());
    
    tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, tileSize, mapScale, texID);
    tile.addGroup(Game::groupHigherMap);
}