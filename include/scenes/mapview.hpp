#ifndef VINDICTA_MAPVIEW_STATE_H
#define VINDICTA_MAPVIEW_STATE_H

#include <SDL2/SDL.h>
#include <algorithm>
#include <set>

#include <settings.hpp>
#include <types.hpp>
#include <lines.hpp>
#include <player.hpp>
#include <raycast.hpp>
#include <maths.hpp>


void render_player(SDL_Renderer* renderer, Player& player);
void render_objects(SDL_Renderer* renderer, Gamestate& state);
void switch_view(Gamestate& state);

void handle_mapview(SDL_Event& event, Gamestate& state);
void update_mapview(const float& dt, Gamestate& state);
void render_mapview(SDL_Renderer* renderer, Gamestate& state);

#endif
