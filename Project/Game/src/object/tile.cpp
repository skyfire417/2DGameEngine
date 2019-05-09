#include "object/tile.h"

const Tile::TileType Tile::kNullTileType {"none"};
const Tile::MarkType Tile::kNullMarkType {"none"};

std::unordered_map<std::string, std::string> Tile::tile_abbrs_;
std::unordered_map<std::string, std::string> Tile::mark_abbrs_;
std::unordered_map<Tile::TileType, Image::ImageID> Tile::tile_image_ids_;
std::unordered_set<Tile::TileType> Tile::collidable_tiles_ {};

std::unordered_set<Tile::TileType> Tile::tile_types_;
std::unordered_set<Tile::MarkType> Tile::mark_types_;

bool Tile::IsMarkType(std::string type) {
    return mark_types_.find(type) != mark_types_.end();
}

bool Tile::IsTileType(std::string type) {
    return tile_types_.find(type) != mark_types_.end();
}

void Tile::ReadConfiguration() {
    auto& config_provider = GetConfigProvider();
    auto& image_provider = GetImageProvider();
    auto levels_shared = config_provider.GetRoot()["levels"]["shared"];

    /// Read "tiles" section
    tile_types_.clear();
    tile_abbrs_.clear();
    tile_image_ids_.clear();
    collidable_tiles_.clear();
    auto tiles = levels_shared["tiles"];
    for (Json::Value::const_iterator tile_itr = tiles.begin();
         tile_itr != tiles.end(); ++tile_itr) {
        auto tile = *tile_itr;
        auto tile_id = tile_itr.key().asString();
        // Load image
        auto ret = config_provider.ParseImageValue(tile["image"]);
        auto image_id = std::get<0>(ret);
        auto image_filename = std::get<1>(ret);
        auto image_rects = std::get<2>(ret);
        image_provider.PutImage(image_id, image_filename, image_rects[0]);
        tile_image_ids_[tile_id] = image_id;
        // Load "collidable"
        if (tile["collidable"].asBool()) {
            collidable_tiles_.insert(tile_id);
        }
        // Load abbreviations
        tile_abbrs_[tile["abbr"].asString()] = tile_id;
        tile_types_.insert(tile_id);
    }

    /// Read "marks" section
    mark_types_.clear();
    mark_abbrs_.clear();
    auto marks = levels_shared["marks"];
    for (Json::Value::const_iterator mark_itr = marks.begin();
         mark_itr != marks.end(); ++mark_itr) {
        auto mark = *mark_itr;
        auto mark_id = mark_itr.key().asString();
        // Load abbreviations
        mark_abbrs_[mark["abbr"].asString()] = mark_id;
        mark_types_.insert(mark_id);
    }

    /// Read "items" section
    auto items = levels_shared["items"];
    for (Json::Value::const_iterator item_itr = items.begin();
         item_itr != items.end(); ++item_itr) {
        auto item = *item_itr;
        auto ret = config_provider.ParseImageValue(item["image"]);
        auto image_id = std::get<0>(ret);
        auto image_filename = std::get<1>(ret);
        auto image_rects = std::get<2>(ret);
        image_provider.PutImage(image_id, image_filename, image_rects[0]);
        auto mark_id = item["mark"].asString();
        mark_abbrs_[item["abbr"].asString()] = mark_id;
        mark_types_.insert(mark_id);
    }

    /// Read "enemies" section
    auto enemies = levels_shared["enemies"];
    for (Json::Value::const_iterator enemy_itr = enemies.begin();
         enemy_itr != enemies.end(); ++enemy_itr) {
        auto enemy = *enemy_itr;
        auto images = enemy["images"];
        for (Json::Value::const_iterator enemy_img_itr = images.begin();
             enemy_img_itr != images.end(); ++enemy_img_itr) {
            auto image = *enemy_img_itr;
            auto ret = config_provider.ParseImageValue(image);
            auto image_id = std::get<0>(ret);
            auto image_filename = std::get<1>(ret);
            auto image_rects = std::get<2>(ret);
            int num_images = image_rects.size();
            assert(num_images > 0);
            if (num_images == 1) {
                image_provider.PutImage(image_id, image_filename,
                                        image_rects[0]);
            } else {
                image_provider.PutSprite(image_id, image_filename, image_rects);
            }
        }
        auto mark_id = enemy["mark"].asString();
        mark_abbrs_[enemy["abbr"].asString()] = mark_id;
        mark_types_.insert(mark_id);
    }
}

bool Tile::IsCollidable() {
    return collidable_tiles_.find(tile_type_) != collidable_tiles_.end();
}

std::pair<int, int> Tile::GetTextureDimen() const {
    // Assumes every tile has same dimension including null tiles
    assert(tile_image_ids_.find("box") != tile_image_ids_.end());
    return GetImageProvider().GetImageDimen(tile_image_ids_["box"]);
}
