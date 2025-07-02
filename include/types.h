#ifndef VINDICTA_TYPES_H
#define VINDICTA_TYPES_H

#include <SDL2/SDL.h>

#include <string>
#include <vector>
#include <set>
#include <map>
#include <variant>

/////////////
//  ENUMS  //
/////////////

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

enum SpriteType {
    Object,
    Enemy
};

////////////////
//  GEOMETRY  //
////////////////

struct point {  // either a coordinate or a directional vector
    float x;
    float y;

    point rotate(float angle) const;
    point operator+(const point& a) const;
    point operator-(const point& a) const;
    point operator*(const float& a) const;
};

struct line {  // line segment with defined start and end points
    point start;
    point end;
    
    line rotate(float angle) const;
};

/////////////////
//  GAMESTATE  //
/////////////////

struct mapline {  // line segment with extra game information
    line geometry;
    std::string image;
    bool solid;
};

struct Sprite {
    SDL_Surface* image;
    SpriteType type;
    point position;
    point direction;
};

struct Player {
    point position;
    float angle;
    float speed = 2.0f;
    int fov = 60;
};

struct Collision {
    point location;
    mapline* target;
    int column;
};

struct RenderEntry {
    float depth;
    std::variant<Collision, Sprite> entry;

    bool operator<(const RenderEntry& other) const {
        return depth < other.depth;
    }
};

struct Gamestate {
    bool running;
    Scene scene;
    int keys;

    Player player;
    std::vector<mapline> map;
    std::vector<Sprite> objects;
    std::vector<Sprite> enemies;
    std::map<std::string, SDL_Surface*> images;

    std::multiset<RenderEntry> renderEntries;
    bool debug = false;
    int ticks = 0;
};

#endif
