#ifndef VINDICTA_LOAD_H
#define VINDICTA_LOAD_H

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <memory>
#include <SDL2/SDL_image.h>

#include <types.hpp>
#include <sprites.hpp>

using json = nlohmann::json;

void load_images(json data, Gamestate& state);
void load_spritesheets(json data, Gamestate& state);
void load_sprites(json data, Gamestate& state);
void load_lines(json data, Gamestate& state);
void load_level(std::string level, Gamestate& state);

#endif
