#ifndef PLATFORMER_WORLD_CAMERA_SYSTEM_H
#define PLATFORMER_WORLD_CAMERA_SYSTEM_H

#include "object/border_manager.h"
#include "object/tile_manager.h"
#include "world/player.h"
#include "world/world.h"

class World;
//! Camera system for the game
/*!
 * Some definitions:
 * - &Tile map: the map of a level, usually bigger than the GUI window
 * - Camera: a rectangular area within the tile map that follows the player
 * - Viewport: the rectangular area within the GUI window where all the moving
 *   objects in game are shown. There is other static information in the GUI
 *   window e.g. life count, etc.
 *
 * Viewport usually has same size as that of camera, i.e. users see the game
 * world through this camera only.
 *
 * Due to the implementation of camera, all objects in the camera will change
 * their positions frequently, thus this class will take care of `Update` and
 * `Render` those objects, e.g. player and tile_manager.
 */
class CameraSystem {
   public:
    CameraSystem(std::shared_ptr<TileManager> cur_tilemap,
                 std::shared_ptr<LevelManager> level_manager,
                 std::shared_ptr<BorderManager> border_manager,
                 std::shared_ptr<Player> player, Math::Rect<int> viewport_rect)
        : cur_tilemap_(cur_tilemap),
          level_manager_(level_manager),
          border_manager_(border_manager),
          player_(player),
          viewport_relpos_win_(viewport_rect.x, viewport_rect.y),
          cam_dimen_(viewport_rect.w, viewport_rect.h) {}

    //! Initialize all objects in camera, i.e. player and tiles
    //! Musted be called after `tile_manager.InitTiles` has been called.
    void ResetAndInit();

    //! Update the camera
    void Update(int ms_passed);

    //! Render to the screen
    void Render(int ms_passed);

    //! Converts a position from map-relative to viewport-relative.
    Math::Vec2<double> ConvertFromMapRelToViewportRelPos(
        Math::Vec2<double> map_relpos) const {
        return Math::Add(map_relpos, {-cam_relpos_map_.x, -cam_relpos_map_.y});
    }

    //! Move the camera to next level
    void MoveToNextLevel(World& world,
                         std::shared_ptr<TileManager> new_tilemap);

    //! Get the tile map dimension
    std::pair<int, int> TileMapDimen() {
        std::pair<int, int> res {cur_tilemap_->tiles_dimen()};
        if (prev_tilemap_ != nullptr) {
            res = {res.first + prev_tilemap_->tiles_dimen().first,
                   std::max(res.second, prev_tilemap_->tiles_dimen().second)};
        }
        return res;
    }

    //! Return top of camera
    Math::Vec2<int> GetCameraTop() {return viewport_relpos_win_;}
    //! Return the position of the camera
    Math::Vec2<double> GetCameraPos() {return cam_relpos_map_;}

   private:
    class MovingObject {
       public:
        MovingObject(std::shared_ptr<Object> object) : object_(object) {
            old_relpos_win_ = object->GetPos();
        }

       private:
        std::shared_ptr<Object> object_;
        Math::Vec2<double> old_relpos_win_;
        Math::Vec2<double> relpos_map_;
    };

    //! Centers the camera to follow the player
    void CenterCamera();

    void RemovePreviousTilemapIfOutOfCamera();

    //! Viewport position relative to GUI window
    Math::Vec2<int> viewport_relpos_win_;
    //! Camera position relative to the tile map's top left corner
    Math::Vec2<double> cam_relpos_map_;
    //! Camera's dimension
    std::pair<int, int> cam_dimen_;

    std::shared_ptr<TileManager> cur_tilemap_;
    std::shared_ptr<TileManager> prev_tilemap_;
    std::shared_ptr<LevelManager> level_manager_;
    std::shared_ptr<BorderManager> border_manager_;
    std::shared_ptr<Player> player_;
};

#endif
