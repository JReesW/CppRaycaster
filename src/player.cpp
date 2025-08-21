#include <player.hpp>


void move_player(const float& dt, Gamestate& state, bool allow_strafing) {
    float dx = 0;
    float dy = 0;
    if (state.keys & Key_W) {
        dx += state.player.speed * cos(maths::deg2rad(state.player.angle));
        dy += state.player.speed * sin(maths::deg2rad(state.player.angle));
    }
    if (state.keys & Key_S) {
        dx -= state.player.speed * cos(maths::deg2rad(state.player.angle));
        dy -= state.player.speed * sin(maths::deg2rad(state.player.angle));
    }

    if (allow_strafing) {
        if (state.keys & Key_A) {
            dx += state.player.speed * cos(maths::deg2rad(state.player.angle - 90.0f));
            dy += state.player.speed * sin(maths::deg2rad(state.player.angle - 90.0f));
        }
        if (state.keys & Key_D) {
            dx += state.player.speed * cos(maths::deg2rad(state.player.angle + 90.0f));
            dy += state.player.speed * sin(maths::deg2rad(state.player.angle + 90.0f));
        }
    }

    Point d {dx, dy};
    Point n = state.player.position + d;

    // SLIDE MOVEMENT ALONG WALL IF COLLIDING
    for (Mapline ml : state.map) {
        if (line_circle_intersect(ml.line, n, 10)) {
            d = d - normal(ml.line) * dot(d, normal(ml.line));
        }
    }
    state.player.position = state.player.position + d;
}
