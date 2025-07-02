#ifndef VINDICTA_PLAYER_H
#define VINDICTA_PLAYER_H

#include <SDL2/SDL.h>

#include <lines.h>
#include <types.h>

void move_player(const float& dt, Gamestate& state, bool allow_strafing);

#endif
