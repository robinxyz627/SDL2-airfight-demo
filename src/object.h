#pragma once
#include <SDL.h>

typedef struct Offset
{
    float x;
    float y;
};

struct Player
{
    SDL_Texture *texture;
    SDL_FPoint pos = {0, 0};
    int width = 0;
    int height = 0;
    int hp = 3;
    int maxHp = 3;
    bool hasShield = false;
    // 伤害
    int damage = 1;
    // 等级
    int level = 1;
    // 子弹类型
    // 子弹列数
    int rows = 1;
    // 子弹间隔ms
    Uint32 shootInterval = 200;
    Uint32 lastShoot = 0;
    // 移动速度
    int speed = 500;
};

struct Bullet
{
    SDL_Texture *texture;
    SDL_FPoint pos = {0, 0};
    Offset offset = {0, 1}; // y default
    int width = 0;
    int height = 0;
    int speed = 300;
    Uint32 lifeTime = 2000;
    Player *owner;
};

struct Enemy
{
    SDL_Texture *texture;
    SDL_FPoint pos = {0, 0};
    SDL_FPoint offset = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 100;
    Uint32 shootInterval = 2000;
    Uint32 lastShoot = 0;
    int damage = 1;
    int hp = 2;
};

struct Enemybullet
{
    SDL_Texture *texture;
    SDL_FPoint pos = {0, 0};
    SDL_FPoint offset = {0, 1}; // y default
    int width = 0;
    int height = 0;
    int speed = 300;
    Enemy *owner;
};

struct Explosion
{
    SDL_Texture *texture;
    SDL_FPoint pos = {0, 0};
    int width = 0;
    int height = 0;
    int currFrame = 0;
    int totalFrame = 0;
    Uint32 startTime = 0;
    Uint32 FPS = 10;
};

enum class ItemType
{
    HEALTH,
    BULLET,
    SHIELD,
    TIME
};
struct BufferItem
{
    SDL_Texture *texture;
    SDL_FPoint pos = {0, 0};
    SDL_FPoint offset = {0, 1};
    int width = 0;
    int height = 0;
    int speed = 50;
    int rebondCounter = 0;
    ItemType type = ItemType::HEALTH;
};

struct BackgroundSprite
{
    SDL_Texture *texture;
    SDL_FPoint pos = {0, 0};
    int width = 0;
    int height = 0;
    int speed = 50;
    float offset = 0;
};
