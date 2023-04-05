#include "TextureManager.h"

SDL_Texture* TextureManager::loadTexture(const char* path)
{
    SDL_Surface *tempSurface = IMG_Load(path);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    if (tex == NULL)
    {
        std::cout << "Failed to load texture " << path << "\n";
    }
    SDL_FreeSurface(tempSurface);

    return tex;
}

SDL_Texture *TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest)
{
    SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}