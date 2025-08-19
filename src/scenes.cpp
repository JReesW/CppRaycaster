#include <scenes.hpp>
#include <iostream>

void handle_event(SDL_Event& event, Gamestate& state) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_q && (event.key.keysym.mod & KMOD_CTRL)) {
            SDL_Log("Quit triggered through Ctrl + Q");
            state.running = false;
        }else if (event.type == SDL_QUIT) {
            SDL_Log("Quit event encountered");
            state.running = false;
        }
    }

    switch (state.scene) {
        case Mapview: handle_mapview(event, state); break;
        case FirstPerson: handle_firstperson(event, state); break;
    }
}

void update(const float& dt, Gamestate& state) {
    switch (state.scene) {
        case Mapview: update_mapview(dt, state); break;
        case FirstPerson: update_firstperson(dt, state); break;
    }
}

void render(SDL_Renderer* renderer, Gamestate& state) {
    switch (state.scene) {
        case Mapview: render_mapview(renderer, state); break;
        case FirstPerson: render_firstperson(renderer, state); break;
    }
}
