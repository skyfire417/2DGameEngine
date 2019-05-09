#ifndef PLATFORMER_OBJECT_COIN_H
#define PLATFORMER_OBJECT_COIN_H

#include "object/collidable_object.h"
//! %Base class for coins
/*!
 * A coin is a collidable object that will be collected and disappears after collision.
 */
class Coin : public CollidableObject {
   public:
    enum class State { kIntact, kTouched };

    Coin(double wobble_vel, double wobble_dist, Image::ImageID img_id,
         int tile_map_index)
        : CollidableObject(Type::kCoin, ImageType::kStatic, img_id),
          wobble_vel_(wobble_vel),
          wobble_dist_(wobble_dist),
          tile_map_index_(tile_map_index) {
        SetCollidPositionOffset({17, 18});
        SetCollidObjDimen({36, 36});
        AddOnCollisionFunc([this](auto o, auto v) {
            if (o.CollidObjType() == CollidableObject::Type::kPlayer) {
                this->state_ = State::kTouched;
                GetMixer().PlayChannel(
                    -1, GetAudioProvider().GetSoundEffect(Audio::ID::kCoin));
            }
        });

        wobble_ = Math::Random() * Math::kPi * 2;
    }
    //! Update the scene
    void Update(int ms_passed) {
        wobble_ += ms_passed / 1000.0 * wobble_vel_;
        pos_.y = origin_pos_.y + std::sin(wobble_) * wobble_dist_;
    }
    
    //! Render to the screen
    void Render(Math::Vec2<double> pos, int ms_passed) {
        if (state_ == State::kIntact) {
            CollidableObject::Render(pos, ms_passed);
        }
    }

    //! Return the state
    State state() { return state_; }

    //! Set the state
    void set_state(State state) { state_ = state; }

    //! Adjust coin position
    void AdjustPos(const Math::Vec2<double>& offset) {
        origin_pos_.Add(offset);
        CollidableObject::AdjustPos(offset);
    }
    
    //! Set coin position
    void SetPos(Math::Vec2<double> pos) {
        origin_pos_ = pos;
        CollidableObject::SetPos(pos);
    }

    //! return the index of tile map
    int tile_map_index() { return tile_map_index_; }

   private:
    State state_ {State::kIntact};

    /* Members for wobbling */
    double wobble_;
    double wobble_vel_;
    double wobble_dist_;

    Math::Vec2<double> origin_pos_;

    int tile_map_index_;
};

#endif
