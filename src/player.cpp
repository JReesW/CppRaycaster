#include <player.hpp>

void move_player(const float& dt, Gamestate& state, bool allow_strafing) {
    auto [nx, ny] = state.player.position;
    if (state.keys & Key_W) {
        nx += state.player.speed * cos(maths::deg2rad(state.player.angle));
        ny += state.player.speed * sin(maths::deg2rad(state.player.angle));
    }
    if (state.keys & Key_S) {
        nx -= state.player.speed * cos(maths::deg2rad(state.player.angle));
        ny -= state.player.speed * sin(maths::deg2rad(state.player.angle));
    }

    if (allow_strafing) {
        if (state.keys & Key_A) {
            nx += state.player.speed * cos(maths::deg2rad(state.player.angle - 90.0f));
            ny += state.player.speed * sin(maths::deg2rad(state.player.angle - 90.0f));
        }
        if (state.keys & Key_D) {
            nx += state.player.speed * cos(maths::deg2rad(state.player.angle + 90.0f));
            ny += state.player.speed * sin(maths::deg2rad(state.player.angle + 90.0f));
        }
    }

    // CHECK IF MOVEMENT DOESN'T COLLIDE
    bool no_collision = true;
    for (Mapline ml : state.map) {
        if (line_circle_intersect(ml.line, Point{nx, ny}, 10)) {
            no_collision = false;
            break;
        }
    }
    if (no_collision) {
        state.player.position.x = nx;
        state.player.position.y = ny;
    }
}
