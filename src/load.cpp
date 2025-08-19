#include <load.h>

// SpriteType string_to_sprite_type(const std::string& type, const std::string& name) {
//     if (type == "decoration") return string_to_decoration(name);
//     if (type == "pickup") return string_to_pickup(name);
//     if (type == "enemy") return string_to_enemy(name);
// }

// DecorationType string_to_decoration(const std::string& name) {
//     if (name == "barrel") return DecorationType::Barrel;
// }
// PickupType string_to_pickup(const std::string& name) {
//     if (name == "health") return PickupType::Health;
// }
// EnemyType string_to_enemy(const std::string& name) {
//     if (name == "goon") return EnemyType::Goon;
// }

void load_images(json data, Gamestate& state) {
    state.images.clear();
    for (const auto& image : data["images"]) {
        std::string path = "resources/images/" + image["path"].get<std::string>() + ".png";
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (surface == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: %s", image["name"].get<std::string>().c_str());
        }
        state.images[image["name"].get<std::string>()] = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    }
}

void load_spritesheets(json data, Gamestate& state) {
    // TODO: Check if this is sufficient for clearing an existing collection of sprite sheets
    state.sheets.clear();
    for (const auto& sheet : data["spritesheets"]) {
        SpriteSheet spritesheet = SpriteSheet();

        std::string name = sheet["name"].get<std::string>();
        std::string path = sheet["path"].get<std::string>();
        path = "resources/images/" + path + "/";
        spritesheet.image = IMG_Load((path + "sheet.png").c_str());
        // SDL_Surface* surface = IMG_Load((path + "sheet.png").c_str());
        // spritesheet.image = *surface;

        std::ifstream file(path + "sheet.json");
        json sheet_data;
        file >> sheet_data;
        
        for (auto& [key, value] : sheet_data.items()) {
            std::string segment_name = key;
            std::vector<int> rect = value.get<std::vector<int>>();
            int t = rect[0];
            int l = rect[1];
            int w = rect[2];
            int h = rect[3];
            spritesheet.segments[segment_name] = SDL_Rect{t, l, w, h};
        }

        state.sheets[name] = spritesheet;
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
        state.map.push_back({{{x1, y1}, {x2, y2}}, image, true});
    }
}

void load_sprites(json data, Gamestate& state) {
    state.decorations.clear();
    state.pickups.clear();
    state.enemies.clear();
    for (const auto& sprite : data["sprites"]) {
        std::string type = sprite["type"].get<std::string>();
        std::string name = sprite["name"].get<std::string>();
        std::string sheet = sprite["sheet"].get<std::string>();
        std::string sheet_segment = sprite["sheet_segment"].get<std::string>();

        float x = sprite["x"].get<float>();
        float y = sprite["y"].get<float>();
        
        if (type == "decoration") {
            state.decorations.push_back({
                Barrel, {sheet, sheet_segment, Point{x, y}, Point{0, 1}}
            });
        }
        if (type == "enemy") {
            state.enemies.push_back({
                Goon, {sheet, sheet_segment, Point{x, y}, Point{0, 1}}
            });
        }
    }
}

void load_level(std::string level, Gamestate& state){
    SDL_Log("Loading level: %s", level.c_str());

    std::string level_path = "resources/levels/" + level + ".json";
    std::ifstream file(level_path);
    json data;
    file >> data;

    load_images(data, state);
    load_spritesheets(data, state);
    load_lines(data, state);
    load_sprites(data, state);
}
