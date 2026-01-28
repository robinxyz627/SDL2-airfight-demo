#pragma once
#include <SDL.h>

typedef struct Offset
{
    float x;
    float y;
};


struct Player
{
    SDL_Texture* texture;
    SDL_FPoint pos = { 0,0 };
    int width = 0;
    int height = 0;
    //伤害
    int damage = 1;
    //等级
    int level = 1;
    //子弹类型
    //子弹列数
    int rows = 5;
    //子弹间隔ms
    Uint32 shootInterval = 200;
    Uint32 lastShoot = 0;
    //移动速度
    int speed = 5;
};

struct Bullet
{
    SDL_Texture* texture;
    SDL_FPoint pos = { 0,0 };
    Offset offset = { 0,1 };//y default
    int width = 0;
    int height = 0;
    int speed = 300;
    Uint32 lifeTime = 2000;
};

struct Enemy
{
    SDL_Texture* texture;
    SDL_FPoint pos = { 0,0 };
    SDL_FPoint offset = { 0,0 };
    int width = 0;
    int height = 0;
    int speed = 100;
};

struct Enemybullet
{
    SDL_Texture* texture;
    SDL_FPoint pos = { 0,0 };
    SDL_FPoint offset = { 0,1 };//y default
    int width = 0;
    int height = 0;
    int speed = 300;
};