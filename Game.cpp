#include "Game.h"
#include "ECS/Components.h"
#include "TextureManager.h"
#include "Map.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
// #include "Menu.h"

// #include "Timer.h"

#include <sstream>

using std::cout;
using std::endl;

Mix_Music *gMusic = NULL;
Mix_Music *gCombat = NULL;

Mix_Chunk *gPlayerHurt = NULL;
Mix_Chunk *gSlash = NULL;

bool isPlayingMusic = false;
bool isPlayingCombat = false;

Map *gamemap;
Manager manager;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;
int Game::total_scale = 3;
int Game::curLevel = 0;
int Game::collisionMap[105][105];
int Game::visit[maxN][maxN];
int Game::numberOfRooms;
vector<int> Game::roomEnermies;
int Game::curRoom;

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
std::vector<Entity *> &doorcolliders(manager.getGroup(Game::groupDoorColliders));
std::vector<Entity *> &projectiles(manager.getGroup(Game::groupProjectiles));
std::vector<Entity *> &enermies(manager.getGroup(Game::groupEnermies));
std::vector<Entity *> &weapons(manager.getGroup(Game::groupWeapons));
std::vector<Entity *> &highertiles(manager.getGroup(Game::groupHigherMap));
std::vector<Entity *> &animtiles(manager.getGroup(Game::groupAnimMap));
std::vector<Entity *> &doortiles(manager.getGroup(Game::groupDoor));
std::vector<Entity *> &rooms(manager.getGroup(Game::groupRoom));

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
        // Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        }

        // Initialize SDL_mixer
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        }
    }

    if (TTF_Init() == -1)
    {
        std::cout << "Error: SDL_TTF" << std::endl;
    }

    gMusic = Mix_LoadMUS("sound/music.wav");
    gCombat = Mix_LoadMUS("sound/combat.wav");

    gPlayerHurt = Mix_LoadWAV("sound/playerHurt.wav");
    gSlash = Mix_LoadWAV("sound/sword.wav");
    //  assets implenemtation

    //  add Texture
    assets->AddTexture("terrain", "assets/tiles_assets_final.png");
    assets->AddTexture("projectile", "assets/proj.png");
    assets->AddTexture("door", "assets/door.png");

    assets->AddTexture("player", "assets/knight_anims.png");
    assets->AddTexture("anime_sword", "assets/weapon_anime_sword.png");
    assets->AddTexture("katana", "assets/katana_slash.png");

    assets->AddTexture("big_demon", "assets/big_demon_anims.png");
    assets->AddTexture("big_zombie", "assets/big_zombie_anims.png");
    assets->AddTexture("fire_projectile", "assets/projectile_fire_ball.png");
    assets->AddTexture("ice_projectile", "assets/projectile_ice_ball.png");
    assets->AddTexture("skull", "assets/skull.png");
    assets->AddTexture("gameover", "assets/gameover.png");
    assets->AddTexture("win", "assets/win.png");
    assets->AddTexture("menu", "assets/background.png");
    assets->AddTexture("ui_heart", "assets/ui_heart.png");
    assets->AddTexture("ui_armor", "assets/ui_armor.png");

    // add Font
    assets->AddFont("DungeonFont", "assets/DungeonFont.ttf", 16);

    gamemap = new Map("terrain", Game::total_scale, 16);

    // // esc implementation
    // map->LoadMap("assets/map_lv1.txt", LV1_SIZE_X, LV1_SIZE_Y);

    SDL_Color white = {255, 255, 255, 255};

    label.addComponent<UILabel>(10, 10, "Init", "DungeonFont", white);
    for (int i = 0; i < rooms.size(); i++)
    {
        cout << rooms[i]->getComponent<ColliderComponent>().collider.x << "   " << rooms[i]->getComponent<ColliderComponent>().collider.y << " " << rooms[i]->getComponent<ColliderComponent>().collider.w << " " << rooms[i]->getComponent<ColliderComponent>().collider.h << endl;
    }
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

void clear_group()
{
    tiles.clear();
    colliders.clear();
    doorcolliders.clear();
    projectiles.clear();
    enermies.clear();
    highertiles.clear();
    animtiles.clear();
    doortiles.clear();
    rooms.clear();
}

void Game::loadNextLevel()
{
    curLevel++;
    if (curLevel == 0)
    {
    }
    else if (curLevel > NUMBER_OF_LEVEL)
    {
    }
    else
    {
        std::string levelMap = "assets/map_lv" + std::to_string(curLevel) + ".txt";
        cout << levelMap << endl;
        clear_group();
        gamemap->LoadMap(levelMap, LV_SIZE_X[curLevel - 1], LV_SIZE_Y[curLevel - 1]);

        if (curLevel == 1)
        {
            // create player
            player.addComponent<TransformComponent>(400, 400, 16, 28, Game::total_scale, 5);
            player.addComponent<DirectionComponent>();
            player.addComponent<SpriteComponent>("player", true, true);
            player.addComponent<MouseController>();
            player.addComponent<KeyboardController>();
            player.addComponent<ColliderComponent>("player", 0, 12, 16, true);
            player.addComponent<HUDComponent>(8, 4, Vector2D(6, 6), 2, "ui_heart", "ui_armor");
            player.addComponent<UpdateSpriteComponent>();
            player.addGroup(groupPlayers);

            // create weapon
            weapon.addComponent<DirectionComponent>();
            weapon.addComponent<TransformComponent>();
            weapon.addComponent<WeaponComponent>("katana", 29, 6, Game::total_scale);
            weapon.addComponent<WeaponSpriteComponent>("katana", 29, 6, Game::total_scale);
            weapon.addComponent<WeaponKeyboardController>();
            weapon.addGroup(Game::groupWeapons);
        }
        else
        {
            player.getComponent<TransformComponent>().setPosition(400, 400);
        }
    }
}

void Game::update()
{
    if (curLevel == 0)
    {
        if (Game::event.type == SDL_MOUSEBUTTONDOWN)
        {

            Game::loadNextLevel();
        }
    };

    if (curLevel > 0 && curLevel <= NUMBER_OF_LEVEL)
    {
        weapon.getComponent<TransformComponent>().setPosition(player.getComponent<TransformComponent>().position.x + player.getComponent<TransformComponent>().width / 2,
                                                              player.getComponent<TransformComponent>().position.y + player.getComponent<TransformComponent>().height / 2);
        // store old information
        if (player.getComponent<HUDComponent>().curHealth == 0)
        {
            curLevel = 1e9;
        }
        for (int i = 0; i < doortiles.size(); i++)
        {
            auto &d = doortiles[i];
            if (Game::roomEnermies[d->getComponent<TileComponent>().getRoom()] == 0)
            {
                int xx = doortiles[i]->getComponent<TileComponent>().position.x / 16 / Game::total_scale;
                int yy = doortiles[i]->getComponent<TileComponent>().position.y / 16 / Game::total_scale;
                Game::collisionMap[xx][yy] = 0;
                doortiles.erase(doortiles.begin() + i);
                doorcolliders.erase(doorcolliders.begin() + i);
            }
        }
        // for (auto &d : doortiles)
        // {
        //     cout << d->getComponent<TileComponent>().getRoom() << "    ";
        // }
        // cout << endl;

        if (player.getComponent<HUDComponent>().curHealth <= 0)
        {
            // Game::isRunning = false;
            // std::cout << "[Game.cpp]: Running out of health" << std::endl;
        }

        Game::curRoom = -1;
        for (int i = 0; i < rooms.size(); i++)
        {
            if (Collision::AABB(rooms[i]->getComponent<ColliderComponent>().collider, player.getComponent<ColliderComponent>().collider))
            {
                cout << "Dang o phong " << i << endl;
                Game::curRoom = i;
                if (curRoom == Game::numberOfRooms - 1)
                {
                    cout << "[Game] DANG O PHONG CUOI" << endl;
                    loadNextLevel();
                }
            }
        }

        for (auto &e : enermies)
        {
            if (e->getComponent<EnermyComponent>().room != Game::curRoom)
            {
                e->getComponent<EnermyComponent>().deactive();
            }
            else
                e->getComponent<EnermyComponent>().active();
            ;
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

        for (auto &c : doorcolliders)
        {
            SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
            if (Collision::AABB(player.getComponent<ColliderComponent>().collider, cCol))
            {
                player.getComponent<TransformComponent>().position.x -= playerVel.x * playerSpeed;
                player.getComponent<ColliderComponent>().update();
                if (Collision::AABB(player.getComponent<ColliderComponent>().collider, cCol))
                {
                    player.getComponent<TransformComponent>().position.x += playerVel.x * playerSpeed;
                }

                player.getComponent<TransformComponent>().position.y -= playerVel.y * playerSpeed;
                player.getComponent<ColliderComponent>().update();
                if (Collision::AABB(player.getComponent<ColliderComponent>().collider, cCol))
                {
                    player.getComponent<TransformComponent>().position.y += playerVel.y * playerSpeed;
                }
                player.getComponent<ColliderComponent>().update();
                break;
            }
        }

        for (auto &c : colliders)
        {
            SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
            if (Collision::AABB(player.getComponent<ColliderComponent>().collider, cCol))
            {
                player.getComponent<TransformComponent>().position.x -= playerVel.x * playerSpeed;
                player.getComponent<ColliderComponent>().update();
                if (Collision::AABB(player.getComponent<ColliderComponent>().collider, cCol))
                {
                    player.getComponent<TransformComponent>().position.x += playerVel.x * playerSpeed;
                }

                player.getComponent<TransformComponent>().position.y -= playerVel.y * playerSpeed;
                player.getComponent<ColliderComponent>().update();
                if (Collision::AABB(player.getComponent<ColliderComponent>().collider, cCol))
                {
                    player.getComponent<TransformComponent>().position.y += playerVel.y * playerSpeed;
                }
                player.getComponent<ColliderComponent>().update();
                break;
            }
        }

        for (auto &e : enermies)
        {
            SDL_Rect eCol = e->getComponent<ColliderComponent>().collider;
            if (Collision::AABB(playerCol, eCol))
            {
                player.getComponent<HUDComponent>().getDamage(e->getComponent<EnermyComponent>().collisionDamage);
                Mix_PlayChannel(-1, gPlayerHurt, 0);
            }
        }

        for (auto &p : projectiles)
        {
            if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
            {
                p->getComponent<ProjectileSpriteComponent>().destroy();
                player.getComponent<HUDComponent>().getDamage(p->getComponent<ProjectileComponent>().dame());
                Mix_PlayChannel(-1, gPlayerHurt, 0);
            }
        }

        Vector2D radiusWeapon = player.getComponent<DirectionComponent>().vec;
        radiusWeapon.normalize();
        radiusWeapon.x *= RANGE_MELE_WEAPON * Game::total_scale;
        radiusWeapon.y *= RANGE_MELE_WEAPON * Game::total_scale;

        for (auto &e : enermies)
        {
            if (weapon.getComponent<WeaponComponent>().isAtacking &&
                weapon.getComponent<WeaponComponent>().attackCounter != e->getComponent<EnermyComponent>().lastTakenDamage &&
                (Collision::Circle((int)weapon.getComponent<TransformComponent>().position.x, (int)weapon.getComponent<TransformComponent>().position.y, radiusWeapon.Abs(), e->getComponent<ColliderComponent>().collider)))
            {
                std::cout << "[Game]: Current health: " << e->getComponent<EnermyComponent>().curHealth << endl;
                e->getComponent<EnermyComponent>().lastTakenDamage = weapon.getComponent<WeaponComponent>().attackCounter;
                e->getComponent<EnermyComponent>().takeDamage(player.getComponent<HUDComponent>().attackDamage);
            }
        }

        for (auto &p : projectiles)
        {
            // std::cout << "[Game] Position of projectile: " << p->getComponent<TransformComponent>().position << "      "
            // << "[Game] player Position: " << player.getComponent<TransformComponent>().position << endl;
            if (weapon.getComponent<WeaponComponent>().isAtacking &&
                Collision::Circle((int)weapon.getComponent<TransformComponent>().position.x, (int)weapon.getComponent<TransformComponent>().position.y, radiusWeapon.Abs(), p->getComponent<ColliderComponent>().collider))
            {
                p->getComponent<ProjectileSpriteComponent>().destroy();
            }
        }

        float newXposCam;
        float newYposCam;

        newXposCam = player.getComponent<TransformComponent>().position.x - SCREEN_WIDTH / 2;
        newYposCam = player.getComponent<TransformComponent>().position.y - SCREEN_HEIGHT / 2;

        camera.x = camera.x + (newXposCam - camera.x) * SMOOTHING_FACTOR;
        camera.y = camera.y + (newYposCam - camera.y) * SMOOTHING_FACTOR;

        if (camera.x < 0)
            camera.x = 0;
        if (camera.y < 0)
            camera.y = 0;
        if (camera.x > LEVEL_WIDTH - camera.w)
            camera.x = LEVEL_WIDTH - camera.w;
        if (camera.y > LEVEL_HEIGHT - camera.h)
            camera.y = LEVEL_HEIGHT - camera.h;
    }

    if (curLevel > NUMBER_OF_LEVEL)
    {
        std::cout << "OKKKKKKKK" << std::endl;
        player.getComponent<HUDComponent>().curHealth = 0;
        SDL_Texture *gameOver = assets->GetTexture("gameOver");
        SDL_Rect dest = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Rect src = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        TextureManager::Draw(gameOver, src, dest, SDL_FLIP_NONE);
    }
}

void Game::render()
{
    SDL_RenderClear(renderer);
    // render map (floor, things which are always behind player)
    if (curLevel == 0)
    {
        if (!isPlayingMusic)
        {
            isPlayingMusic = true;
            Mix_PlayMusic(gMusic, -1);
        }

        SDL_Texture *gameOver = assets->GetTexture("menu");
        SDL_Rect dest = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Rect src = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        TextureManager::Draw(gameOver, src, dest, SDL_FLIP_NONE);
    }

    if (curLevel > 0 && curLevel <= NUMBER_OF_LEVEL)
    {
        if (!isPlayingCombat)
        {
            isPlayingCombat = true;
            Mix_PlayMusic(gCombat, -1);
        }

        for (auto &t : tiles)
        {
            t->draw();
        }

        for (auto &t : animtiles)
        {
            t->draw();
        }
        // render enermies which are behind player
        for (auto &e : enermies)
        {
            if (!Collision::isFront(e->getComponent<ColliderComponent>().collider, player.getComponent<ColliderComponent>().collider))
                e->draw();
        }

        // render player
        for (auto &p : players)
        {
            p->draw();
        }

        // render player's weapons
        for (auto &w : weapons)
        {
            w->draw();
        }

        // render enermies which are in front of player
        for (auto &e : enermies)
        {
            if (Collision::isFront(e->getComponent<ColliderComponent>().collider, player.getComponent<ColliderComponent>().collider))
                e->draw();
        }

        // render projectiles
        for (auto &p : projectiles)
        {
            // if (Collision::AABB(camera, p->getComponent<ColliderComponent>().collider))
            p->draw();
        }

        // render wall in front of player
        for (auto &t : highertiles)
        {
            t->draw();
        }

        for (auto &d : doortiles)
        {
            d->draw();
        }

        // render colliders
        // for (auto &c : colliders)
        // {
        //     // if (Collision::AABB(c->getComponent<ColliderComponent>().collider, player.getComponent<ColliderComponent>().collider))
        //     c->draw();
        // }

        // draw hud to the topmost
        player.getComponent<HUDComponent>().draw();
        // label.draw();
    }
    if (curLevel == NUMBER_OF_LEVEL + 1)
    {
        if (!isPlayingMusic)
        {
            isPlayingMusic = true;
            Mix_PlayMusic(gMusic, -1);
        }
        player.getComponent<HUDComponent>().curHealth = 0;
        SDL_Texture *gameOver = assets->GetTexture("win");
        SDL_Rect dest = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Rect src = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        TextureManager::Draw(gameOver, src, dest, SDL_FLIP_NONE);
    }

    if (curLevel > NUMBER_OF_LEVEL + 1)
    {
        if (!isPlayingMusic)
        {
            isPlayingMusic = true;
            Mix_PlayMusic(gMusic, -1);
        }
        player.getComponent<HUDComponent>().curHealth = 0;
        SDL_Texture *gameOver = assets->GetTexture("gameover");
        SDL_Rect dest = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Rect src = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        TextureManager::Draw(gameOver, src, dest, SDL_FLIP_NONE);
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