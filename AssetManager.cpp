#include "AssetManager.h"
#include "ECS/Components.h"

AssetManager::AssetManager(Manager *man)
{
    manager = man;
}

AssetManager::~AssetManager()
{
}

void AssetManager::CreatProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id)
{
    auto &projectile(manager->addEntity());
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 16, 16, 1);
    projectile.addComponent<SpriteComponent>(id, false);
    projectile.addComponent<ProjectileComponent>(range, speed, vel);
    projectile.addComponent<ColliderComponent>("Projectile");
    projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::CreateEnermy(Vector2D pos, int spd, int width, int height, std::string idSprite, int mH, int cD)
{
    auto &enermy(manager->addEntity());
    enermy.addComponent<TransformComponent>(pos.x, pos.y, width, height, 3, spd);
    enermy.addComponent<SpriteComponent>(idSprite, true, false);
    enermy.addComponent<ColliderComponent>(idSprite);
    enermy.addComponent<EnermyComponent>(mH, cD);
    enermy.addGroup(Game::groupEnermies);
}

void AssetManager::AddTexture(std::string id, const char *path)
{
    textures.emplace(id, TextureManager::loadTexture(path));
}

SDL_Texture *AssetManager::GetTexture(std::string id)
{
    return textures[id];
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
    fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font *AssetManager::GetFont(std::string id)
{
    return fonts[id];
}