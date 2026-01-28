#pragma once
#include "sceneBase.h"
#include "object.h"
#include <list>
#include <random>

class Game;

typedef struct inputPosition
{
    int x;
    int y;
} Input;

class SceneMain : public SceneBase
{
public:
    SceneMain();
    // ~SceneMain();
    void init() override;
    void update(float dt) override;
    void render() override;
    void handleEvents(SDL_Event *ev) override;
    void clean() override;
    void keyBoardInput();

    void bulletUpdate(float delteTime);
    void bulletRender();

    void enemyUpdate(float delteTime);
    void enemyRender();

    void enemybulletUpdate(float delteTime);
    void enemybulletRender();

private:
    Game &game;
    Player playerMain;
    Input inputPos;
    void areaFilter(SDL_FPoint &src,SDL_FPoint &dst);
    std::list<Bullet*> bulletList;
    Bullet bulletTemplate;

    std::mt19937 generator;
    std::uniform_real_distribution<float> distribution;
    void randomInit();

    std::list<Enemy*> enemyList;
    Enemy enemyTemplate;

    std::list<Enemybullet*> enemyBulletList;
    Enemybullet enemyBulletTemplate;
};
