#pragma once

#include "sceneBase.h"
#include "object.h"
// singleton
class Game
{
public:
    static Game &getInstance();
    ~Game();
    void run();
    void init();
    void clean();
    void changeScene(SceneBase *newScene);

    void handleEvents(SDL_Event *ev);
    void update(float dt);
    void render();
    void logLevel();

    SDL_Renderer *getRenderer() { return renderer; }
    SDL_Window *getWindow() {return window;}
    int getGameWidth() { return gameWidth; }
    int getGameHeight() { return gameHeight; }

    int getGameFps() { return fps; }

private:
    Game();
    //删除拷贝和复制构造函数
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;
    void gameLog();
    bool is_running = false;
    SceneBase *currScene = nullptr;
    float dt = 0.016f;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    int gameWidth = 600;
    int gameHeight = 800;
    int fps = 60;
    Uint32 frameTimer;
    float deltaTime;

    //bg
    BackgroundSprite nearBg;
    BackgroundSprite farBg;
    void bgUpdate(float dt);
    void bgRender();

};