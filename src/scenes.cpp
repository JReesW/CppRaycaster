#include <scenes.h>
#include <iostream>

void handle_event(SDL_Event& event, Gamestate& state) {
    if (event.key.keysym.sym == SDLK_q && (event.key.keysym.mod & KMOD_CTRL)) {
        state.running = false;
    }else if (event.type == SDL_QUIT) {
        std::cout << "quit event" << std::endl;
        state.running = false;
    }

    switch (state.scene) {
        case Mapview: handle_mapview(event, state); break;
        case FirstPerson: handle_firstperson(event, state); break;
    }
}

void update(const float& dt, Gamestate& state) {
    switch (state.scene) {
        case Mapview: case FirstPerson:
            update_mapview(dt, state); break;
    }
}

void render(SDL_Renderer* renderer, Gamestate& state) {
    switch (state.scene) {
        case Mapview: render_mapview(renderer, state); break;
        case FirstPerson: render_firstperson(renderer, state); break;
    }
}
