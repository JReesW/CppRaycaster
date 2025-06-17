#ifndef VINDICTA_FIRSTPERSON_H
#define VINDICTA_FIRSTPERSON_H

#include <SDL2/SDL.h>
#include <settings.h>
#include <gamestate.h>
#include <maths.h>
#include <scenes/mapview.h>

void draw_slice(SDL_Surface* surface, int x, int y_top, int y_bot, SDL_Surface* texture);
void render_background(SDL_Renderer* renderer, Gamestate& state);
void render_walls(SDL_Renderer* renderer, Gamestate& state);

void handle_firstperson(SDL_Event& event, Gamestate& state);
void render_firstperson(SDL_Renderer* renderer, Gamestate& state);

#endif
