#include "Game.h"
#include "ECS/Components.h"
#include "TextureManager.h"
#include "Map.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"

#include <sstream>

using std::cout;
using std::endl;

Map *map;
Manager manager;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

bool Game::isRunning = false;

AssetManager *Game::assets = new AssetManager(&manager);

Entity &player(manager.addEntity());
Entity &label(manager.addEntity());
Entity &weapon(manager.addEntity());

Game::Game() {}

Game::~Game() {}

// add Groups
std::vector<Entity *> &tiles(manager.getGroup(Game::groupMap));
std::vector<Entity *> &players(manager.getGroup(Game::groupPlayers));
std::vector<Entity *> &colliders(manager.getGroup(Game::groupColliders));
std::vector<Entity *> &projectiles(manager.getGroup(Game::groupProjectiles));
std::vector<Entity *> &enermies(manager.getGroup(Game::groupEnermies));
std::vector<Entity *> &weapons(manager.getGroup(Game::groupWeapons));

void Game::init(const char *title, int xPos, int yPos, int width, int height, bool fullScreen)
{
    int flags = 0;
    if (fullScreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        cout << "Subsystem initialised! ...\n";

        window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
        if (window)
        {
            cout << "Window created! ...\n";
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            cout << "Renderer initialised! ...\n";
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }

        isRunning = true;
    }

    if (TTF_Init() == -1)
    {
        std::cout << "Error: SDL_TTF" << std::endl;
    }

    // assets implenemtation

    // add Texture
    assets->AddTexture("terrain", "assets/tiles_assets.png");
    assets->AddTexture("player", "assets/knight_anims.png");
    assets->AddTexture("projectile", "assets/proj.png");
    assets->AddTexture("big_demon", "assets/big_demon_amins.png");
    assets->AddTexture("anime_sword", "assets/weapon_anime_sword.png");

    // add Font
    assets->AddFont("candara", "assets/candara.ttf", 16);

    map = new Map("terrain", 3, 16);

    // esc implementation
    map->LoadMap("assets/map_demo_30x30.map", 30, 30);

    player.addComponent<TransformComponent>(301, 301, 16, 28, 3, 5);
    player.addComponent<DirectionComponent>();
    player.addComponent<SpriteComponent>("player", true, true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addComponent<HUDComponent>(10, 10, Vector2D(6, 6), true);
    player.addGroup(groupPlayers);

    weapon.addComponent<DirectionComponent>();
    weapon.addComponent<WeaponComponent>("anime_sword", 30, 12, 2);
    weapon.addGroup(Game::groupWeapons);

    SDL_Color white = {255, 255, 255, 255};

    label.addComponent<UILabel>(10, 10, "Init", "candara", white);

    assets->CreatProjectile(Vector2D(3, 3), Vector2D(1, 1), 200, 1, "projectile");
    assets->CreatProjectile(Vector2D(3, 3), Vector2D(1, 2), 200, 1, "projectile");
    assets->CreatProjectile(Vector2D(3, 3), Vector2D(1, 3), 200, 1, "projectile");
    assets->CreatProjectile(Vector2D(3, 3), Vector2D(1, 4), 200, 1, "projectile");
    assets->CreatProjectile(Vector2D(3, 3), Vector2D(1, 5), 200, 1, "projectile");
    assets->CreatProjectile(Vector2D(3, 3), Vector2D(1, 6), 200, 1, "projectile");
    assets->CreatProjectile(Vector2D(3, 3), Vector2D(1, 7), 200, 1, "projectile");

    assets->CreateEnermy(Vector2D(500, 500), 1, 32, 36, "big_demon", 7, 1);
}

void Game::handleEvents()
{
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDLK_ESCAPE:
        isRunning = false;
        break;
    default:
        break;
    }
}

void Game::update()
{
    // store old information
    if (player.getComponent<HUDComponent>().curHealth <= 0){
        Game::isRunning = false;
    std::cout << "[Game.cpp]: Running out of health" << std::endl;
    }

    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;
    Vector2D playerVel = player.getComponent<TransformComponent>().velocity;
    int playerSpeed = player.getComponent<TransformComponent>().speed;

    manager.refresh();
    manager.update();

    std::stringstream ss;
    ss << "Player Position: " << playerPos;
    label.getComponent<UILabel>().deleteTexture();
    label.getComponent<UILabel>().SetLabelText(ss.str(), "candara");

    for (auto &c : colliders)
    {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        if (Collision::AABB(playerCol, cCol))
        {
            std::cout << "[Game.cpp]: Player hit collider" << std::endl;
            player.getComponent<TransformComponent>().position.x -= playerVel.x * playerSpeed;
            player.getComponent<TransformComponent>().position.y -= playerVel.y * playerSpeed;

            if (player.getComponent<TransformComponent>().position.x == playerPos.x &&
                player.getComponent<TransformComponent>().position.y == playerPos.y)
            {
                std::cout << "[Game.cpp]: Player returned to the old position" << std::endl;
            }
            break;
        }
    }

    for (auto &e : enermies)
    {
        SDL_Rect eCol = e->getComponent<ColliderComponent>().collider;
        if (Collision::AABB(playerCol, eCol))
        {
            player.getComponent<HUDComponent>().getDamage(e->getComponent<EnermyComponent>().collisionDamage);
        }
    }

    for (auto &p : projectiles)
    {
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
        {
            p->destroy();
            std::cout << "[Game.cpp]: Hit player" << std::endl;
            // player.getComponent<HUDComponent>().getDamage(1);
        }
    }

    // camera.x = player.getComponent<TransformComponent>().position.x - SCREEN_WIDTH / 2 ;
    // camera.y = player.getComponent<TransformComponent>().position.y - SCREEN_HEIGHT / 2;
    camera.x = player.getComponent<TransformComponent>().position.x - SCREEN_WIDTH / 2 + player.getComponent<TransformComponent>().width / 2;
    camera.y = player.getComponent<TransformComponent>().position.y - SCREEN_HEIGHT / 2 + player.getComponent<TransformComponent>().height / 2;

    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;
    if (camera.x > LEVEL_WIDTH - camera.w)
        camera.x = LEVEL_WIDTH - camera.w;
    if (camera.y > camera.h)
        camera.y = LEVEL_HEIGHT - camera.w;
}

void Game::render()
{
    SDL_RenderClear(renderer);
    for (auto &t : tiles)
    {
        // if (Collision::AABB(camera, t->getComponent<ColliderComponent>().collider))
        t->draw();
    }

    for (auto &c : colliders)
    {
        if (Collision::AABB(c->getComponent<ColliderComponent>().collider, player.getComponent<ColliderComponent>().collider))
            c->draw();
    }

    for (auto &e : enermies)
    {
        if (!Collision::isFront(e->getComponent<ColliderComponent>().collider, player.getComponent<ColliderComponent>().collider))
            e->draw();
    }

    for (auto &p : players)
    {
        p->draw();
    }

    for (auto &e : enermies)
    {
        if (Collision::isFront(e->getComponent<ColliderComponent>().collider, player.getComponent<ColliderComponent>().collider))
            e->draw();
    }

    for (auto &p : projectiles)
    {
        if (Collision::AABB(camera, p->getComponent<ColliderComponent>().collider))
            p->draw();
    }

    for (auto &w : weapons)
    {
        w->draw();
    }

    label.draw();

    SDL_RenderPresent(renderer);
}

void Game::clean()
{
    window = nullptr;
    renderer = nullptr;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    cout << "Game cleaned! ...\n";
}