#include <scenes/firstperson.h>
#include <iostream>


const SDL_Rect ceiling_r {0, 0, settings::SCREENWIDTH, settings::SCREENHEIGHT / 2};
const SDL_Rect floor_r {0, settings::SCREENHEIGHT / 2, settings::SCREENWIDTH, settings::SCREENHEIGHT / 2};


void draw_slice(SDL_Surface* surface, int x, int y_top, int y_bot, SDL_Surface* texture, int dist_along) {
    // SDL_Log("started slice draw");
    int y_top_i = maths::clamp(y_top, 0, settings::SCREENHEIGHT);
    int y_bot_i = maths::clamp(y_bot, 0, settings::SCREENHEIGHT);
    // SDL_Log("calculated ys");

    Uint32* source = (Uint32*)texture->pixels;
    Uint32* target = (Uint32*)surface->pixels;
    // SDL_Log("loaded pixels");

    for (int y_target = y_top_i; y_target <= y_bot_i; y_target++) {
        int y_source = std::round(maths::lerp(texture->h, 0, maths::invlerp(y_top, y_bot, y_target)));
        int x_source = dist_along % texture->w;
        // SDL_Log("x: %i  |  y_t/b: %ix%i  |  y_target: %i  |  source: (%i, %i)  |  screen: %ix%i  |  texture: %ix%i", x, y_top, y_bot, y_target, x_source, y_source, surface->w, surface->h, texture->w, texture->h);
        target[y_target * surface->w + x] = source[y_source * texture->w + x_source];
    }
}

void render_background(SDL_Renderer* renderer, Gamestate& state) {
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderFillRect(renderer, &ceiling_r);

    SDL_SetRenderDrawColor(renderer, 140, 80, 0, 255);
    SDL_RenderFillRect(renderer, &floor_r);
}

void render_walls(SDL_Renderer* renderer, Gamestate& state) {
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, settings::SCREENWIDTH, settings::SCREENHEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
    SDL_LockSurface(surface);

    for (int x = 0; x < settings::SCREENWIDTH; x++) {
        if (state.collisions.at(x).size() > 0) {
            // TODO: Remove this once auto-sorted collision lists are implemented
            float min_dist = 9999;
            point closest {0, 0};
            Collision closest_c;
            for (Collision c : state.collisions.at(x)) {
                point p = c.location;
                float new_dist = distance(state.player.position, p);
                if (new_dist < min_dist) {
                    min_dist = new_dist;
                    closest = p;
                    closest_c = c;
                }
            }
            // SDL_Log("got closest collision");

            float angle_offset = maths::lerp(-30.0f, 30.0f, (float)x / (float)(settings::SCREENWIDTH - 1));
            float apparent_dist = min_dist * cosf(maths::deg2rad(angle_offset));
            float apparent_height = (settings::SCREEN_CAM_DIST / apparent_dist) * (float)settings::WALL_HEIGHT;

            // SDL_Log("got apparent dimensions");

            int half = settings::SCREENHEIGHT / 2;
            float a_h_half = apparent_height / 2;
            float color_factor = maths::clamp(300.0f - (apparent_dist / 2.0f), 0.0f, 255.0f);

            // SDL_Log("determined color");

            // SDL_SetRenderDrawColor(renderer, 0, 0, (int)color_factor, 255);
            // SDL_RenderDrawLine(renderer, x, half - a_h_half, x, half + a_h_half);
            // TODO: Have collisions store the line they hit, need it for calculating dist_along
            int dist_along = static_cast<int>(std::round(distance(closest_c.location, closest_c.target->geometry.start))) * 4;
            // SDL_Log("calc dist between (%f, %f) and (%f, %f)", closest_c.location.x, closest_c.location.y, closest_c.target->geometry.start.x, closest_c.target->geometry.start.y);
            // SDL_Log("dist along %i", dist_along);
            // SDL_Log("trying %s", closest_c.target->image.c_str());
            draw_slice(surface, x, half - a_h_half, half + a_h_half, state.images[closest_c.target->image], dist_along);
        }
    }

    SDL_UnlockSurface(surface);
    SDL_Texture* screenTexture = SDL_CreateTextureFromSurface(renderer, surface);
    // SDL_RenderClear(renderer);
    // SDL_SetTextureBlendMode(screenTexture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, screenTexture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(screenTexture);
}


void handle_firstperson(SDL_Event& event, Gamestate& state) {
    handle_mapview(event, state);

    if (event.type == SDL_MOUSEMOTION) {
        state.player.angle += event.motion.xrel / 5.0f;
    }
}

void render_firstperson(SDL_Renderer* renderer, Gamestate& state) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_background(renderer, state);
    render_walls(renderer, state);

    SDL_RenderPresent(renderer);
}
