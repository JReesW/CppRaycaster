#ifndef VINDICTA_PLAYER_H
#define VINDICTA_PLAYER_H

#include <lines.h>

struct Player {
    point position;
    float angle;
    float speed = 2.0f;
    int fov = 60;
};

#endif
