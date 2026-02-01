#pragma once
#include "sceneBase.h"
#include "object.h"
#include <list>
#include <map>
#include <random>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class Game;

typedef struct inputPosition
{
    int x;
    int y;
} Input;

enum class soundType
{
    PLAYER_SHOOT,
    ENEMY_SHOOT,
    PLAYER_EXPLOSION,
    ENEMY_EXPLOSION,
    PICKUP,
    HIT,
};
const std::map<soundType, std::string> soundTypeToPath = {
    {soundType::PLAYER_SHOOT, "/assets/sound/xs_laser.wav"},
    {soundType::ENEMY_SHOOT, "/assets/sound/laser_shoot4.wav"},
    {soundType::PLAYER_EXPLOSION, "/assets/sound/explosion1.wav"},
    {soundType::ENEMY_EXPLOSION, "/assets/sound/explosion3.wav"},
    {soundType::PICKUP, "/assets/sound/eff5.wav"},
    {soundType::HIT, "/assets/sound/eff11.wav"},
};

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

private:
    void keyBoardInput();

    void playerUpdate(float delteTime);
    void playerRender();

    void bulletUpdate(float delteTime);
    void bulletRender();

    void enemyUpdate(float delteTime);
    void enemyRender();
    void enemyExplosion(Enemy *enemy);

    void enemybulletShoot(Enemy *obj);
    void enemybulletUpdate(float delteTime);
    void enemybulletRender();
    SDL_FPoint directionNormalize(Enemy *enemy);

    void effectUpdate(float delteTime);
    void effectRender();

    void itemSpawn();
    void itemSpawn(Enemy *enemy);
    void itemRebond(BufferItem &item);
    void itemUpdate(float delteTime);
    void itemRender();

    void UIRender();
    void scoreUpdate(float delteTime);

private:
    Game &game;
    Mix_Music *bgm;
    Player playerMain;
    Input inputPos;
    void areaFilter(SDL_FPoint &src, SDL_FPoint &dst);
    std::list<Bullet *> bulletList;
    Bullet bulletTemplate;

    std::mt19937 generator;
    std::uniform_real_distribution<float> distribution;
    std::uniform_int_distribution<int> distributionInt;
    void randomInit();

    std::list<Enemy *> enemyList;
    Enemy enemyTemplate;

    std::list<Enemybullet *> enemyBulletList;
    Enemybullet enemyBulletTemplate;

    std::list<Explosion *> explosionList;
    Explosion explosionTemplate;

    std::list<BufferItem *> itemList;
    BufferItem itemBulletsTemplate;
    BufferItem itemHealthTemplate;
    BufferItem itemShieldTemplate;
    BufferItem itemTimeTemplate;

    std::map<soundType, Mix_Chunk *> sounds;

    // status
    bool isDead = false;

    // PlayerManager
    void hpChange(int delta);

    // UI
    int score = 0;
    SDL_Texture *uiHealthTexture;
    TTF_Font *scoreFont;
};
