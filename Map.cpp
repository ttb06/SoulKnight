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

SDL_Rect returnRect(int x1, int y1, int x2, int y2)
{
    return SDL_Rect({x1, y1, x2 - x1 + 1, y2 - y1 + 1});
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

            int idx = atoi(index.c_str());

            if (idx == 0)
                continue;
            // voi so da dua vao, so hang don vi chi sprite nap vao quai, con lai la so phong
            // Vi vay, so quai max la 10, se cap nhat cach tinh moi sau
            int enermyIndex = idx % 10;
            std::cout << enermyIndex << std::endl;
            int room = idx % 10;
            Vector2D pos(x * scaledSize, y * scaledSize);
            std::string enermyID, prjID;
            int width, height, speed;
            switch (enermyIndex)
            {
            case 2:
                enermyID = "big_demon";
                prjID = "fire_projectile";
                speed = 1;
                width = 32;
                height = 36;
                break;

            case 1:
                enermyID = "big_zombie";
                prjID = "ice_projectile";
                speed = 1;
                width = 32;
                height = 36;
                break;
            }

            Game::assets->CreateEnermy(pos, speed, width, height, enermyID, 7, 1, prjID, room);
        }
    }

    getline(mapFile, line);
    // load room "door"
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
            std::cout << "[map] door : " << x << "   " << y << std::endl;
            Game::collisionMap[x][y] = bfs::block;
            srcY = 3 * tileSize;
            srcX = 2 * tileSize;
            AddDoorTile(srcX, srcY, x * scaledSize, y * scaledSize, atoi(index.c_str()));

            auto &tCol(manager.addEntity());
            tCol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
            tCol.addGroup(Game::groupDoorColliders);
        }
    }

    getline(mapFile, line);
    // load number of room and enermies in room
    // start from 1, and room 0 auto have 0 enermies
    mapFile >> Game::numberOfRooms;
    Game::roomEnermies.clear();
    for (int i = 0; i < Game::numberOfRooms; i++)
    {
        if (i == 0)
            Game::roomEnermies.push_back(0);
        else
        {
            int t;
            mapFile >> t;
            Game::roomEnermies.push_back(t);
        }
    }

    getline(mapFile, line);
    for (int i = 0; i < Game::numberOfRooms; i++)
    {
        int x1, y1, x2, y2;
        mapFile >> x1 >> y1 >> x2 >> y2;
        // std::cout << x1 << y1 << x2 << y2 << std::endl;
        x1 *= (scaledSize);
        y1 *= (scaledSize);
        x2 *= (scaledSize);
        y2 *= (scaledSize);
        auto &room(manager.addEntity());
        // ColliderComponent(std::string t, int xPos, int yPos, int w, int h, bool isPlayer = false)
        room.addComponent<ColliderComponent>("room", x1, y1, x2 - x1, y2 - y1, false);
        room.addGroup(Game::groupRoom);
        std::cout << room.getComponent<ColliderComponent>().collider.x << " " << room.getComponent<ColliderComponent>().collider.y << std::endl;
    }

    // for (auto r : Game::roomCoordinate)
    // {
    //     std::cout << r.x << " " << r.y << " " << r.w << " " << r.h << std::endl;
    // }
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

void Map::AddDoorTile(int srcX, int srcY, int xPos, int yPos, int room)
{
    auto &tile(manager.addEntity());

    tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, tileSize, mapScale, texID, false, room);
    tile.addGroup(Game::groupDoor);
}