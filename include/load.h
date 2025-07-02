#ifndef VINDICTA_LOAD_H
#define VINDICTA_LOAD_H

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>
#include <SDL2/SDL_image.h>

#include <types.h>

using json = nlohmann::json;

void load_images(json data, Gamestate& state);
void load_lines(json data, Gamestate& state);

void load_level(std::string level, Gamestate& state);

#endif
