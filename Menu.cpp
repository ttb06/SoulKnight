#include "Menu.h"
#include "Constant.h"
#include "Game.h"

using namespace std;

// LTexture *background;
// LTexture *play;
// bool isPlay = false;

Menu::Menu(string bg, string pl)
{
    background->loadFromFileee("assets/background.png");
    play->loadFromFileee("assets/play.png");
};

Menu::~Menu(){};

void Menu::draw()
{
    SDL_Rect clip = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    background->render(0, 0, &clip);
    play->render(800, 570);
}

void Menu::update()
{
    if (Game::event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (Game::event.button.button == SDL_BUTTON_RIGHT)
            {
                int dx, dy;
                SDL_GetMouseState(&dx, &dy);
                if (dx >= 800 && dx <= 800 + play->getWidth() && dy >= 570 && dy <= 570 + play->getHeight())
                {
                    Game::curLevel ++;
                }
            }
        }
}