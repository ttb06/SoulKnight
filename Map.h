#pragma once
#include <string>

class Map
{
public:
    Map(std::string tID, int ms, int ts);
    ~Map();

    void LoadMap(std::string path, int sizeX, int sizeY);
    void AddTile(int srcX, int srcY, int xPos, int yPos);

private:
    std::string texID;
    int mapScale;
    int tileSize;
    int scaledSize;
};