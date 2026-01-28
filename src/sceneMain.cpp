#include "sceneMain.h"
#include "game.h"
#include "SDL.h"
#include "SDL_image.h"
#include <string>

SceneMain::SceneMain() : game(Game::getInstance())
{
    inputPos = {0, 0};
}

void SceneMain::init()
{
    // random init
    randomInit();

    std::string texturePath = std::string(PROJECT_ROOT) + "/assets/image/SpaceShip.png";
    playerMain.texture = IMG_LoadTexture(game.getRenderer(), texturePath.c_str());
    if (playerMain.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load texture: %s", SDL_GetError());
        exit(1);
    }
    SDL_QueryTexture(playerMain.texture, NULL, NULL, &playerMain.width, &playerMain.height);
    playerMain.width /= 4;
    playerMain.height /= 4;
    playerMain.pos.x = static_cast<float>(game.getGameWidth() / 2 - playerMain.width / 2);
    playerMain.pos.y = static_cast<float>(game.getGameHeight() - playerMain.height);

    // load到模板
    std::string bulletTexturePath = std::string(PROJECT_ROOT) + "/assets/image/laser-1.png";
    bulletTemplate.texture = IMG_LoadTexture(game.getRenderer(), bulletTexturePath.c_str());
    if (bulletTemplate.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load bullet texture: %s", SDL_GetError());
        exit(1);
    }
    SDL_QueryTexture(bulletTemplate.texture, NULL, NULL, &bulletTemplate.width, &bulletTemplate.height);
    bulletTemplate.width /= 4;
    bulletTemplate.height /= 4;

    std::string enemyTexturePath = std::string(PROJECT_ROOT) + "/assets/image/insect-1.png";
    enemyTemplate.texture = IMG_LoadTexture(game.getRenderer(), enemyTexturePath.c_str());
    if (enemyTemplate.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load enemy texture: %s", SDL_GetError());
        exit(1);
    }
    SDL_QueryTexture(enemyTemplate.texture, NULL, NULL, &enemyTemplate.width, &enemyTemplate.height);
    enemyTemplate.width /= 4;
    enemyTemplate.height /= 4;

    std::string enemyBulletTexturePath = std::string(PROJECT_ROOT) + "/assets/image/bullet-1.png";
    enemyBulletTemplate.texture = IMG_LoadTexture(game.getRenderer(), enemyBulletTexturePath.c_str());
    if (enemyBulletTemplate.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load enemy bullet texture: %s", SDL_GetError());
        exit(1);
    }
    SDL_QueryTexture(enemyBulletTemplate.texture, NULL, NULL, &enemyBulletTemplate.width, &enemyBulletTemplate.height);
    enemyBulletTemplate.width /= 4;
    enemyBulletTemplate.height /= 4;
}

void SceneMain::update(float delteTime)
{
    playerMain.pos.x += inputPos.x * playerMain.speed;
    playerMain.pos.y += inputPos.y * playerMain.speed;
    bulletUpdate(delteTime);
    enemyUpdate(delteTime);
}
void SceneMain::areaFilter(SDL_FPoint &src, SDL_FPoint &dst)
{
    if (src.x < 0)
    {
        dst.x = 0;
    }
    else if (src.x > game.getGameWidth() - playerMain.width)
    {
        dst.x = game.getGameWidth() - playerMain.width;
    }
    else
    {
        dst.x = src.x; // 如果在边界内，保留原值
    }

    // 限制Y坐标在合理范围内
    if (src.y < 0)
    {
        dst.y = 0;
    }
    else if (src.y > game.getGameHeight() - playerMain.height)
    {
        dst.y = game.getGameHeight() - playerMain.height;
    }
    else
    {
        dst.y = src.y; // 如果在边界内，保留原值
    }
}
void SceneMain::randomInit()
{
    std::random_device rd;
    generator = std::mt19937(rd());
    distribution = std::uniform_real_distribution<float>(0.0f, 1.0f);
}
void SceneMain::render()
{
    if (!playerMain.texture)
    {
        return; // 或者输出调试信息
    }
    bulletRender();

    areaFilter(playerMain.pos, playerMain.pos);
    SDL_Rect rect = {static_cast<int>(playerMain.pos.x), static_cast<int>(playerMain.pos.y), playerMain.width, playerMain.height};
    SDL_RenderCopy(game.getRenderer(), playerMain.texture, NULL, &rect);

    enemyRender();
}

void SceneMain::handleEvents(SDL_Event *ev)
{
    if (ev->type == SDL_KEYDOWN || ev->type == SDL_KEYUP)
    {
        keyBoardInput();
    }
}

void SceneMain::clean()
{
    for (auto bullet : bulletList)
    {
        if (bullet != nullptr)
        {
            delete bullet;
        }
    }
    bulletList.clear();

    if (playerMain.texture)
    {
        SDL_DestroyTexture(playerMain.texture);
    }
    playerMain.texture = nullptr;

    // clear template
    if (bulletTemplate.texture)
    {
        SDL_DestroyTexture(bulletTemplate.texture);
    }
}

void SceneMain::keyBoardInput()
{
    // clear
    inputPos = {0, 0};
    auto keyArry = SDL_GetKeyboardState(NULL);
    if (keyArry[SDL_SCANCODE_A] || keyArry[SDL_SCANCODE_LEFT])
    {
        inputPos.x = -1;
    }
    if (keyArry[SDL_SCANCODE_D] || keyArry[SDL_SCANCODE_RIGHT])
    {
        inputPos.x = 1;
    }
    if (keyArry[SDL_SCANCODE_W] || keyArry[SDL_SCANCODE_UP])
    {
        inputPos.y = -1;
    }
    if (keyArry[SDL_SCANCODE_S] || keyArry[SDL_SCANCODE_DOWN])
    {
        inputPos.y = 1;
    }
}
void SceneMain::bulletUpdate(float delteTime)
{
    // update
    for (auto it = bulletList.begin(); it != bulletList.end();)
    {
        Bullet *bullet = *it;
        bullet->pos.y -= bullet->speed * delteTime;
        bullet->lifeTime -= delteTime * 1000;

        if (bullet->lifeTime <= 0)
        {
            delete bullet;
            it = bulletList.erase(it);
        }
        else
        {
            it++;
        }
    }
    // generate
    Uint32 currTick = SDL_GetTicks();
    if (currTick - playerMain.lastShoot >= playerMain.shootInterval)
    {
        playerMain.lastShoot = currTick;
        int rows = playerMain.rows;
        int intervalPixel = bulletTemplate.width * (rows - 1);
        for (int i = 0; i < rows; i++)
        {
            // generate bullet
            auto bullet = new Bullet(bulletTemplate);
            // init pos
            bullet->pos.x = playerMain.pos.x + playerMain.width / 2 - bullet->width / 2 - intervalPixel / 2 + i * bulletTemplate.width;
            bullet->pos.y = playerMain.pos.y;
            bulletList.push_back(bullet);
        }
    }
}

void SceneMain::bulletRender()
{
    for (auto bullet : bulletList)
    {
        SDL_Rect rect = {static_cast<int>(bullet->pos.x), static_cast<int>(bullet->pos.y), bullet->width, bullet->height};
        SDL_RenderCopy(game.getRenderer(), bullet->texture, NULL, &rect);
    }
}

void SceneMain::enemyUpdate(float delteTime)
{
    // updata enemy pos/status
    for (auto it = enemyList.begin(); it != enemyList.end();)
    {
        Enemy *enemy = *it;
        enemy->pos.y += enemy->speed * delteTime;
        enemy->pos.x += enemy->offset.x * enemy->speed * delteTime;
        if (enemy->pos.y > game.getGameHeight() || enemy->pos.x > game.getGameWidth() || enemy->pos.x < 0 - enemy->width)
        {
            delete enemy;
            it = enemyList.erase(it);
        }
        else
        {
            it++;
        }
    }
    if (distribution(generator) < 1 / 60.0f)
    {
        // todo generate enemy
        auto enemy = new Enemy(enemyTemplate);
        // random birth pos
        enemy->pos.x = distribution(generator) * (game.getGameWidth() - enemy->width);
        // random x offset (-1,1) make sure enemy will not out of screen
        if (enemy->pos.x < game.getGameWidth() / 2)
        {
            //(0,1)
            enemy->offset.x = distribution(generator) / 2;
        }
        else
        {
            enemy->offset.x = -distribution(generator) / 2;
        }
        enemy->pos.y = static_cast<float>(-enemy->height);
        enemyList.push_back(enemy);
    }
}

void SceneMain::enemyRender()
{
    for (auto enemy : enemyList)
    {
        SDL_Rect rect = {static_cast<int>(enemy->pos.x), static_cast<int>(enemy->pos.y), enemy->width, enemy->height};
        SDL_RenderCopy(game.getRenderer(), enemy->texture, NULL, &rect);
    }
}

void SceneMain::enemybulletUpdate(float delteTime)
{
    for(auto it = enemyBulletList.begin(); it != enemyBulletList.end();){

    }
}

void SceneMain::enemybulletRender()
{
}
