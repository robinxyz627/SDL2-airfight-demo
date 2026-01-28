#include "game.h"
#include "sceneMain.h"
#include <SDL.h>
#include <SDL_image.h>
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
            }else{
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
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Couldn't initialize SDL_image: %s\n", IMG_GetError());
        is_running = false;
        return;
    }
    is_running = true;
    SceneMain *ms = new SceneMain();//model
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


    IMG_Quit();
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
    currScene->update(dt);
}

void Game::render()
{
    SDL_RenderClear(renderer);
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
