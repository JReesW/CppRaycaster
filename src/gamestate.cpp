#include <types.hpp>


std::vector<Sprite*> Gamestate::get_sprites() {
    std::vector<Sprite*> result;

    for (Decoration& d : this->decorations) {
        result.push_back(&d.sprite);
    }
    for (Pickup& p : this->pickups) {
        result.push_back(&p.sprite);
    }
    for (Enemy& e : this->enemies) {
        result.push_back(&e.sprite);
    }

    return result;
}


void Gamestate::clear_images() {
    for (auto& kv : this->sheets) {
        SDL_FreeSurface(kv.second.image);
    }
    for (auto& kv : this->images) {
        SDL_FreeSurface(kv.second);
    }
}
