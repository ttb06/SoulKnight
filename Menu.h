#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "LTexture.h"
#include <iostream>

using namespace std;

class Menu
{
public:
    LTexture *background;
    LTexture *play;
    bool isPlay = false;

    Menu(std::string bg, std::string pl);
    
    ~Menu();

    void draw();

    void update();
    
};