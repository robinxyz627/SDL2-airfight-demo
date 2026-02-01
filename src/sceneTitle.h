#include "sceneBase.h"
#include "game.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
class SceneTitle : public SceneBase
{
public:
    SceneTitle() : game(Game::getInstance()) {}
    virtual void clean() override;
    virtual void handleEvents(SDL_Event *ev) override;
    virtual void init() override;
    virtual void render() override;
    virtual void update(float dt) override;

private:
    Game &game;
    Mix_Music *bgm;
    TTF_Font *title;
    TTF_Font *tip;

    // tools
    bool isTipAppear = true;
    void randerTextrelativeCenter(const char *text, TTF_Font *font, SDL_Color color, float y);
};