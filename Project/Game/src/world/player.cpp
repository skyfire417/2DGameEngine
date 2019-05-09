#include "world/player.h"
#include <stdexcept>

double Player::moving_vel_x_ {60.0};
double Player::moving_init_vel_y_ {30.0};

std::unordered_set<std::string> Player::image_sets_;
std::string Player::image_set_selected_;
int Player::max_live_count_;

Player::Player()
    : FallableObject(CollidableObject::Type::kPlayer, ImageType::kStatic,
                     GetStandImageID()),
      live_count_(max_live_count_),
      walk_sprite_count_(GetImageProvider().GetFrameCount(GetWalkSpriteID())) {
    AddOnCollisionFunc([this](auto& o, auto v) {
        if (v.y < 0) {
            this->vert_move_state_ = VertMoveState::kOnGround;
        }
        // Stops when running into obstacles
        if (o.CollidObjType() == CollidableObject::Type::kObstacle) {
            if (abs(v.x) > 0) {
                set_velx(0);
            }
            if (abs(v.y) > 0) {
                set_vely(0);
            }
        }
    });
}

void Player::Update(int ms_passed) {
    vert_move_state_ = VertMoveState::kInAir;
    FallableObject::Update(ms_passed);
}

void Player::Render(Math::Vec2<double> pos, int ms_passed) {
    auto& image_provider = GetImageProvider();
    if (vert_move_state_ == VertMoveState::kInAir) {
        image_provider.RenderImage(GetJumpImageID(), pos,
                                   !is_last_horz_direction_right_);
    } else {
        if (horz_move_state_ == HorzMoveState::kStand) {
            image_provider.RenderImage(GetStandImageID(), pos,
                                       !is_last_horz_direction_right_);
        } else {
            UpdateWalkSpriteIndex(ms_passed);
            image_provider.RenderSprite(
                GetWalkSpriteID(), walk_sprite_index_, pos,
                horz_move_state_ == HorzMoveState::kLeft);
        }
    }
}

void Player::ReadConfiguration() {
    auto& config_provider = GetConfigProvider();
    auto& image_provider = GetImageProvider();
    Json::Value player = config_provider.GetValue("player");
    moving_vel_x_ = player["init_vel_x"].asDouble();
    moving_init_vel_y_ = player["init_vel_y"].asDouble();
    image_set_selected_ = player["init_image_selected"].asString();
    max_live_count_ = player["max_lives"].asInt();

    auto images_all = player["images"];
    image_sets_.clear();
    for (Json::Value::const_iterator images_all_itr = images_all.begin();
         images_all_itr != images_all.end(); ++images_all_itr) {
        auto images_choice = *images_all_itr;
        for (Json::Value::const_iterator images_choice_itr =
                 images_choice.begin();
             images_choice_itr != images_choice.end(); ++images_choice_itr) {
            auto image = *images_choice_itr;
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
        image_sets_.insert(images_all_itr.key().asString());
    }
}

void Player::StartMoveHorz(bool to_right) {
    if (vert_move_state_ == VertMoveState::kOnGround) {
        // Started walking
        walk_sprite_index_ = 0;
        walk_sprite_ms_passed_ = 0;
    }
    set_velx(moving_vel_x_ * (to_right ? 1 : -1));
    is_last_horz_direction_right_ = to_right;
    if (to_right)
        set_move_state(Player::HorzMoveState::kRight);
    else
        set_move_state(Player::HorzMoveState::kLeft);
}

void Player::UpdateWalkSpriteIndex(int ms_passed) {
    walk_sprite_ms_passed_ += ms_passed;
    int passed_frame = walk_sprite_ms_passed_ / kMsPerWalkSprite;
    walk_sprite_ms_passed_ -= kMsPerWalkSprite * passed_frame;

    walk_sprite_index_ += passed_frame;
    walk_sprite_index_ %= walk_sprite_count_;
}

std::string Player::GetStandImageID() {
    return "player_" + image_set_selected_ + "_stand";
}

std::string Player::GetJumpImageID() {
    return "player_" + image_set_selected_ + "_jump";
}

std::string Player::GetWalkSpriteID() {
    return "player_" + image_set_selected_ + "_walk";
}

void Player::ChangeImageSet(std::string selection) {
    if (image_sets_.find(selection) == image_sets_.end()) {
        std::ostringstream stream;
        std::copy(image_sets_.begin(), image_sets_.end(),
                  std::ostream_iterator<std::string>(stream, ", "));
        std::string set_str = stream.str();
        throw std::runtime_error("Invalid selection of player's image set: " +
                                 selection + " not in [" + set_str + "]");
    }
    image_set_selected_ = selection;
}

void Player::ChangeMaxLives(int lives) {
    max_live_count_ = lives;
    live_count_ = lives;
}

void Player::Jump() {
    set_vely(moving_init_vel_y());
    set_move_state(VertMoveState::kInAir);
}
