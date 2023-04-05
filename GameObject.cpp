#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char *textureSheet, int _xPos, int _yPos)
{
    objTexture = TextureManager::loadTexture(textureSheet);

    xPos = _xPos;
    yPos = _yPos;
}

GameObject::~GameObject()
{
}

void GameObject::update()
{
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = 16;
    srcRect.h = 28;

    destRect.x = xPos;
    destRect.y = yPos;
    destRect.w = srcRect.w*2;
    destRect.h = srcRect.h*2;
}

void GameObject::render(){
    SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}
