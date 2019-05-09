#ifndef PLATFORMER_OBJECT_TILE_MANAGER_H
#define PLATFORMER_OBJECT_TILE_MANAGER_H

#include "object/coin.h"
#include "object/enemy/patrol_enemy.h"
#include "object/tile.h"
#include "physics/collision.h"
#include "resource/base.h"
#include "resource/image.h"
#include "tinymath.h"
#include "world/level_manager.h"

//! Manages tile objects
class TileManager : public Base {
   public:
    static void ReadConfiguration();

    TileManager(std::shared_ptr<Collision> collision) : collision_(collision) {}

    //! Copy constructor that copies only some of the members
    //! Only members that are not easy to copy are copied here.
    //! Most members will be initialized by `InitTiles`.
    TileManager(const TileManager& other) : TileManager(other.collision_) {
        enemy_collid_funcs_ = other.enemy_collid_funcs_;
    }

    //! Removes all tiles and recreate using the specified layout
    /*!
     * The first tile's position is set at `start_point`.
     * It sets tiles' on-collision functions to stop colliding objects.
     * It resets any other on-collision functions set previously.
     */
    void InitTiles(std::shared_ptr<Level> level);

    //! Return the tiles
    const std::vector<std::shared_ptr<Tile>>& tiles() { return tiles_; }

    /*! \brief Returns the initial player position relative to the top-left
     *         corner of the first tile.
     * Must be called after `InitTiles` has been called.
     */
    Math::Vec2<double> PlayerInitRelPosMap() {
        return Math::Add(init_player_pos_, pos_offset_);
    }

    //! Returns dimension (width and height) of all tiles combined
    //! Must be called after `InitTiles` has been called.
    std::pair<int, int> tiles_dimen() { return tiles_dimen_; }

    //! Updates all tiles' positions
    void Update(int ms_passed);

    //! Renders all tiles' with first tile rendered at a certain position
    void Render(Math::Vec2<double> start_point, int ms_passed);

    //! Returns tile's width
    int TileWidth() { return tiles_[0]->GetTextureDimen().first; }

    //! Returns tile's height
    int TileHeight() { return tiles_[0]->GetTextureDimen().second; }

    //! Share collidable objects
    std::vector<std::shared_ptr<CollidableObject>> ShareCollidableObjects();

    //! Return total count of coins
    int TotalCoinCount() { return coins_.size(); }

    //! Return the count of collected coins
    int CollectedCoinCount() {
        return std::count_if(coins_.begin(), coins_.end(), [](auto& coin) {
            return coin->state() == Coin::State::kTouched;
        });
    }

    //! Add enemy on collision functions
    void AddEnemyOnCollisionFunction(
        std::function<void(CollidableObject&, Math::Vec2<double>)> func) {
        enemy_collid_funcs_.push_back(func);
    }

    //! Clear objects
    void Clear() {
        // Unregister collision observation for all current tiles
        for (auto& obj : ShareCollidableObjects()) {
            collision_->RemoveCollidableObject(*obj);
        }
        for (auto& coin : coins_) {
            collision_->RemoveCollidableObject(*coin);
        }
        for (auto& patrol : patrol_enemies_) {
            collision_->RemoveCollidableObject(*patrol);
        }

        tiles_.clear();
        coins_.clear();
        patrol_enemies_.clear();
    }

    //! Adjust positions of all objects
    void AdjustAllObjectsPosition(Math::Vec2<double> offset) {
        pos_offset_.Add(offset);
        MoveAllObjectsBy(offset);
    }

    Tile::TileType GetTileType(int row, int col);
    Tile::MarkType GetTileMarkType(int row, int col);

    //! Mark next tile change
    void MarkNextTileChange(std::string selection);
    //! Set the position of the tile corresponding to mouse position
    void SetTile(int row, int col, int mouse_x, int mouse_y);
    //! Get tiles
    std::vector<std::shared_ptr<Tile>> GetTiles() { return tiles_; }

    /*! \brief Returns a tile's position relative to the top-left
     *         corner of the first tile.
     */
    Math::Vec2<double> TileRelPosMap(int tile_index, int tile_width,
                                     int tile_height) {
        int num_col = cur_level_->tile_num_col();
        return {(double)(tile_index % num_col) * tile_width,
                (double)(tile_index / num_col) * tile_height};
    }

    void RemovePatrolEnemy(const PatrolEnemy& enemy);

    void ResizeMap(int new_row, int new_col);

   private:
    //! Render backgoround
    void RenderBackground(Math::Vec2<double> start_point);

    //! Render text for levels
    void RenderLevelText(Math::Vec2<double> start_point);

    //! Move objects by an offset
    void MoveAllObjectsBy(Math::Vec2<int> offset) {
        for (auto& tile : tiles_) {
            tile->AdjustPos(offset);
        }
        for (auto& coin : coins_) {
            coin->AdjustPos(offset);
        }
        for (auto& patrol : patrol_enemies_) {
            patrol->AdjustPos(offset);
        }
    }

    //! Create coins
    std::shared_ptr<Coin> CreateCoin(int tile_map_index, int tile_width,
                                     int tile_height);
    //! Remove a coin a certain position
    bool RemoveCoinAt(const Math::Vec2<int> point);

    //! Create a patrol enemy
    std::shared_ptr<PatrolEnemy> CreatePatrolEnemy(Tile::MarkType type,
                                                   int tile_map_index,
                                                   int tile_width,
                                                   int tile_height);
    //! Remove patrol enemy
    bool RemovePatrolEnemyAt(const Math::Vec2<int> point);

    std::shared_ptr<Tile> CreateTile(int tile_map_index,
                                     std::string tile_type_abbr,
                                     std::string mark_type_abbr);

    std::shared_ptr<Level> cur_level_;
    Math::Vec2<double> pos_offset_ {0, 0};
    std::vector<std::shared_ptr<Tile>> tiles_;
    std::vector<std::shared_ptr<Coin>> coins_;
    static double coin_wobble_vel_;
    static double coin_wobble_dist_;
    Math::Vec2<double> init_player_pos_;
    Math::Vec2<double> level_text_pos_;
    std::pair<int, int> tiles_dimen_;

    /* Enemies */
    static double slime_init_vel_;
    static double fly_init_vel_;
    std::vector<std::shared_ptr<PatrolEnemy>> patrol_enemies_;
    std::vector<std::function<void(CollidableObject&, Math::Vec2<double>)>>
        enemy_collid_funcs_;

    std::shared_ptr<Collision> collision_;

    enum class TileChangeType { kMark, kTile };
    std::string next_tile_change_value_;
    TileChangeType next_tile_change_type_;
};

#endif
