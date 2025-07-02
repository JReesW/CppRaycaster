#include <raycast.h>

void raycast(Gamestate& state) {
    state.renderEntries.clear();
    for (int x = 0; x < settings::SCREENWIDTH; x++) {
        // CLEAR COLLISIONS FOR THIS RAY
        std::set<RenderEntry> collisions;

        // CREATE THE RAY
        float angle = maths::lerp(-30.0f, 30.0f, (float)x / (float)(settings::SCREENWIDTH - 1));
        point raydir = point{1, 0}.rotate(state.player.angle + angle) * 1000.0;
        line ray = {state.player.position, state.player.position + raydir};
        
        // CHECK COLLISIONS
        for (mapline& ml : state.map) {
            std::optional<point> i = intersection(ray, ml.geometry);
            if (i != std::nullopt) {
                collisions.insert({distance(state.player.position, i.value()), Collision{i.value(), &ml, x}});
            }
        }
        if (collisions.size() > 0) {
            state.renderEntries.insert(*collisions.begin());
        }
    }
}
