#ifndef PLATFORMER_WORLD_WORLD_H
#define PLATFORMER_WORLD_WORLD_H

#include <SDL2pp/Rect.hh>
#include <SDL2pp/Texture.hh>
#include <memory>
#include <vector>
#include "game.h"
#include "object/border_manager.h"
#include "object/tile_manager.h"
#include "physics/collision.h"
#include "resource/base.h"
#include "tinymath.h"
#include "world/camera_system.h"
#include "world/level.h"
#include "world/level_manager.h"
#include "world/player.h"
#include "world/player_input.h"

class Game;  // Forward declaration for circular include
class CameraSystem;

//! Manages the objects in the game
/*!
 * It also controls game progress by managing different levels,
 * interaction among different objects.
 * It also keeps track of current state of game and shows
 * proper messages to users at certain circumstances.
 */
class World : public Base {
   public:
    //! Interface for each state of the world
    class State {
       public:
        virtual ~State() {}

        //! Update world
        virtual std::unique_ptr<State> Update(World& world, int ms_passed) {
            return nullptr;
        }
        //! Render to the screen
        virtual void Render(World& world) {}
        //! Process input
        virtual std::unique_ptr<State> ProcessInput(World& world, SDL_Event e) {
            return nullptr;
        }
        //! Enter the world
        virtual void Enter(World& world) {}
    };
    //! State when world is just instantiated
    class InitState : public State {};
    //! State when a level has started
    class StartedState : public State {
       public:
        std::unique_ptr<State> Update(World& world, int ms_passed);
        std::unique_ptr<State> ProcessInput(World& world, SDL_Event e);
    };
    //! State when player has lost all their lives
    class LostGameState : public State {
       public:
        std::unique_ptr<State> ProcessInput(World& world, SDL_Event e);
        void Render(World& world);
    };
    //! State when player has lost in a level
    class LostLevelState : public State {
       public:
        std::unique_ptr<State> Update(World& world, int ms_passed);
    };
    //! State when player has won in a level
    class WonState : public State {
       public:
        void Render(World& world);
        std::unique_ptr<State> ProcessInput(World& world, SDL_Event e);
    };

    World(std::shared_ptr<Game> game, Math::Rect<int> area);
    //! Forbids copying
    World(const World&) = delete;
    //! Forbids copying
    World& operator=(const World&) = delete;
    //! Returns a rectangle representing game area
    Math::Rect<int> area();
    //! Update world
    void Update(int ms_passed);
    //! Render world
    void Render(int ms_passed, float fps);
    //! Processes user input
    void ProcessInput(SDL_Event e);
    //! Processes user mouse input
    /*!
     * When any of the events happens, this method is called:
     * - SDL_MOUSEMOTION
     * - SDL_MOUSEBUTTONDOWN
     * - SDL_MOUSEBUTTONUP
     * - SDL_FINGERMOTION
     * - SDL_FINGERUP
     * - SDL_FINGERDOWN
     * Mouse position is passed in as x and y
     */
    void ProcessMouseInput(SDL_Event e);
    //! Processes socket method call
    //! May throw runtime_error
    Json::Value ProcessSocketCall(std::string action_name, Json::Value args);
    //! Starts game by starting the first level
    void Start();

    void SetCurTilemap(std::shared_ptr<TileManager> map) { cur_tilemap_ = map; }

   private:
    //! General function for showing a message if level ends
    void RenderLevelEndMessage(std::string title, std::string body);
    //! Shows level message if player wins a level
    void RenderLevelWonMessage();
    //! Shows level message if player wins the game
    void RenderGameWonMessage();
    //! Shows level message if player loses the game
    void RenderGameLostMessage();
    //! Initialize objects before starting the next level
    void InitCurLevel();
    //! Check if player wins the level
    bool CheckIfWinLevel();

    //! Guard for single instance
    static bool instantiated_;
    std::unique_ptr<State> state_ {std::make_unique<InitState>()};
    std::shared_ptr<Collision> collision_ {std::make_shared<Collision>()};
    //! The area in the window that reserved for stuff related to game
    Math::Rect<int> area_;
    //! The borders that constrains where the player can go around
    Math::Rect<int> area_main_;
    //! The area for player and level info etc.
    Math::Rect<int> area_info_;
    std::shared_ptr<Game> game_;
    std::shared_ptr<LevelManager> level_manager_ {
        std::make_shared<LevelManager>()};
    std::shared_ptr<TileManager> cur_tilemap_ {
        std::make_shared<TileManager>(collision_)};
    std::shared_ptr<CameraSystem> camera_system_;
    std::shared_ptr<BorderManager> border_manager_ {
        std::make_shared<BorderManager>()};

    std::shared_ptr<Player> player_ {std::make_shared<Player>()};
    PlayerInput player_input_;
    //! Area for win/lose message box
    Math::Rect<int> area_win_lose_ {0, 0, 400, 200};
};
#endif
