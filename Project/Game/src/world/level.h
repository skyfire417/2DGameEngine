#ifndef PLATFORMER_WORLD_LEVEL_H
#define PLATFORMER_WORLD_LEVEL_H

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>
#include "resource/base.h"
#include "resource/image.h"

//! Level configuration
class Level : public Base {
   public:
    Level(const std::string& background_img_id, int tile_num_col,
          const std::vector<std::string>& tile_layout, int level_index)
        : background_img_id_(background_img_id),
          tile_num_col_(tile_num_col),
          tile_layout_(tile_layout),
          level_no_ {level_index + 1} {}
    //! Returns number of columns in a tile layout
    int tile_num_col() const { return tile_num_col_; }
    //! Returns tile layout
    const std::vector<std::string>& tile_layout() const { return tile_layout_; }
    //! Get id of background image
    Image::ImageID background_img_id() const { return background_img_id_; }
    
    //! Return level
    int level_no() const { return level_no_; }

   private:
    Image::ImageID background_img_id_;
    std::vector<std::string> tile_layout_;
    int tile_num_col_;
    //! Current level number starting from 1
    int level_no_ {1};
};

#endif
