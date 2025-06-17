#ifndef VINDICTA_GAMESTATE_H
#define VINDICTA_GAMESTATE_H

#include <vector>
#include <array>
#include <map>
#include <string>
#include <SDL2/SDL.h>

#include <settings.h>
#include <lines.h>
#include <player.h>


enum Scene {
    Opening,
    Menu,
    Mapview,
    FirstPerson
};

enum HoldKey {
    Key_None = 0,
    Key_W = 1,
    Key_A = 2,
    Key_S = 4,
    Key_D = 8
};

struct Collision {
    point location;
    mapline* target;
};

struct Gamestate {
    bool running;
    Scene scene;
    int keys;

    Player player;
    std::vector<mapline> map;
    std::array<std::vector<Collision>, settings::SCREENWIDTH> collisions;
    std::map<std::string, SDL_Surface*> images;
};

#endif
