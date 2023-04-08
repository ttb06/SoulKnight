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
std::vector<ColliderComponent*> Game::colliders;

Manager manager;
auto &player(manager.addEntity());

auto &wall(manager.addEntity());
auto &zombie(manager.addEntity());

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

    player.addComponent<TransformComponent>(2);
    player.addComponent<SpriteComponent>("assets/knight_idle_sprite.png");
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");

    zombie.addComponent<TileComponent>(300, 300, 16, 16, 2);
    zombie.addComponent<SpriteComponent>("assets/ice_zombie_idle_anim_f0.png");
    zombie.addComponent<ColliderComponent>("zombie");
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

    for (auto collider: colliders)
    {
        if (Collision::AABB(player.getComponent<ColliderComponent>(),
                            *collider) && collider->tag!= player.getComponent<ColliderComponent>().tag )
        {
            player.getComponent<TransformComponent>().velocity * -1;
        }
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    // Draw anything to here
    map->DrawMap();
    manager.draw();
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
