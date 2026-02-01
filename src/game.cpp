#include "game.h"
#include "sceneMain.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
Game::Game()
{
}

Game &Game::getInstance()
{
    static Game instance;
    return instance;
}

Game::~Game()
{
    clean();
}

void Game::run()
{
    while (is_running)
    {
        if (currScene != nullptr)
        {
            auto frameStart = SDL_GetTicks();
            SDL_Event ev;
            // 处理事件
            handleEvents(&ev);
            // 更新
            update(deltaTime);
            // 渲染
            render();
            // log
            gameLog();
            auto frameDiff = SDL_GetTicks() - frameStart;
            if (frameDiff < frameTimer)
            {
                SDL_Delay(frameTimer - frameDiff);
                deltaTime = frameTimer / 1000.0f;
            }
            else
            {
                deltaTime = frameDiff / 1000.0f;
            }
        }
    }
    clean();
    std::cout << "please init first" << std::endl;
}

void Game::init()
{
    frameTimer = 1000 / fps;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL: %s\n", SDL_GetError());
        is_running = false;
        return;
    }
    if (!gameWidth || !gameHeight)
    {
        is_running = false;
        return;
    }
    window = SDL_CreateWindow("飞机大战", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              gameWidth, gameHeight,
                              SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't create window: %s\n", SDL_GetError());
        is_running = false;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't create renderer: %s\n", SDL_GetError());
        is_running = false;
        return;
    }
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL_image: %s\n", IMG_GetError());
        is_running = false;
        return;
    }

    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL_mixer: %s\n", Mix_GetError());
        is_running = false;
        return;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't open audio: %s\n", Mix_GetError());
        is_running = false;
        return;
    }
    Mix_AllocateChannels(32);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_Volume(-1, MIX_MAX_VOLUME / 8);

    // bg init
    std::string bgPath = std::string(PROJECT_ROOT) + "/assets/image/Stars-A.png";
    nearBg.texture = IMG_LoadTexture(renderer, bgPath.c_str());
    bgPath = std::string(PROJECT_ROOT) + "/assets/image/Stars-B.png";
    farBg.texture = IMG_LoadTexture(renderer, bgPath.c_str());
    if (nearBg.texture == nullptr || farBg.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't load background image: %s\n", SDL_GetError());
        is_running = false;
        return;
    }
    SDL_QueryTexture(nearBg.texture, NULL, NULL, &nearBg.width, &nearBg.height);
    nearBg.width /= 2;
    nearBg.height /= 2;
    nearBg.speed = 20;
    SDL_QueryTexture(farBg.texture, NULL, NULL, &farBg.width, &farBg.height);
    farBg.width /= 4;
    farBg.height /= 4;
    farBg.speed = 10;

    // TTF
    if (TTF_Init() != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to init TTF: %s", SDL_GetError());
        is_running = false;
        return;
    }

    is_running = true;
    SceneMain *ms = new SceneMain(); // model
    changeScene(ms);
}

void Game::clean()
{
    if (currScene != nullptr)
    {
        currScene->clean();
        delete currScene;
    }
    if (renderer != nullptr)
        SDL_DestroyRenderer(renderer);
    if (window != nullptr)
        SDL_DestroyWindow(window);

    if (nearBg.texture != nullptr)
    {
        SDL_DestroyTexture(nearBg.texture);
    }
    if (farBg.texture != nullptr)
    {
        SDL_DestroyTexture(farBg.texture);
    }

    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

void Game::changeScene(SceneBase *newScene)
{
    if (currScene != nullptr)
    {
        currScene->clean();
        delete currScene;
    }
    currScene = newScene;
    currScene->init();
}

void Game::handleEvents(SDL_Event *ev)
{
    while (SDL_PollEvent(ev))
    {
        if (ev->type == SDL_QUIT)
        {
            is_running = false;
        }
        currScene->handleEvents(ev);
    }
}

void Game::update(float dt)
{
    bgUpdate(dt);
    currScene->update(dt);
}

void Game::render()
{
    SDL_RenderClear(renderer);
    bgRender();
    currScene->render();
    SDL_RenderPresent(renderer);
}

void Game::logLevel()
{
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
}

void Game::gameLog()
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "tick", SDL_GetTicks());
}

void Game::bgUpdate(float dt)
{
    nearBg.offset += nearBg.speed * dt;
    if (nearBg.offset >= 0)
    {
        nearBg.offset -= nearBg.height;
    }
    farBg.offset += farBg.speed * dt;
    if (farBg.offset >= 0)
    {
        farBg.offset -= farBg.height;
    }
}

void Game::bgRender()
{
    for (int posY = static_cast<int>(nearBg.offset); posY < getGameHeight(); posY += nearBg.height)
    {
        for (int posX = 0; posX < getGameWidth(); posX += nearBg.width)
        {
            SDL_Rect dst = {posX, posY, nearBg.width, nearBg.height};
            SDL_RenderCopy(renderer, nearBg.texture, NULL, &dst);
        }
    }

    for (int posY = static_cast<int>(farBg.offset); posY < getGameHeight(); posY += farBg.height)
    {
        for (int posX = 0; posX < getGameWidth(); posX += farBg.width)
        {
            SDL_Rect dst = {posX, posY, farBg.width, farBg.height};
            SDL_RenderCopy(renderer, farBg.texture, NULL, &dst);
        }
    }
}
