#include <iostream>
// #include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
int main(int, char **)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 720, SDL_WINDOW_SHOWN);
    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::cout << "SDL_Init Success" << std::endl;

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (IMG_Init(imgFlags) != imgFlags)
    {
        std::cout << "IMG_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Texture *texture = IMG_LoadTexture(render, "assets/image/bg.png");

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        std::cout << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
    }
    Mix_Music *music = Mix_LoadMUS("assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    Mix_PlayMusic(music, -1);

    //文本创建
    if(TTF_Init() != 0){
        std::cout << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return 1;
    }
    TTF_Font *font = TTF_OpenFont("assets/font/VonwaonBitmap-12px.ttf", 24);
    SDL_Color color = {255, 255, 255};
    SDL_Surface *textSurface = TTF_RenderUTF8_Solid(font, "我爱中国！", color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(render, textSurface);

    while (true)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                return 0;
            }
        }
        SDL_RenderClear(render);
        SDL_Rect rect = {0, 0, 200, 200};
        SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
        SDL_RenderFillRect(render, &rect);
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);

        // 纹理
        SDL_Rect rectPhone = {120, 0, 480, 720};
        SDL_RenderCopy(render, texture, NULL, &rectPhone);

        //文本纹理
        SDL_Rect rectText = {120, 240,textSurface->w, textSurface->h};
        SDL_RenderCopy(render, textTexture, NULL, &rectText);

        SDL_RenderPresent(render);
    }
    //destory IMG
    SDL_DestroyTexture(texture);
    IMG_Quit();

    // destory MIX
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    Mix_Quit();

    // destory TTF
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}