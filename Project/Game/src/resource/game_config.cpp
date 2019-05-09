#include "resource/game_config.h"

std::tuple<std::string, std::string, std::vector<std::vector<int>>>
GameConfig::ParseImageValue(const Json::Value& value) const {
    std::vector<std::vector<int>> rect;
    const Json::Value rect_value = value["rect"];
    for (Json::ArrayIndex i = 0; i < rect_value.size(); i++) {
        std::vector<int> image;
        const Json::Value image_value = rect_value[i];
        for (Json::ArrayIndex j = 0; j < image_value.size(); j++) {
            image.push_back(image_value[j].asInt());
        }
        rect.push_back(image);
    }
    return {value["id"].asString(), value["file"].asString(), rect};
}
