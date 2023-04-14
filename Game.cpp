#include "Game.h"
#include "ECS/Components.h"
#include "TextureManager.h"
#include "Map.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"

using std::cout;
using std::endl;

Map *map;
Manager manager;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
bool Game::isRunning = false;

AssetManager *Game::assets = new AssetManager(&manager);

auto &player(manager.addEntity());
auto &bigZombie(manager.addEntity());

Game::Game()
{
}

Game::~Game()
{
}

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

    assets->AddTexture("terrain", "assets/tiles_assets.png");
    assets->AddTexture("player", "assets/knight_anims.png");
    assets->AddTexture("projectile", "assets/proj.png");
    // esc implementation

    map = new Map("terrain", 3, 16);
    map->LoadMap("assets/map_demo_30x30.map", 30, 30);

    player.addComponent<TransformComponent>(300, 300, 16, 28, 3);
    player.addComponent<SpriteComponent>("player", true, true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addComponent<HUDComponent>(10, 10);
    player.addGroup(groupPlayers);

    assets->CreatProjectile(Vector2D(3, 3), Vector2D(1, 1), 200, 1, "projectile");
    assets->CreatProjectile(Vector2D(3, 3), Vector2D(1, 2), 200, 1, "projectile");
    assets->CreatProjectile(Vector2D(3, 3), Vector2D(1, 3), 200, 1, "projectile");
    assets->CreatProjectile(Vector2D(3, 3), Vector2D(1, 4), 200, 1, "projectile");
    assets->CreatProjectile(Vector2D(3, 3), Vector2D(1, 5), 200, 1, "projectile");
    assets->CreatProjectile(Vector2D(3, 3), Vector2D(1, 6), 200, 1, "projectile");
    assets->CreatProjectile(Vector2D(3, 3), Vector2D(1, 7), 200, 1, "projectile");

    // bigZombie.addComponent<TransformComponent>(10, 10, 32, 34, 3, 5);
    // bigZombie.addComponent<SpriteComponent>("assets/big_zombie_anims.png", true);
    // bigZombie.addGroup(groupEnemies);
}

auto &tiles(manager.getGroup(Game::groupMap));
auto &players(manager.getGroup(Game::groupPlayers));
auto &colliders(manager.getGroup(Game::groupColliders));
auto &projectiles(manager.getGroup(Game::groupProjectiles));

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

    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;

    manager.refresh();
    manager.update();

    for (auto &c : colliders)
    {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        if (Collision::AABB(playerCol, cCol))
        {
            player.getComponent<TransformComponent>().position = playerPos;
        }
    }

    for (auto &p : projectiles)
    {
        if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
        {
            p->destroy();
            std::cout << "Hit player" << std::endl;
            player.getComponent<HUDComponent>().getDamage(1);
        }
    }

    // camera.x = player.getComponent<TransformComponent>().position.x - SCREEN_WIDTH / 2 ;
    // camera.y = player.getComponent<TransformComponent>().position.y - SCREEN_HEIGHT / 2;
    camera.x = player.getComponent<TransformComponent>().position.x - SCREEN_WIDTH / 2 + player.getComponent<TransformComponent>().width;
    camera.y = player.getComponent<TransformComponent>().position.y - SCREEN_HEIGHT / 2 + player.getComponent<TransformComponent>().height;

    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;
    if (camera.x > camera.w)
        camera.x = camera.w;
    if (camera.y > camera.h)
        camera.y = camera.h;
}

void Game::render()
{
    SDL_RenderClear(renderer);
    for (auto &t : tiles)
    {
        t->draw();
    }

    for (auto &c : colliders)
    {
        c->draw();
    }

    for (auto &p : players)
    {
        p->draw();
    }

    for (auto &p : projectiles)
    {
        p->draw();
    }
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