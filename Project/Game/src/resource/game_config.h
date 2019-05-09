#ifndef PLATFORMER_RESOURCE_GAME_CONFIG_H
#define PLATFORMER_RESOURCE_GAME_CONFIG_H

#include "resource/config.h"
#include "utility.h"

//! Represent GameConfig objects and functions
class GameConfig : public Config {
   public:
    GameConfig() {
        std::istringstream(
            Utility::ReadFileToString("../config/config.json", "rb")) >>
            config_root_;
    }
    //! Get root
    const Json::Value& GetRoot() const { return config_root_; }

    //! Get value
    const Json::Value& GetValue(const std::string& name) const {
        return config_root_[name];
    }

    std::tuple<std::string, std::string, std::vector<std::vector<int>>>
    //! Parse image value
    ParseImageValue(const Json::Value& value) const;

   private:
    Json::Value config_root_;
};

#endif
