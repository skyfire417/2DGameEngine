#include "world/player_input.h"

PlayerInput::PlayerInput() {
    auto key_horz = std::make_unique<KeyManager>();
    key_horz->AddNegativeKey({SDLK_LEFT, SDLK_a});
    key_horz->AddPositiveKey({SDLK_RIGHT, SDLK_d});
    state_horz_ =
        std::make_unique<NoInputState>(PlayerInput::Direction::kHorz, key_horz);

    auto key_vert = std::make_unique<KeyManager>();
    key_vert->AddNegativeKey({SDLK_UP, SDLK_w, SDLK_SPACE});
    key_vert->AddPositiveKey({SDLK_DOWN, SDLK_s});
    state_vert_ =
        std::make_unique<NoInputState>(PlayerInput::Direction::kVert, key_vert);
}

std::unique_ptr<PlayerInput::State> PlayerInput::NoInputState::ProcessInput(
    Player& player, SDL_Event e) {
    if (e.type == SDL_KEYDOWN) {
        int key = e.key.keysym.sym;
        if (key_manager_->IsNegative(key)) {
            auto next =
                std::make_unique<PlayerInput::NegState>(std::move(*this));
            next->SetLastInput(key);
            return next;
        } else if (key_manager_->IsPositive(key)) {
            auto next =
                std::make_unique<PlayerInput::PosState>(std::move(*this));
            next->SetLastInput(key);
            return next;
        }
    }
    return nullptr;
}

void PlayerInput::NoInputState::Enter(Player& player) {
    if (direction_ == Direction::kHorz) {
        player.set_velx(0);
        player.set_move_state(Player::HorzMoveState::kStand);
    }
}

std::unique_ptr<PlayerInput::State> PlayerInput::NegState::ProcessInput(
    Player& player, SDL_Event e) {
    if (e.type == SDL_KEYDOWN) {
        int key = e.key.keysym.sym;
        if (key_manager_->IsPositive(key)) {
            auto next =
                std::make_unique<PlayerInput::PosState>(std::move(*this));
            next->SetLastInput(key);
            return next;
        }
        if (direction_ == Direction::kHorz) {
            player.set_velx(-player.moving_vel_x());
        } else if (direction_ == Direction::kVert) {
            // Jump if player on ground
            if (player.vert_move_state() == Player::VertMoveState::kOnGround) {
                player.Jump();
            }
        }
    } else if (e.type == SDL_KEYUP) {
        int key = e.key.keysym.sym;
        if (key == last_input_) {
            auto next =
                std::make_unique<PlayerInput::NoInputState>(std::move(*this));
            next->SetLastInput(key);
            return next;
        }
    }
    return nullptr;
}

void PlayerInput::NegState::Enter(Player& player) {
    if (direction_ == Direction::kHorz) {
        player.StartMoveHorz(false);
    } else if (direction_ == Direction::kVert) {
        // Jump if player on ground
        if (player.vert_move_state() == Player::VertMoveState::kOnGround) {
            player.Jump();
        }
    }
}

std::unique_ptr<PlayerInput::State> PlayerInput::PosState::ProcessInput(
    Player& player, SDL_Event e) {
    if (e.type == SDL_KEYDOWN) {
        int key = e.key.keysym.sym;
        if (key_manager_->IsNegative(key)) {
            auto next =
                std::make_unique<PlayerInput::NegState>(std::move(*this));
            next->SetLastInput(key);
            return next;
        }
        if (direction_ == Direction::kHorz) {
            player.set_velx(player.moving_vel_x());
        }
    } else if (e.type == SDL_KEYUP) {
        int key = e.key.keysym.sym;
        if (key == last_input_) {
            auto next =
                std::make_unique<PlayerInput::NoInputState>(std::move(*this));
            next->SetLastInput(key);
            return next;
        }
    }
    return nullptr;
}

void PlayerInput::PosState::Enter(Player& player) {
    if (direction_ == Direction::kHorz) {
        player.StartMoveHorz(true);
    }
}

void PlayerInput::ProcessInput(SDL_Event e, Player& player) {
    auto next = state_horz_->ProcessInput(player, e);
    if (next != nullptr) {
        state_horz_ = std::move(next);
        state_horz_->Enter(player);
    }

    next = state_vert_->ProcessInput(player, e);
    if (next != nullptr) {
        state_vert_ = std::move(next);
        state_vert_->Enter(player);
    }
}
