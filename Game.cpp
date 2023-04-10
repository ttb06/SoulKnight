#include "Game.h"
#include "ECS/Components.h"
#include "TextureManager.h"
#include "Map.h"
#include "Vector2D.h"
#include "Collision.h"

using std::cout;
using std::endl;

Map *map;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;
std::vector<ColliderComponent *> Game::colliders;

Manager manager;
auto &player(manager.addEntity());
auto &wall(manager.addEntity());
auto &bigZombie(manager.addEntity());

enum groupLabels : std::size_t
{
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders
};

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
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

        isRunning = true;
    }

    map = new Map();

    // esc implementation

    Map::LoadMap("assets/p16x16.map", 16, 16);

    player.addComponent<TransformComponent>(3);
    player.addComponent<SpriteComponent>("assets/knight_anims.png", true);
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);

    bigZombie.addComponent<TransformComponent>(10, 10, 32, 34, 3);
    bigZombie.addComponent<SpriteComponent>("assets/big_zombie_anims.png", true);
    bigZombie.addGroup(groupEnemies);
}

void Game::handleEvents()
{
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        break;
    }
}

void Game::update()
{
    manager.refresh();
    manager.update();

    for (auto collider : colliders)
    {
        if (Collision::AABB(player.getComponent<ColliderComponent>(),
                            *collider) &&
            collider->tag != player.getComponent<ColliderComponent>().tag)
        {
            player.getComponent<TransformComponent>().velocity * -1;
        }
    }
}

auto &tiles = manager.getGroup(groupMap);
auto &enemies = manager.getGroup(groupEnemies);
auto &players = manager.getGroup(groupPlayers);

void Game::render()
{
    SDL_RenderClear(renderer);
    for (auto t : tiles)
    {
        t->draw();
    }
    for (auto p : players)
    {
        p->draw();
    }
    for (auto e : enemies)
    {
        e->draw();
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

void Game::AddTile(int id, int x, int y)
{
    auto &tile(manager.addEntity());
    tile.addComponent<TileComponent>(x, y, 16, 16, id);
    tile.addGroup(groupMap);
}