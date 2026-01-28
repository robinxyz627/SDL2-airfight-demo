#pragma once
#include <SDL.h>
class SceneBase
{
public:
    SceneBase() = default;
    virtual ~SceneBase() = default;

    virtual void init() = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual void handleEvents(SDL_Event* ev) = 0;
    virtual void clean() = 0;
};