#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include <settings.h>
#include <types.h>
#include <scenes.h>
#include <load.h>

template<typename T>
bool errored(T* ptr, const char* func) {
    if (ptr == nullptr) {
        std::cerr << func << " Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return true;
    }
    return false;
}

int main(int argc, char* argv[]) {

    // SETUP SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* win = SDL_CreateWindow("Wolfenstein",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       settings::SCREENWIDTH,
                                       settings::SCREENHEIGHT,
                                       SDL_WINDOW_SHOWN);
    if (errored(win, "SDL_CreateWindow")) return 1;
    SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP);

    SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, 0);
    if (errored(renderer, "SDL_CreateRenderer")) return 1;
    SDL_RenderSetLogicalSize(renderer, settings::SCREENWIDTH, settings::SCREENHEIGHT);

    Gamestate state {
        // RUNNING
        true,
        
        // SCENE
        Mapview,

        // KEYS
        Key_None,
        
        // PLAYER
        {{50, 240}, 0}
    };

    load_level("test", state);


    // GAME LOOP
    unsigned int a = 0;
    unsigned int b = 0;
    unsigned int delta = 0;

    while (state.running) {
        // EVENTS
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            handle_event(event, state);
        }

        // DELTA-TIME
        a = SDL_GetTicks();
        delta = a - b;

        if (delta > 1000 / (float)settings::FPS) {
            b = a;
            update(delta, state);
            render(renderer, state);
            state.ticks += 1;
        }
    }

    SDL_Log("Game stopped running");

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
