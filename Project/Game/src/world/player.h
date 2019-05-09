#ifndef PLATFORMER_WORLD_PLAYER_H
#define PLATFORMER_WORLD_PLAYER_H

#include <SDL2pp/Rect.hh>
#include <unordered_set>
#include "object/fallable_object.h"

//! Manages player info such as lives, score etc.
class Player : public FallableObject {
   public:
    class VertMoveState {
       public:
        enum Value { kOnGround, kInAir };
        VertMoveState(Value val) : val_(val) {}
        //! Define the operators
        bool operator==(const Value& val) { return val_ == val; }
        //! Define the operators
        bool operator==(const VertMoveState& state) {
            return state.val_ == val_;
        }

        std::string to_string() {
            switch (val_) {
                case kOnGround:
                    return "on-ground";
                case kInAir:
                    return "in-air";
            }
            return "unknown-vertical-state";
        }

       private:
        Value val_;
    };
    //! State of horizontal moves
    class HorzMoveState {
       public:
        enum Value { kLeft, kRight, kStand };
        HorzMoveState(Value val) : val_(val) {}
        bool operator==(const Value& val) { return val_ == val; }
        bool operator==(const HorzMoveState& state) {
            return state.val_ == val_;
        }

        std::string to_string() {
            switch (val_) {
                case kLeft:
                    return "moving-left";
                case kRight:
                    return "moving-right";
                case kStand:
                    return "standing";
            }
            return "unknown-horizontal-state";
        }

       private:
        Value val_;
    };

    //! Reads json configuration
    static void ReadConfiguration();
    //! Change image set selection
    /*!
     * May throw a runtime error indicating invlaid selection.
     */
    static void ChangeImageSet(std::string selection);
    //! Change max lives selection
    /*!
     * May throw a runtime error indicating invlaid selection.
     */
    void ChangeMaxLives(int lives);
    int GetMaxLives() { return max_live_count_; }

    //! Constructor for Player class
    Player();
    //! Update the position of player
    void Update(int ms_passed);
    //! Render the player to the screen
    void Render(Math::Vec2<double> pos, int ms_passed);

    //! Return player's moving state
    VertMoveState vert_move_state() { return vert_move_state_; }
    //! Return player's moving state
    HorzMoveState horz_move_state() { return horz_move_state_; }

    //! Sets player's moving state
    void set_move_state(VertMoveState state) { vert_move_state_ = state; }
    //! Sets player's moving state
    void set_move_state(HorzMoveState state) { horz_move_state_ = state; }

    //! Return player lives
    int live_count() const { return live_count_; }

    //! Reset player live
    void ResetLiveCount() { live_count_ = max_live_count_; }

    //! Player lose one live
    void Minus1LiveCount() { live_count_ -= 1; }

    //! Returns the velocity on x axis when moving
    double moving_vel_x() const { return moving_vel_x_; }

    //! Returns the initial velocity on y axis when moving
    double moving_init_vel_y() const { return moving_init_vel_y_; }
    //! Start to move in the horizon direction
    void StartMoveHorz(bool to_right);
    //! Get selected image
    std::string GetImageSelected() { return image_set_selected_; }

    void Jump();

   private:
    static std::unordered_set<std::string> image_sets_;
    static std::string image_set_selected_;

    //! Get stand image id
    static std::string GetStandImageID();
    //! Get jump image id
    static std::string GetJumpImageID();
    //! Get walk sprite id
    static std::string GetWalkSpriteID();

    static int max_live_count_;

    //! Update the walking sprite of the player
    void UpdateWalkSpriteIndex(int ms_passed);

    const int kMsPerWalkSprite = 100;

    VertMoveState vert_move_state_ {VertMoveState::kInAir};
    HorzMoveState horz_move_state_ {HorzMoveState::kStand};
    int walk_sprite_count_ = 0;
    int walk_sprite_index_ = 0;
    int walk_sprite_ms_passed_ = 0;
    bool is_last_horz_direction_right_ = true;

    int live_count_;
    static double moving_vel_x_;
    static double moving_init_vel_y_;
};

#endif
