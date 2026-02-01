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
    bulletTemplate.owner = &playerMain;

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
    enemyBulletTemplate.owner = &enemyTemplate;

    // effects sprite Atlas
    std::string explosionTexturePath = std::string(PROJECT_ROOT) + "/assets/effect/explosion.png";
    explosionTemplate.texture = IMG_LoadTexture(game.getRenderer(), explosionTexturePath.c_str());
    if (explosionTemplate.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load explosion texture: %s", SDL_GetError());
        exit(1);
    }
    SDL_QueryTexture(explosionTemplate.texture, NULL, NULL, &explosionTemplate.width, &explosionTemplate.height);
    explosionTemplate.totalFrame = explosionTemplate.width / explosionTemplate.height;
    // explosionTemplate.height /= 4;
    explosionTemplate.width = explosionTemplate.height;

    std::string itemTemplateTexturePath = std::string(PROJECT_ROOT) + "/assets/image/bonus_bullets.png";
    itemBulletsTemplate.texture = IMG_LoadTexture(game.getRenderer(), itemTemplateTexturePath.c_str());
    itemTemplateTexturePath = std::string(PROJECT_ROOT) + "/assets/image/bonus_life.png";
    itemHealthTemplate.texture = IMG_LoadTexture(game.getRenderer(), itemTemplateTexturePath.c_str());
    itemTemplateTexturePath = std::string(PROJECT_ROOT) + "/assets/image/bonus_shield.png";
    itemShieldTemplate.texture = IMG_LoadTexture(game.getRenderer(), itemTemplateTexturePath.c_str());
    itemTemplateTexturePath = std::string(PROJECT_ROOT) + "/assets/image/bonus_time.png";
    itemTimeTemplate.texture = IMG_LoadTexture(game.getRenderer(), itemTemplateTexturePath.c_str());
    if (itemBulletsTemplate.texture == nullptr || itemHealthTemplate.texture == nullptr || itemShieldTemplate.texture == nullptr || itemTimeTemplate.texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load BufferItem texture: %s", SDL_GetError());
        exit(1);
    }
    SDL_QueryTexture(itemBulletsTemplate.texture, NULL, NULL, &itemBulletsTemplate.width, &itemBulletsTemplate.height);
    itemBulletsTemplate.width /= 4;
    itemBulletsTemplate.height /= 4;
    SDL_QueryTexture(itemHealthTemplate.texture, NULL, NULL, &itemHealthTemplate.width, &itemHealthTemplate.height);
    itemHealthTemplate.width /= 4;
    itemHealthTemplate.height /= 4;
    SDL_QueryTexture(itemShieldTemplate.texture, NULL, NULL, &itemShieldTemplate.width, &itemShieldTemplate.height);
    itemShieldTemplate.width /= 4;
    itemShieldTemplate.height /= 4;
    SDL_QueryTexture(itemTimeTemplate.texture, NULL, NULL, &itemTimeTemplate.width, &itemTimeTemplate.height);
    itemTimeTemplate.width /= 4;
    itemTimeTemplate.height /= 4;

    // music
    bgm = Mix_LoadMUS((std::string(PROJECT_ROOT) + "/assets/music/03_Racing_Through_Asteroids_Loop.ogg").c_str());
    if (bgm == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load music: %s", SDL_GetError());
        exit(1);
    }
    Mix_PlayMusic(bgm, -1);
    for (const auto &path : soundTypeToPath)
    {
        std::string mPath = std::string(PROJECT_ROOT) + path.second;
        Mix_Chunk *chunk = Mix_LoadWAV(mPath.c_str());
        if (chunk == nullptr)
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load sound: %s", SDL_GetError());
            continue;
        }
        sounds[path.first] = chunk;
    }

    // UI
    std::string uiPath = std::string(PROJECT_ROOT) + "/assets/image/Health UI Black.png";
    uiHealthTexture = IMG_LoadTexture(game.getRenderer(), uiPath.c_str());
    if (uiHealthTexture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load uiHealthTexture: %s", SDL_GetError());
        exit(1);
    }

    uiPath = std::string(PROJECT_ROOT) + "/assets/font/VonwaonBitmap-16px.ttf";
    scoreFont = TTF_OpenFont(uiPath.c_str(), 32);
    if (scoreFont == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load scoreFont: %s", SDL_GetError());
        exit(1);
    }
}

void SceneMain::update(float delteTime)
{
    playerUpdate(delteTime);
    bulletUpdate(delteTime);
    enemyUpdate(delteTime);
    enemybulletUpdate(delteTime);
    effectUpdate(delteTime);
    itemUpdate(delteTime);

    // score
    scoreUpdate(delteTime);
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
    distributionInt = std::uniform_int_distribution<int>(0, 100);
}
void SceneMain::hpChange(int delta)
{
    if (delta < 0)
    {
        if (playerMain.hasShield)
        {
            playerMain.hasShield = false;
            return;
        }
        playerMain.hp = playerMain.hp + delta <= 0 ? 0 : playerMain.hp + delta;
    }
    else
    {
        playerMain.hp = playerMain.hp + delta >= playerMain.maxHp ? playerMain.maxHp : playerMain.hp + delta;
    }
}
void SceneMain::render()
{
    bulletRender();

    playerRender();

    enemyRender();
    enemybulletRender();
    effectRender();
    itemRender();

    // UI on top
    UIRender();
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
    if (playerMain.texture)
    {
        SDL_DestroyTexture(playerMain.texture);
    }
    playerMain.texture = nullptr;

    // bullet clear
    if (bulletTemplate.texture)
    {
        SDL_DestroyTexture(bulletTemplate.texture);
    }
    bulletTemplate.texture = nullptr;
    for (auto bullet : bulletList)
    {
        if (bullet != nullptr)
        {
            delete bullet;
        }
    }
    bulletList.clear();
    // enemy clear
    if (enemyTemplate.texture)
    {
        SDL_DestroyTexture(enemyTemplate.texture);
    }
    enemyTemplate.texture = nullptr;
    for (auto enemy : enemyList)
    {
        if (enemy != nullptr)
        {
            delete enemy;
        }
    }
    enemyList.clear();
    // enemyBullet clear
    if (enemyBulletTemplate.texture)
    {
        SDL_DestroyTexture(enemyBulletTemplate.texture);
    }
    enemyBulletTemplate.texture = nullptr;
    for (auto enemyBullet : enemyBulletList)
    {
        if (enemyBullet != nullptr)
        {
            delete enemyBullet;
        }
    }
    enemyBulletList.clear();
    // explosion clear
    if (explosionTemplate.texture)
    {
        SDL_DestroyTexture(explosionTemplate.texture);
    }
    explosionTemplate.texture = nullptr;
    for (auto explosion : explosionList)
    {
        if (explosion != nullptr)
        {
            delete explosion;
        }
    }

    // item clear
    if (itemBulletsTemplate.texture)
    {
        SDL_DestroyTexture(itemBulletsTemplate.texture);
    }
    itemBulletsTemplate.texture = nullptr;
    if (itemHealthTemplate.texture)
    {
        SDL_DestroyTexture(itemHealthTemplate.texture);
    }
    if (itemShieldTemplate.texture)
    {
        SDL_DestroyTexture(itemShieldTemplate.texture);
    }
    if (itemTimeTemplate.texture)
    {
        SDL_DestroyTexture(itemTimeTemplate.texture);
    }
    for (auto item : itemList)
    {
        if (item != nullptr)
        {
            delete item;
        }
    }
    itemList.clear();

    // mixer clear
    if (bgm != nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
        for (const auto &sound : sounds)
        {
            Mix_FreeChunk(sound.second);
        }
        sounds.clear();
    }

    // UI clear
    if (uiHealthTexture != nullptr)
    {
        SDL_DestroyTexture(uiHealthTexture);
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
void SceneMain::playerUpdate(float delteTime)
{
    if (isDead)
    {
        return;
    }
    if (playerMain.hp <= 0)
    {
        isDead = true;
        auto exp = new Explosion(explosionTemplate);
        exp->pos.x = playerMain.pos.x + explosionTemplate.width / 2 - explosionTemplate.width / 2;
        exp->pos.y = playerMain.pos.y + explosionTemplate.height / 2 - explosionTemplate.height / 2;
        exp->startTime = SDL_GetTicks();
        explosionList.push_back(exp);
        // 爆炸音效
        Mix_PlayChannel(2, sounds[soundType::PLAYER_EXPLOSION], 0);
        return;
    }
    playerMain.pos.x += inputPos.x * playerMain.speed * delteTime;
    playerMain.pos.y += inputPos.y * playerMain.speed * delteTime;
    // 碰撞检测
    SDL_Rect playerRect = {static_cast<int>(playerMain.pos.x), static_cast<int>(playerMain.pos.y), playerMain.width, playerMain.height};
    for (auto enemy : enemyList)
    {
        SDL_Rect enemyRect = {static_cast<int>(enemy->pos.x), static_cast<int>(enemy->pos.y), enemy->width, enemy->height};
        if (SDL_HasIntersection(&enemyRect, &playerRect))
        {
            hpChange(-1);
            enemy->hp = 0;
        }
    }
}
void SceneMain::playerRender()
{
    if (!playerMain.texture)
    {
        return; // 或者输出调试信息
    }
    if (isDead)
    {
        return;
    }
    areaFilter(playerMain.pos, playerMain.pos);
    SDL_Rect rect = {static_cast<int>(playerMain.pos.x), static_cast<int>(playerMain.pos.y), playerMain.width, playerMain.height};
    SDL_RenderCopy(game.getRenderer(), playerMain.texture, NULL, &rect);
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
            bool isErase = false;
            SDL_Rect bulletRect = {static_cast<int>(bullet->pos.x), static_cast<int>(bullet->pos.y), bullet->width, bullet->height};
            for (auto enemy : enemyList)
            {
                SDL_Rect enemyRect = {static_cast<int>(enemy->pos.x), static_cast<int>(enemy->pos.y), enemy->width, enemy->height};
                if (SDL_HasIntersection(&enemyRect, &bulletRect))
                {
                    if (bullet->owner != nullptr && bullet->owner == &playerMain)
                    {
                        enemy->hp -= bullet->owner->damage;
                    }
                    delete bullet;
                    it = bulletList.erase(it);
                    isErase = true;
                    break;
                }
            }
            if (!isErase)
            {
                it++;
            }
        }
    }
    // generate
    if (isDead)
    {
        return;
    }
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
        // 发射音效
        Mix_PlayChannel(-1, sounds[soundType::PLAYER_SHOOT], 0);
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
    auto currTick = SDL_GetTicks();
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
            bool isErase = false;
            if (currTick - enemy->lastShoot >= enemy->shootInterval)
            {
                enemybulletShoot(enemy);
                enemy->lastShoot = currTick;
            }
            if (enemy->hp <= 0)
            {
                // delete enemy;
                enemyExplosion(enemy);
                it = enemyList.erase(it);
                isErase = true;
            }
            if (!isErase)
            {
                it++;
            }
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

void SceneMain::enemyExplosion(Enemy *enemy)
{
    auto currTick = SDL_GetTicks();
    auto exp = new Explosion(explosionTemplate);
    exp->pos.x = enemy->pos.x + enemy->width / 2 - exp->width / 2;
    exp->pos.y = enemy->pos.y + enemy->height / 2 - exp->height / 2;
    exp->startTime = currTick;
    explosionList.push_back(exp);
    // explosion chunk
    Mix_PlayChannel(3, sounds[soundType::ENEMY_EXPLOSION], 0);
    // spawn item
    itemSpawn(enemy);
    // score increase
    score += 10;
    delete enemy;
}

void SceneMain::enemybulletShoot(Enemy *obj)
{
    if (isDead)
    {
        return;
    }
    auto bullet = new Enemybullet(enemyBulletTemplate);
    bullet->pos.x = obj->pos.x + obj->width / 2 - bullet->width / 2;
    bullet->pos.y = obj->pos.y + obj->height / 2 - bullet->height / 2;
    bullet->offset = directionNormalize(obj);
    enemyBulletList.push_back(bullet);

    // 发射音效
    Mix_PlayChannel(1, sounds[soundType::ENEMY_SHOOT], 0);
}

void SceneMain::enemybulletUpdate(float delteTime)
{
    auto margin = 32;
    for (auto it = enemyBulletList.begin(); it != enemyBulletList.end();)
    {
        Enemybullet *bullet = *it;
        bullet->pos.x += bullet->offset.x * bullet->speed * delteTime;
        bullet->pos.y += bullet->offset.y * bullet->speed * delteTime;
        if (bullet->pos.x < -margin || bullet->pos.x > game.getGameWidth() + margin || bullet->pos.y < -margin || bullet->pos.y > game.getGameHeight() + margin)
        {
            delete bullet;
            it = enemyBulletList.erase(it);
        }
        else
        {
            if (isDead)
            {
                it++;
                continue;
            }
            bool isErase = false;
            SDL_Rect bulletRect = {static_cast<int>(bullet->pos.x), static_cast<int>(bullet->pos.y), bullet->width, bullet->height};
            SDL_Rect playerRect = {static_cast<int>(playerMain.pos.x), static_cast<int>(playerMain.pos.y), playerMain.width, playerMain.height};
            if (SDL_HasIntersection(&playerRect, &bulletRect))
            {
                hpChange(-bullet->owner->damage);
                delete bullet;
                it = enemyBulletList.erase(it);
                isErase = true;
            }
            if (!isErase)
            {
                it++;
            }
        }
    }
}

void SceneMain::enemybulletRender()
{
    for (auto bullet : enemyBulletList)
    {
        SDL_Rect rect = {static_cast<int>(bullet->pos.x), static_cast<int>(bullet->pos.y), bullet->width, bullet->height};
        float angle = atan2(bullet->offset.y, bullet->offset.x) * 180 / M_PI - 90;
        // SDL_RenderCopy(game.getRenderer(), bullet->texture, NULL, &rect);
        SDL_RenderCopyEx(game.getRenderer(), bullet->texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
    }
}

SDL_FPoint SceneMain::directionNormalize(Enemy *enemy)
{
    // centre point diff vec2
    auto x = (playerMain.pos.x + playerMain.width / 2) - (enemy->pos.x + enemy->width / 2);
    auto y = (playerMain.pos.y + playerMain.height / 2) - (enemy->pos.y + enemy->height / 2);
    // 求模长
    float length = sqrt(x * x + y * y);
    // 避免除以0
    if (length != 0)
    {
        return SDL_FPoint{x / length, y / length};
    }
    return SDL_FPoint();
}

void SceneMain::effectUpdate(float delteTime)
{
    auto currTick = SDL_GetTicks();
    for (auto it = explosionList.begin(); it != explosionList.end();)
    {
        auto exp = *it;
        auto perFrameTime = 1000 / exp->FPS;
        exp->currFrame = (currTick - exp->startTime) / perFrameTime;
        if (exp->currFrame >= exp->totalFrame)
        {
            delete exp;
            it = explosionList.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void SceneMain::effectRender()
{
    for (auto exp : explosionList)
    {
        SDL_Rect src = {(exp->width) * (exp->currFrame), 0, exp->width, exp->height};
        SDL_Rect dst = {static_cast<int>(exp->pos.x), static_cast<int>(exp->pos.y), exp->width, exp->height};
        SDL_RenderCopy(game.getRenderer(), exp->texture, &src, &dst);
    }
}

void SceneMain::itemSpawn()
{
    // spawn item in screen
    float randomFloat = distribution(generator);       // [0.0 - 1.0)
    int randomInt = static_cast<int>(randomFloat * 4); // [0 - 3]
    ItemType type = static_cast<ItemType>(randomInt);
    BufferItem *item = nullptr;
    switch (type)
    {
    case ItemType::HEALTH:
        item = new BufferItem(itemHealthTemplate);
        break;
    case ItemType::SHIELD:
        item = new BufferItem(itemShieldTemplate);
        break;
    case ItemType::TIME:
        item = new BufferItem(itemTimeTemplate);
        break;
    case ItemType::BULLET:
        item = new BufferItem(itemBulletsTemplate);
        break;

    default:
        item = new BufferItem(itemHealthTemplate);
        break;
    }
    if (item != nullptr)
    {
        item->type = type;
        item->pos.x = distribution(generator) * game.getGameWidth();
        item->pos.y = distribution(generator) * game.getGameHeight();
        // offset x (-1 ~ 1) y (0.5,1)
        item->offset.x = distribution(generator) * 2 - 1;
        item->offset.y = distribution(generator) / 2 + 0.5;
        itemList.push_back(item);
    }
}

void SceneMain::itemSpawn(Enemy *enemy)
{
    // enemy out of screen do not spawn item
    if (enemy->pos.x < -enemy->width || enemy->pos.x > game.getGameWidth() || enemy->pos.y < -enemy->height || enemy->pos.y > game.getGameHeight())
    {
        return;
    }
    if (distribution(generator) < 1 / 5.0f)
    {
        // anything BufferItem may spawn
        float randomFloat = distribution(generator);       // [0.0 - 1.0)
        int randomInt = static_cast<int>(randomFloat * 4); // [0 - 3]
        ItemType type = static_cast<ItemType>(randomInt);
        BufferItem *item = nullptr;
        switch (type)
        {
        case ItemType::HEALTH:
            item = new BufferItem(itemHealthTemplate);
            break;
        case ItemType::SHIELD:
            item = new BufferItem(itemShieldTemplate);
            break;
        case ItemType::TIME:
            item = new BufferItem(itemTimeTemplate);
            break;
        case ItemType::BULLET:
            item = new BufferItem(itemBulletsTemplate);
            break;

        default:
            item = new BufferItem(itemHealthTemplate);
            break;
        }
        if (item != nullptr)
        {
            item->type = type;
            item->pos.x = enemy->pos.x + enemy->width / 2 - item->width / 2;
            item->pos.y = enemy->pos.y + enemy->height / 2 - item->height / 2;
            // offset x (-1 ~ 1) y (0.5,1)
            item->offset.x = distribution(generator) * 2 - 1;
            item->offset.y = distribution(generator) / 2 + 0.5;
            itemList.push_back(item);
        }
    }
}

void SceneMain::itemRebond(BufferItem &item)
{
    // if near screen,rebond
    bool isRebond = false;
    //  up down
    if (item.pos.y < 0 || item.pos.y > game.getGameHeight() - item.height)
    {
        item.offset.y *= -1;
        isRebond = true;
    }
    // left right
    if (item.pos.x < 0 || item.pos.x > game.getGameWidth() - item.width)
    {
        item.offset.x *= -1;
        isRebond = true;
    }
    if (isRebond)
    {
        item.rebondCounter++;
    }
}

void SceneMain::itemUpdate(float delteTime)
{
    for (auto it = itemList.begin(); it != itemList.end();)
    {
        auto item = *it;
        item->pos.x += item->offset.x * item->speed * delteTime;
        item->pos.y += item->offset.y * item->speed * delteTime;
        // if near screen,rebond
        itemRebond(*item);
        if (item->rebondCounter > 5)
        {
            delete item;
            it = itemList.erase(it);
        }
        else
        {
            if (isDead)
            {
                it++;
                continue;
            }
            bool isErase = false;
            SDL_Rect itemRect = {static_cast<int>(item->pos.x), static_cast<int>(item->pos.y), item->width, item->height};
            SDL_Rect playerRect = {static_cast<int>(playerMain.pos.x), static_cast<int>(playerMain.pos.y), playerMain.width, playerMain.height};
            if (SDL_HasIntersection(&playerRect, &itemRect))
            {
                switch (item->type)
                {
                case ItemType::HEALTH:
                    hpChange(1);
                    break;
                case ItemType::SHIELD:
                    playerMain.hasShield = true;
                    break;
                case ItemType::TIME:
                    playerMain.shootInterval *= 0.8;
                case ItemType::BULLET:
                    playerMain.rows += 1;
                }
                delete item;
                it = itemList.erase(it);
                // pick up chunk
                Mix_PlayChannel(4, sounds[soundType::PICKUP], 0);
                // score increase
                score += 20;
                isErase = true;
            }
            if (!isErase)
            {
                it++;
            }
        }
    }
}

void SceneMain::itemRender()
{
    for (auto item : itemList)
    {
        // SDL_Rect src = {0, 0, item->width, item->height};
        SDL_Rect dst = {static_cast<int>(item->pos.x), static_cast<int>(item->pos.y), item->width, item->height};
        SDL_RenderCopy(game.getRenderer(), item->texture, NULL, &dst);
    }
}

void SceneMain::UIRender()
{
    int x = 10;
    int y = 10;
    int size = 32;
    int offset = 48;
    // Uint8 r, g, b, a;
    // SDL_GetRenderDrawColor(game.getRenderer(), &r, &g, &b, &a);
    // SDL_SetRenderDrawColor(game.getRenderer(), 100, 100, 100, 150);
    // SDL_SetRenderDrawBlendMode(game.getRenderer(), SDL_BLENDMODE_BLEND);

    SDL_SetTextureColorMod(uiHealthTexture, 100, 100, 100);

    for (int i = 0; i < playerMain.maxHp; i++)
    {
        SDL_Rect dst = {x + i * offset, y, size, size};
        SDL_RenderCopy(game.getRenderer(), uiHealthTexture, NULL, &dst);
    }
    // SDL_SetRenderDrawColor(game.getRenderer(), r, g, b, a);
    // SDL_SetRenderDrawBlendMode(game.getRenderer(), SDL_BLENDMODE_NONE);
    SDL_SetTextureColorMod(uiHealthTexture, 255, 255, 255);
    for (int i = 0; i < playerMain.hp; i++)
    {
        SDL_Rect dst = {x + i * offset, y, size, size};
        SDL_RenderCopy(game.getRenderer(), uiHealthTexture, NULL, &dst);
    }

    auto label = "SCORE: " + std::to_string(score);
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *text = TTF_RenderUTF8_Solid(scoreFont, label.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(game.getRenderer(), text);
    SDL_Rect dst = {game.getGameWidth() - 10 - text->w, 10, text->w, text->h};
    SDL_RenderCopy(game.getRenderer(), texture, NULL, &dst);
    SDL_FreeSurface(text);
    SDL_DestroyTexture(texture);
}

void SceneMain::scoreUpdate(float)
{
    static Uint32 counter = 0;
    static Uint8 coef = 1;
    static Uint8 counterSecond = 0;
    counter++;
    if (isDead)
    {
        return;
    }
    if (counter >= (game.getGameFps() / 2))
    {
        score += 10 * coef;
        counter = 0;
        counterSecond++;
    }
    if (counterSecond > 10)
    {
        coef += 1;
        counterSecond = 0;
    }
}
