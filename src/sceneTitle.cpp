#pragma once
#include "sceneTitle.h"
#include "sceneMain.h"
#include <string>

void SceneTitle::clean()
{
    if (title != nullptr)
    {
        TTF_CloseFont(title);
    }
    if (tip != nullptr)
    {
        TTF_CloseFont(tip);
    }
    if (bgm != nullptr)
    {
        Mix_FreeMusic(bgm);
    }
}

void SceneTitle::handleEvents(SDL_Event *ev)
{
    if (ev->type == SDL_KEYDOWN)
    {
        if (ev->key.keysym.scancode == SDL_SCANCODE_SPACE)
        {
            game.changeScene(new SceneMain());
        }
    }
}

void SceneTitle::init()
{
    // ttf load
    std::string ttfPath = std::string(PROJECT_ROOT) + "/assets/font/VonwaonBitmap-16px.ttf";
    title = TTF_OpenFont(ttfPath.c_str(), 64);
    tip = TTF_OpenFont(ttfPath.c_str(), 32);
    if (title == nullptr || tip == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "TTF_OpenFont: %s", TTF_GetError());
        exit(1);
    }

    // music
    bgm = Mix_LoadMUS((std::string(PROJECT_ROOT) + "/assets/music/06_Battle_in_Space_Intro.ogg").c_str());
    if (bgm == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load music: %s", SDL_GetError());
        exit(1);
    }
    Mix_PlayMusic(bgm, -1);
}

void SceneTitle::render()
{
    SDL_Color color = {255, 255, 255, 255};
    std::string text = "SDL2太空飞机大战";
    randerTextrelativeCenter(text.c_str(), title, color, 0.4);

    text = "按SPACE键开始游戏";
    if (isTipAppear)
    {
        randerTextrelativeCenter(text.c_str(), tip, color, 0.6);
    }
}

void SceneTitle::update(float dt)
{
    // tip flash
    static float counter = 0;
    counter += dt;
    if (counter >= 0.5)
    {
        counter = 0;
        isTipAppear = !isTipAppear;
    }
}

void SceneTitle::randerTextrelativeCenter(const char *text, TTF_Font *font, SDL_Color color, float y)
{

    SDL_Surface *textSurface = TTF_RenderUTF8_Solid(font, text, color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(game.getRenderer(), textSurface);
    int posY = game.getGameHeight() * y < game.getGameHeight() ? static_cast<int>((game.getGameHeight() - textSurface->h) * y) : game.getGameHeight();
    SDL_Rect textRect = {game.getGameWidth() / 2 - textSurface->w / 2, posY, textSurface->w, textSurface->h};

    SDL_RenderCopy(game.getRenderer(), textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}
