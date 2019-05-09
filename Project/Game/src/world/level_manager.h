#ifndef PLATFORMER_WORLD_LEVEL_MANAGER_H
#define PLATFORMER_WORLD_LEVEL_MANAGER_H

#include <json/json.h>
#include "resource/base.h"
#include "world/level.h"

//! Manages the levels in the game
class LevelManager : public Base {
   public:
    LevelManager() {}

    //! Read configuration from a JSON configuration file
    static void ReadConfiguration();
    static const std::unordered_set<Image::ImageID> background_img_ids();
    //! Return the number of current level
    int CurrentLevelNo() { return cur_level_ + 1; };
    std::shared_ptr<Level> ShareCurrentLevel() {
        assert(levels_.size() > 0);
        return levels_[cur_level_];
    }
    //! Get current level
    const Level& CurrentLevel() { return *ShareCurrentLevel(); }

    // Reset the level
    void ResetLevel() { cur_level_ = 0; }
    //! Go to next level
    void IncLevel() { cur_level_ += 1; }
    //! Check if it is the final level
    bool IsFinalLevel() { return cur_level_ == levels_.size() - 1; }

    //! Get the number of levels
    int TotalLevel() { return levels_.size(); }

   private:
    static std::vector<std::shared_ptr<Level>> levels_;
    static std::unordered_set<Image::ImageID> background_img_ids_;

    int cur_level_ {0};
};

#endif
