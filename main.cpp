#include "Game.h"
#include "Constant.h"

Game *game = nullptr;

int main(int argc, char *args[])
{
    game = new Game();
    game->init("Hello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);
    
    Uint32 frameStart;
    int frameTime;
    
    while (game->running())
    {
        frameStart = SDL_GetTicks();
        
        game->handleEvents();
        game->update();
        game->render();
        
        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    game->clean();
    return 0;
}
