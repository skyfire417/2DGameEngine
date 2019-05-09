#ifndef PLATFORMER_PLAYER_INPUT_H
#define PLATFORMER_PLAYER_INPUT_H

#include <initializer_list>
#include <memory>
#include <unordered_set>
#include "world/player.h"
//! An implementation of player input
class PlayerInput {
   public:
    PlayerInput();
    //! Process function for user input
    void ProcessInput(SDL_Event e, Player& player);

   private:
    enum class Direction { kHorz, kVert };
    //! Represents key manager
    class KeyManager {
       public:
        //! Add negative key
        void AddNegativeKey(std::initializer_list<int> keys) {
            keys_neg_.insert(keys.begin(), keys.end());
        }
        //! Add positive key
        void AddPositiveKey(std::initializer_list<int> keys) {
            keys_pos_.insert(keys.begin(), keys.end());
        }
        //! Return whether the key is negative
        bool IsNegative(int key) {
            return keys_neg_.find(key) != keys_neg_.end();
        }
        //! Return whether the key is positive
        bool IsPositive(int key) {
            return keys_pos_.find(key) != keys_neg_.end();
        }

       private:
        std::unordered_set<int> keys_neg_;
        std::unordered_set<int> keys_pos_;
    };

    //! Interface of the state of player input
    class State {
       public:
        State(Direction dir, std::unique_ptr<KeyManager>& key_manager)
            : direction_(dir), key_manager_(std::move(key_manager)) {}
        State(State&& other)
            : direction_(other.direction_),
              key_manager_(std::move(other.key_manager_)) {}
        State(const State& other) = delete;
        virtual ~State() {}

        //! Set the last input
        void SetLastInput(int input) { last_input_ = input; }

        //! Process input
        virtual std::unique_ptr<State> ProcessInput(Player& player,
                                                    SDL_Event e) {
            return nullptr;
        }
        //! Player enter
        virtual void Enter(Player& player) {}

       protected:
        int last_input_;
        Direction direction_;
        std::unique_ptr<KeyManager> key_manager_;
    };
    //! State when player is not moving
    class NoInputState : public State {
       public:
        NoInputState(Direction dir, std::unique_ptr<KeyManager>& k)
            : State(dir, k) {}
        NoInputState(State&& other) : State(std::move(other)) {}
        std::unique_ptr<State> ProcessInput(Player& player, SDL_Event e);
        void Enter(Player& player);
    };
    //! State when player is moving left
    class NegState : public State {
       public:
        NegState(Direction dir, std::unique_ptr<KeyManager>& k)
            : State(dir, k) {}
        NegState(State&& other) : State(std::move(other)) {}
        std::unique_ptr<State> ProcessInput(Player& player, SDL_Event e);
        void Enter(Player& player);
    };
    //! State when player is moving right
    class PosState : public State {
       public:
        PosState(Direction dir, std::unique_ptr<KeyManager>& k)
            : State(dir, k) {}
        PosState(State&& other) : State(std::move(other)) {}
        std::unique_ptr<State> ProcessInput(Player& player, SDL_Event e);
        void Enter(Player& player);
    };

    std::unique_ptr<State> state_horz_;
    std::unique_ptr<State> state_vert_;
};

#endif
