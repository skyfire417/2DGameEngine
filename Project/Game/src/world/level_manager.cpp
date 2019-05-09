#include "world/level_manager.h"
#include <sstream>
#include "utility.h"

std::vector<std::shared_ptr<Level>> LevelManager::levels_;
std::unordered_set<Image::ImageID> LevelManager::background_img_ids_;

void LevelManager::ReadConfiguration() {
    auto& config_provider = GetConfigProvider();
    auto& image_provider = GetImageProvider();
    Json::Value levels = config_provider.GetValue("levels");
    Json::Value shared = levels["shared"];

    /// Load configuration of level background
    Json::Value backgrounds = shared["background"];
    for (Json::Value::const_iterator bg_itr = backgrounds.begin();
         bg_itr != backgrounds.end(); ++bg_itr) {
        auto ret = config_provider.ParseImageValue((*bg_itr)["image"]);
        auto image_id = std::get<0>(ret);
        auto image_filename = std::get<1>(ret);
        auto image_rects = std::get<2>(ret);
        int num_images = image_rects.size();
        assert(num_images > 0);
        if (num_images == 1) {
            image_provider.PutImage(image_id, image_filename, image_rects[0]);
        } else {
            image_provider.PutSprite(image_id, image_filename, image_rects);
        }
        background_img_ids_.insert(image_id);
    }

    const Json::Value map = levels["map"];
    for (int i = 0; i < map.size(); ++i) {
        const Json::Value level_val = map[i];

        std::vector<std::string> layout;
        const Json::Value layout_val = level_val["tiles"];
        for (int j = 0; j < layout_val.size(); ++j) {
            layout.push_back(layout_val[j].asString());
        }
        levels_.push_back(
            std::make_shared<Level>(level_val["background_img_id"].asString(),
                                    level_val["num_col"].asInt(), layout, i));
    }
}

const std::unordered_set<Image::ImageID> LevelManager::background_img_ids() {
    return background_img_ids_;
}
