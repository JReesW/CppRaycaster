#include <scenes/firstperson.h>
#include <iostream>


const SDL_Rect ceiling_r {0, 0, settings::SCREENWIDTH, settings::SCREENHEIGHT / 2};
const SDL_Rect floor_r {0, settings::SCREENHEIGHT / 2, settings::SCREENWIDTH, settings::SCREENHEIGHT / 2};


void draw_slice(SDL_Surface* surface, int x, int y_top, int y_bot, SDL_Surface* texture, int dist_along) {
    int y_top_i = maths::clamp(y_top, 0, settings::SCREENHEIGHT);
    int y_bot_i = maths::clamp(y_bot, 0, settings::SCREENHEIGHT);

    Uint32* source = (Uint32*)texture->pixels;
    Uint32* target = (Uint32*)surface->pixels;

    for (int y_target = y_top_i; y_target <= y_bot_i; y_target++) {
        int y_source = std::round(maths::lerp(texture->h, 0, maths::invlerp(y_top, y_bot, y_target)));
        int x_source = dist_along % texture->w;
        target[y_target * surface->w + x] = source[y_source * texture->w + x_source];
    }
}

void render_background(SDL_Renderer* renderer, Gamestate& state) {
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderFillRect(renderer, &ceiling_r);

    SDL_SetRenderDrawColor(renderer, 140, 80, 0, 255);
    SDL_RenderFillRect(renderer, &floor_r);
}

void render_walls(SDL_Surface* surface, const RenderEntry* entry, Gamestate& state) {
    Collision collision = std::get<Collision>(entry->entry);

    // CALCULATE APPARENT DISTANCE AND HEIGHT
    float angle_offset = maths::lerp(-30.0f, 30.0f, (float)collision.column / (float)(settings::SCREENWIDTH - 1));
    float apparent_dist = entry->depth * cosf(maths::deg2rad(angle_offset));
    float apparent_height = (settings::SCREEN_CAM_DIST / apparent_dist) * (float)settings::WALL_HEIGHT;

    // CALCULATE LINE DRAW HEIGHTS AND COLORING
    int half = settings::SCREENHEIGHT / 2;
    float a_h_half = apparent_height / 2;
    float color_factor = maths::clamp(300.0f - (apparent_dist / 2.0f), 0.0f, 255.0f);  // TODO: USE TO SHADE TEXTURES

    // CALCULATE THE DISTANCE ALONG THE HIT MAPLINE, TO DETERMINE WHICH COLUMN OF THE TEXTURE TO USE
    int dist_along = static_cast<int>(std::round(distance(collision.location, collision.target->geometry.start))) * 6;
    draw_slice(surface, collision.column, half - a_h_half, half + a_h_half, state.images[collision.target->image], dist_along);
}

void render_sprites(SDL_Surface* surface, const RenderEntry* entry, Gamestate& state) {\
    Sprite sprite = std::get<Sprite>(entry->entry);
    
    SDL_Surface* image;
    if (sprite.image != state.images["barrel"]) {
        image = state.ticks % 20 < 10 ? state.images["goon_front_1"] : state.images["goon_front_2"];
    } else {
        image = sprite.image;
    } 

    // RELATIVE ANGLE FROM PLAYER TO SPRITE
    point dir = {cos(maths::deg2rad(state.player.angle)), sin(maths::deg2rad(state.player.angle))};
    float angle_offset = get_relative_angle(state.player.position, dir, sprite.position);

    // APPARENT DISTANCE, HEIGHT, AND SCALE
    float apparent_dist = entry->depth * cosf(angle_offset);
    float apparent_height = (settings::SCREEN_CAM_DIST / apparent_dist) * 600.0f;  // TODO: Have this depend on the size of the sprite
    float scale = apparent_height / image->h;

    // CALCULATING THE SPRITE'S BLIT POSITION
    float normalized_angle = angle_offset / maths::deg2rad(settings::FOV / 2);
    float x_offset = ((normalized_angle + 1.0f) * 0.5f) * settings::SCREENWIDTH;
    float left = x_offset - image->w * scale;
    float top = (settings::SCREENHEIGHT / 2.0f) + (apparent_height * scale / 3.0f);

    // DRAW THE SPRITE
    SDL_Rect dest = {(int)left, (int)top, (int)(image->w * scale), (int)(image->h * scale)};
    SDL_BlitScaled(image, NULL, surface, &dest);
    // TODO: Don't blit scaled, just manually draw sprites pixel by pixel.
}


void handle_firstperson(SDL_Event& event, Gamestate& state) {
    handle_mapview(event, state);

    // TURN ON MOUSE MOVEMENT
    if (event.type == SDL_MOUSEMOTION) {
        state.player.angle += event.motion.xrel / 5.0f;
    }

    // DEBUG
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_l) {
            state.debug = true;
        }
    }
}

void update_firstperson(const float& dt, Gamestate& state) {
    // UPDATE THE PLAYER AND RAYCASTING
    move_player(dt, state, true);
    raycast(state);

    // ADD SPRITES TO THE RENDER ENTRIES LIST IF THEY'RE MOSTLY WITHIN THE PLAYER'S VIEW
    for (Sprite& sprite : state.objects) {
        point dir = {cos(maths::deg2rad(state.player.angle)), sin(maths::deg2rad(state.player.angle))};
        float degrees = maths::rad2deg(get_relative_angle(state.player.position, dir, sprite.position));

        if (degrees > -40.0f && degrees < 40.0f) {
            state.renderEntries.insert({distance(state.player.position, sprite.position), sprite});
        }
    }
}

void render_firstperson(SDL_Renderer* renderer, Gamestate& state) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_background(renderer, state);

    // PREPARE SURFACE FOR PIXEL OPERATIONS
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, settings::SCREENWIDTH, settings::SCREENHEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0, 0));
    SDL_LockSurface(surface);

    for (auto i = state.renderEntries.rbegin(); i != state.renderEntries.rend(); ++i) {
        // IF THE ENTRY IS A COLLISION
        if (std::holds_alternative<Collision>(i->entry)) {
            render_walls(surface, &*i, state);
        }else if (std::holds_alternative<Sprite>(i->entry)) {
            SDL_UnlockSurface(surface);
            render_sprites(surface, &*i, state);
            // if (state.debug) SDL_Log("SPRITE");
            SDL_LockSurface(surface);
        }
    }

    // FINISH PIXEL OPERATIONS
    SDL_UnlockSurface(surface);
    SDL_Texture* screenTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_RenderCopy(renderer, screenTexture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(screenTexture);

    SDL_RenderPresent(renderer);
    state.debug = false;
}
