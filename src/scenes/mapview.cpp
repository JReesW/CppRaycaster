#include <scenes/mapview.h>
#include <iostream>

////////////////////////
// SPECIFIC FUNCTIONS //
////////////////////////

void render_player(SDL_Renderer* renderer, Player& player) {
    Point direction = Point{5.0, 0.0}.rotate(player.angle);
    Point start = player.position - direction;  // {player.position.x - direction.x, player.position.y - direction.y};
    Point end = player.position + direction;  // {player.position.x + direction.x, player.position.y + direction.y};
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
    direction = direction.rotate(135.0f);
    Point end1 = end + direction;
    SDL_RenderDrawLine(renderer, end.x, end.y, end1.x, end1.y);
    direction = direction.rotate(90.0f);
    end1 = end + direction;
    SDL_RenderDrawLine(renderer, end.x, end.y, end1.x, end1.y);
}

void render_objects(SDL_Renderer* renderer, Gamestate& state) {
    for (Sprite* sprite : state.get_sprites()) {
        auto [x, y] = sprite->position;
        SDL_RenderDrawLine(renderer, x - 10, y - 10, x + 10, y + 10);
        SDL_RenderDrawLine(renderer, x - 10, y + 10, x + 10, y - 10);
    }
}

void switch_view(Gamestate& state) {
    state.scene = state.scene == FirstPerson ? Mapview : FirstPerson;
    SDL_SetRelativeMouseMode(state.scene == FirstPerson ? SDL_TRUE : SDL_FALSE);
}

///////////////////////
// GENERIC FUNCTIONS //
///////////////////////

void handle_mapview(SDL_Event& event, Gamestate& state) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_w: state.keys |= Key_W; break;
            case SDLK_a: state.keys |= Key_A; break;
            case SDLK_s: state.keys |= Key_S; break;
            case SDLK_d: state.keys |= Key_D; break;

            case SDLK_m: switch_view(state);
        }
    } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_w: state.keys &= ~Key_W; break;
            case SDLK_a: state.keys &= ~Key_A; break;
            case SDLK_s: state.keys &= ~Key_S; break;
            case SDLK_d: state.keys &= ~Key_D; break;
        }
    }
}

void update_mapview(const float& dt, Gamestate& state) {
    move_player(dt, state, false);

    // ROTATE
    if (state.keys & Key_A) {
        state.player.angle -= 4.0;
    }
    if (state.keys & Key_D) {
        state.player.angle += 4.0;
    }
}

void render_mapview(SDL_Renderer* renderer, Gamestate& state) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    // MAP LINES
    for (Mapline ml : state.map) {
        Line& l = ml.line;
        SDL_RenderDrawLine(renderer, l.start.x, l.start.y, l.end.x, l.end.y);
    }

    // RAYCASTING
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    // PLAYER
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    render_player(renderer, state.player);
    render_objects(renderer, state);

    SDL_RenderPresent(renderer);
}
