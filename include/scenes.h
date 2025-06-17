#ifndef VINDICTA_SCENES_H
#define VINDICTA_SCENES_H

#include <SDL2/SDL.h>
#include <scenes/mapview.h>
#include <scenes/firstperson.h>

void handle_event(SDL_Event& event, Gamestate& state);

void update(const float& dt, Gamestate& state);

void render(SDL_Renderer* renderer, Gamestate& state);

#endif
