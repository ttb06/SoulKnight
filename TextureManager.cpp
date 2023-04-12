#include "TextureManager.h"

SDL_Texture *TextureManager::loadTexture(const char *path)
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

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, NULL, NULL, flip);
}

void TextureManager::drawRect(int xPos, int yPos, int width, int height, SDL_Color color)
{
    SDL_Rect destR = {xPos, yPos, width, height};
    SDL_SetRenderDrawColor(Game::renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(Game::renderer, &destR);
    SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
}