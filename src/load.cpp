#include <load.h>


void load_images(json data, Gamestate& state) {
    state.images.clear();
    for (const auto& image : data["images"]) {
        std::string path = "resources/images/" + image["path"].get<std::string>() + ".bmp";
        SDL_Surface* surface = SDL_LoadBMP(path.c_str());
        if (surface == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: %s", image["name"].get<std::string>().c_str());
        }
        state.images[image["name"].get<std::string>()] = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    }
}

void load_lines(json data, Gamestate& state) {
    state.map.clear();
    for (const auto& line : data["lines"]) {
        float x1 = line["x1"].get<float>();
        float y1 = line["y1"].get<float>();
        float x2 = line["x2"].get<float>();
        float y2 = line["y2"].get<float>();
        std::string image = line["image"].get<std::string>();
        state.map.push_back({{{x1, y1}, {x2, y2}}, image});
    }
}

void load_level(std::string level, Gamestate& state){
    SDL_Log("Loading level: %s", level.c_str());

    std::string level_path = "resources/levels/" + level + ".json";
    std::ifstream file(level_path);
    json data;
    file >> data;

    load_images(data, state);
    load_lines(data, state);
}
