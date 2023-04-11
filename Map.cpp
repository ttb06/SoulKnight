#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS\ECS.h"
#include "ECS\Components.h"

// manager has been defined somewhere else
extern Manager manager;

Map::Map(const char *mfp, int ms, int ts)
{
    mapFilePath = mfp;
    mapScale = ms;
    tileSize = ts;
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

            AddTile(srcX, srcY, x * tileSize * mapScale, y * tileSize * mapScale);
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
            if (atoi(index.c_str()) == 1)
            {
                auto &tCol(manager.addEntity());
                tCol.addComponent<ColliderComponent>("terrain", x * tileSize * mapScale, y * tileSize * mapScale, tileSize * mapScale);
                tCol.addGroup(Game::groupColliders);
            }
        }
    }
}

void Map::AddTile(int srcX, int srcY, int xPos, int yPos)
{
    auto &tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, tileSize, mapScale, mapFilePath);
    tile.addGroup(Game::groupMap);
}