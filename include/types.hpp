#ifndef VINDICTA_TYPES_H
#define VINDICTA_TYPES_H

#include <SDL2/SDL.h>

#include <string>
#include <vector>
#include <set>
#include <map>
#include <variant>

////////////////
//  GEOMETRY  //
////////////////

struct Point {  // either a coordinate or a directional vector
    float x;
    float y;

    Point rotate(float angle) const;
    Point operator+(const Point& a) const;
    Point operator-(const Point& a) const;
    Point operator*(const float& a) const;
};

struct Line {  // line segment with defined start and end points
    Point start;
    Point end;
    
    Line rotate(float angle) const;
};

////////////////
//  ENTITIES  //
////////////////

enum DecorationName {
    Barrel
};

enum PickupName {
    Health
};

enum EnemyName {
    Goon
};

struct SpriteSheet {
    SDL_Surface* image;
    std::map<std::string, SDL_Rect> segments;
};

struct Sprite {
    std::string sheet;
    std::string sheet_segment;
    
    Point position;
    Point direction;
};

struct Decoration {
    DecorationName name;
    Sprite sprite;
};

struct Pickup {
    PickupName name;
    Sprite sprite;
};

struct Enemy {
    EnemyName name;
    Sprite sprite;
};

struct Player {
    Point position;
    float angle;
    float speed = 2.0f;
    int fov = 60;
};

/////////////////
//  GAMESTATE  //
/////////////////

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

struct Mapline {  // line segment with extra game information
    Line line;
    std::string image;
    bool solid;
};

struct Collision {
    Point location;
    Mapline* target;
    int column;
};

struct RenderEntry {
    float depth;
    std::variant<Collision, Sprite*> entry;

    bool operator<(const RenderEntry& other) const {
        return depth < other.depth;
    }
};

struct Gamestate {
    bool running;
    Scene scene;
    int keys;

    Player player;
    std::vector<Mapline> map;
    std::vector<Decoration> decorations;
    std::vector<Pickup> pickups;
    std::vector<Enemy> enemies;
    std::map<std::string, SDL_Surface*> images;
    std::map<std::string, SpriteSheet> sheets;

    std::multiset<RenderEntry> renderEntries;

    SDL_Surface* bufferSurface;
    SDL_Texture* bufferTexture;
    std::vector<Sprite*> get_sprites();
    void clear_images();
    bool debug = false;
    int ticks = 0;
};

#endif
