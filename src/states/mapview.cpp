#include <scenes/mapview.h>
#include <iostream>

////////////////////////
// SPECIFIC FUNCTIONS //
////////////////////////

void render_player(SDL_Renderer* renderer, Player& player) {
    point direction = point{5.0, 0.0}.rotate(player.angle);
    point start = player.position - direction;  // {player.position.x - direction.x, player.position.y - direction.y};
    point end = player.position + direction;  // {player.position.x + direction.x, player.position.y + direction.y};
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
}

void render_raycasts(SDL_Renderer* renderer, Gamestate& state) {
    for (int x = 0; x < settings::SCREENWIDTH; x += 10) {
        // TODO: Remove this once auto-sorted collision lists are implemented
        float min_dist = 9999;
        point closest {0, 0};
        for (Collision c : state.collisions.at(x)) {
            point p = c.location;
            float new_dist = distance(state.player.position, p);
            if (new_dist < min_dist) {
                min_dist = new_dist;
                closest = p;
            }
        }
        SDL_RenderDrawLine(renderer, state.player.position.x, state.player.position.y, closest.x, closest.y);
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
    // MOVE
    auto [nx, ny] = state.player.position;
    if (state.keys & Key_W) {
        nx += state.player.speed * cos(maths::deg2rad(state.player.angle));
        ny += state.player.speed * sin(maths::deg2rad(state.player.angle));
    }
    if (state.keys & Key_S) {
        nx -= state.player.speed * cos(maths::deg2rad(state.player.angle));
        ny -= state.player.speed * sin(maths::deg2rad(state.player.angle));
    }

    // CHECK IF MOVEMENT DOESN'T COLLIDE
    bool no_collision = true;
    for (mapline ml : state.map) {
        if (line_circle_intersect(ml.geometry, point{nx, ny}, 10)) {
            no_collision = false;
            break;
        }
    }
    if (no_collision) {
        state.player.position.x = nx;
        state.player.position.y = ny;
    }

    // ROTATE
    if (state.keys & Key_A) {
        state.player.angle -= 4.0;
    }
    if (state.keys & Key_D) {
        state.player.angle += 4.0;
    }

    // RAYCAST
    for (int x = 0; x < settings::SCREENWIDTH; x++) {
        // CLEAR COLLISIONS FOR THIS RAY
        state.collisions.at(x).clear();

        // CREATE THE RAY
        float angle = maths::lerp(-30.0f, 30.0f, (float)x / (float)(settings::SCREENWIDTH - 1));
        point raydir = point{1, 0}.rotate(state.player.angle + angle) * 1000.0;
        line ray = {state.player.position, state.player.position + raydir};
        
        // CHECK COLLISIONS
        for (mapline& ml : state.map) {
            std::optional<point> i = intersection(ray, ml.geometry);
            if (i != std::nullopt) {
                state.collisions.at(x).push_back({i.value(), &ml});
            }
        }
    }
}

void render_mapview(SDL_Renderer* renderer, Gamestate& state) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    // MAP LINES
    for (mapline ml : state.map) {
        line& l = ml.geometry;
        SDL_RenderDrawLine(renderer, l.start.x, l.start.y, l.end.x, l.end.y);
    }

    // RAYCASTING
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    render_raycasts(renderer, state);

    // PLAYER
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    render_player(renderer, state.player);

    SDL_RenderPresent(renderer);
}
