#ifndef VINDICTA_MAPVIEW_STATE_H
#define VINDICTA_MAPVIEW_STATE_H

#include <SDL2/SDL.h>
#include <settings.h>
#include <gamestate.h>
#include <maths.h>
#include <algorithm>

void render_player(SDL_Renderer* renderer, Player& player);
void render_raycasts(SDL_Renderer* renderer, Gamestate& state);
void switch_view(Gamestate& state);

void handle_mapview(SDL_Event& event, Gamestate& state);
void update_mapview(const float& dt, Gamestate& state);
void render_mapview(SDL_Renderer* renderer, Gamestate& state);

#endif
