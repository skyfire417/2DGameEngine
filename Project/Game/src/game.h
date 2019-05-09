#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <SDL.h>
#include <SDL2pp/Window.hh>
#include <memory>
#include <unordered_map>
#include "resource/base.h"
#include "start_menu.h"
#include "world/world.h"

class World;  // Forward declaration for circular include
class StartMenu;

//! Represents the whole game that starts a game, handles user input, etc.
/*!
 * It keeps a World instance. Only one game instance is allowed to be
 * instantiated.
 */
class Game : public Base {
   public:
    //! Enum for game state
    enum class State {
        kInstantiated, /*!< After instantiated before started */
        kStarted,      /*!< After game has started before the end */
    };

    //! Initialized a game with specific window width and height
    Game(int width, int height);
    //! Forbids copying
    Game(const Game &) = delete;
    ~Game();
    //! Forbids copying
    Game &operator=(const Game &) = delete;

    //! Sets a World
    /*!
     * Due to circular dependency, we have to pass in a World
     * instance after instantiating both Game and World.
     */
    void SetWorld(std::shared_ptr<World> world);
    //! Runs the game
    void Run();
    //! Gets the language set
    Text::Lang lang();
    //! Sets the language before game starts
    void set_lang(Text::Lang lang);
    //! Starts the game
    /*! Before the game starts, user cannot play. */
    void Start();
    //! Getter for game state
    State state();
    //! Wins the game
    void Win();
    //! Loses the game
    void Lose();
    //! Gets window's width
    int GetWindowWidth();
    //! Gets window's height
    int GetWindowHeight();

   private:
    //! Guard for single instance
    static bool instantiated_;
    //! Game quits after this flag is set to true
    bool quit_ {false};
    //! Game state
    State state_ {State::kInstantiated};
    //! Current language
    Text::Lang lang_ {Text::Lang::kEnglish};
    std::shared_ptr<World> world_;
    std::unique_ptr<SDL2pp::Window> window_;
    std::unique_ptr<StartMenu> start_menu_;
    int start_menu_pos_x_ {0};
    int start_menu_pos_y_ {0};

    //! Update the scene
    void Update(int ms_passed);
    //! Render to the screen
    void Render(int ms_passed, float fps);
    //! Handler for user input
    void ProcessInput(SDL_Event event);

    //! Socket filename
    static const std::string kSocketFilePath;
    //! Socket buffer size
    static const int kSocketBufferSize = 2048;
    //! Socket file descriptor
    int socket_filedescriptor;
    //! Socket file path
    std::string socket_filepath;

    //! Handles failed system call
    /*!
     * Throws system_error when error happens.
     */
    int GuardSysCall(int ret, const std::string &msg);
    //! Polls message from socket
    /*!
     * Throws system_error when error happens.
     */
    void PollSocket();
    //! Helper method to construct an error message for system call
    std::string CreateErrMsgForSysCall(const std::string &msg);
    //! Handler for socket message
    void ProcessSocketMessage(char *message, int client_socket_filedescriptor);
    //! Reads all configuration
    void ReadAllConfiguration();
    //! Creates a JSON response to socket call
    Json::Value CreateSocketCallResponse(bool is_success, std::string message);
};

#endif
