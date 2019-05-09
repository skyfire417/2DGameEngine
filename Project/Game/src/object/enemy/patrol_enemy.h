#ifndef PLATFORMER_OBJECT_ENEMY_PATROL_ENEMY_H
#define PLATFORMER_OBJECT_ENEMY_PATROL_ENEMY_H

#include "json/json.h"
#include "object/collidable_object.h"

//! An enemy that moves back and forth in one direction at a constant speed
/*!
 * There must be two obstacles defining its moving area.
 * This enemy can move either horizontally or vertically, but note that
 * its sprite images are only flipped horziontally when switching
 * direction.
 */
class PatrolEnemy : public CollidableObject {
   public:
    class MoveState {
       public:
        enum Value { kPositive, kNegative };

        MoveState(Value val) : val_(val) {}
        bool operator==(const Value& val) { return val_ == val; }
        bool operator==(const MoveState& state) { return state.val_ == val_; }
        bool operator!=(const MoveState& state) { return state.val_ != val_; }
        MoveState& operator=(const Value& val) {
            val_ = val;
            return *this;
        }
        MoveState& operator=(const MoveState& state) {
            val_ = state.val_;
            return *this;
        }

        MoveState Switch() {
            switch (val_) {
                case kPositive:
                    return kNegative;
                case kNegative:
                    return kPositive;
            }
        }

       private:
        Value val_;
    };

    //! Initializes a patrolling enemy
    /*!
     * The following information is passed in:
     * - An initial speed (signed) and the direction flag when
     *   the enemy is at its initial speed
     * - An sprite image ID and the direction of it
     *
     * When changing direction, its sprite images are flipped horizontally.
     */
    PatrolEnemy(Math::Vec2<double> init_vel, MoveState init_direction,
                Image::SpriteID sprite_id, MoveState sprite_img_direction,
                int tile_map_index)
        : CollidableObject(CollidableObject::Type::kEnemy, ImageType::kSprite,
                           sprite_id),
          init_vel_(init_vel),
          sprite_id_(sprite_id),
          move_sprite_count_(GetImageProvider().GetFrameCount(sprite_id)),
          init_state_(init_direction),
          move_state_(init_direction),
          sprite_image_direction_(sprite_img_direction),
          tile_map_index_(tile_map_index) {
        SetVel(init_vel);
        AddOnCollisionFunc([this](auto& o, auto v) {
            if (o.CollidObjType() == CollidableObject::Type::kObstacle) {
                move_state_ = move_state_.Switch();
                SetVel(Math::Scale(init_vel_,
                                   move_state_ == init_state_ ? 1.0 : -1.0));
                move_sprite_index_ = 0;
                move_sprite_ms_passed_ = 0;
            }
        });
    }

    void Render(Math::Vec2<double> pos, int ms_passed) {
        UpdateMoveSpriteIndex(ms_passed);
        GetImageProvider().RenderSprite(sprite_id_, move_sprite_index_, pos,
                                        move_state_ != sprite_image_direction_);
    }

    int tile_map_index() { return tile_map_index_; }

   private:
    Math::Vec2<double> init_vel_ {0, 0};
    MoveState init_state_;

    /* Members for sprites */
    void UpdateMoveSpriteIndex(int ms_passed) {
        move_sprite_ms_passed_ += ms_passed;
        int passed_frame = move_sprite_ms_passed_ / kMsPerMoveSprite;
        move_sprite_ms_passed_ -= kMsPerMoveSprite * passed_frame;

        move_sprite_index_ += passed_frame;
        move_sprite_index_ %= move_sprite_count_;
    }

    Image::SpriteID sprite_id_;
    const int kMsPerMoveSprite = 100;
    int move_sprite_count_ = 0;
    int move_sprite_index_ = 0;
    int move_sprite_ms_passed_ = 0;
    MoveState move_state_;
    MoveState sprite_image_direction_;

    int tile_map_index_;
};

#endif
