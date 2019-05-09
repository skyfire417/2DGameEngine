#include "game.h"
#include <SDL.h>
#include <SDL2pp/Renderer.hh>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <system_error>
#include <unistd.h>
#include "object/object.h"
#include "resource/game_audio.h"
#include "resource/game_config.h"
#include "resource/game_font.h"
#include "resource/game_image.h"
#include "resource/game_text.h"
#include "utility.h"

bool Game::instantiated_ {false};
const std::string Game::kSocketFilePath {"/tmp/gamecastle-gamemaker"};


Game::Game(int width, int height) {
    assert(!instantiated_);

    window_ = std::make_unique<SDL2pp::Window>(
        "Platformer - Game Castle", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

    Provide(std::make_unique<SDL2pp::Mixer>(MIX_DEFAULT_FREQUENCY,
                                            MIX_DEFAULT_FORMAT, 2, 4096));
    Provide(std::make_unique<GameImage>());
    GetImageProvider().ProvideRenderer(std::make_unique<SDL2pp::Renderer>(
        *window_, -1, SDL_RENDERER_ACCELERATED));
    Provide(std::make_unique<GameFont>());
    Provide(std::make_unique<GameAudio>());
    Provide(std::make_unique<GameText>());
    Provide(std::make_unique<GameConfig>());

    /// Read configuration
    ReadAllConfiguration();
    /// Load all resources upfront
    // Load music and sound
    GetAudioProvider().PutMusic(Audio::ID::kBackground, "Puzzle-Dreams.ogg");
    GetAudioProvider().PutSoundEffect(Audio::ID::kCoin, "collect-coin.ogg");
    // Load fonts
    GetFontProvider().PutFont(Font::ID::kInfo, "Roboto-Regular.ttf", 18);
    GetFontProvider().PutFont(Font::ID::kWinLose, "Roboto-Regular.ttf", 20);
    GetFontProvider().PutFont(Font::ID::kLevelText, "Roboto-Regular.ttf", 40);
    // Load texts
    Text& text_provider = GetTextProvider();
    text_provider.LoadText();

    // Initialize start menu
    start_menu_ = std::make_unique<StartMenu>();
    start_menu_pos_x_ =
        (window_->GetWidth() - start_menu_->GetTitleWidth()) / 2;
    start_menu_pos_y_ =
        (int)(0.4 * window_->GetHeight()) - (start_menu_->GetHeight()) / 2;

    // Initialize socket
    socket_filedescriptor = GuardSysCall(socket(AF_UNIX, SOCK_STREAM, 0),
                                         "Cannot creat UNIX socket");
    int socket_file_flags = GuardSysCall(fcntl(socket_filedescriptor, F_GETFL),
                                         "Cannot get flags of UNIX socket");
    GuardSysCall(
        fcntl(socket_filedescriptor, F_SETFL, socket_file_flags | O_NONBLOCK),
        "Cannot set UNIX socket to be non-blocking");
    socket_filepath = kSocketFilePath;
    unlink(socket_filepath.c_str());
    sockaddr_un binding_addr;
    strcpy(binding_addr.sun_path, socket_filepath.c_str());
    binding_addr.sun_family = AF_UNIX;
    GuardSysCall(bind(socket_filedescriptor, (sockaddr*)&binding_addr,
                      sizeof(sockaddr_un)),
                 "Cannot bind to UNIX socket");
    GuardSysCall(listen(socket_filedescriptor, 100), "Cannot listen to socket");
    printf("[Game][Constructor] Started to listen to socket: %s\n",
           socket_filepath.c_str());

    instantiated_ = true;
}

Game::~Game() {
    close(socket_filedescriptor);
    unlink(socket_filepath.c_str());
}

void Game::SetWorld(std::shared_ptr<World> world) { world_ = std::move(world); }

void Game::Run() {
    // Main loop
    SDL_Event e;

    const int kMSPerUpdate = 2;
    const int kFPSCapped = 60;
    const int kTicksPerFrame = 1000 / kFPSCapped;
    int start_time = SDL_GetTicks();
    int last_time = start_time;
    int current_time;
    int lag = 0;
    int frame_count = 0;
    float realtime_fps;
    char* socket_msg;
    // While application is running
    while (!quit_) {
        current_time = SDL_GetTicks();
        int ticks = current_time - last_time;
        // smaller the ticks, faster the rendering
        if (ticks < kTicksPerFrame) {
            SDL_Delay(kTicksPerFrame - ticks);
            current_time = SDL_GetTicks();
        }

        realtime_fps = 1 / ((current_time - last_time) / 1000.f);
        // avgFPS = frame_count / ((current_time - start_time) / 1000.f);

        // How much real time has passed during last frame
        lag += current_time - last_time;
        last_time = current_time;

        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            ProcessInput(e);
        }

        // Handle socket message
        PollSocket();

        int ms_passed = 0;
        while (lag >= kMSPerUpdate) {
            Update(kMSPerUpdate);
            lag -= kMSPerUpdate;
            ms_passed += kMSPerUpdate;
        }
        Render(ms_passed, realtime_fps);

        frame_count += 1;
    }
}

Text::Lang Game::lang() { return lang_; }

void Game::set_lang(Text::Lang lang) { lang_ = lang; }

void Game::Start() {
    assert(world_ != nullptr);

    state_ = Game::State::kStarted;
    world_->Start();
}

Game::State Game::state() { return state_; }

void Game::Win() { quit_ = true; }

void Game::Lose() { quit_ = true; }

int Game::GetWindowWidth() { return window_->GetWidth(); }

int Game::GetWindowHeight() { return window_->GetHeight(); }

void Game::Update(int ms_passed) {
    // Only update world after game has started
    if (state_ == Game::State::kStarted) {
        world_->Update(ms_passed);
    } else if (state_ == Game::State::kInstantiated) {
        start_menu_->set_pos_x(start_menu_pos_x_);
        start_menu_->set_pos_y(start_menu_pos_y_);
    }
}

void Game::Render(int ms_passed, float fps) {
    SDL2pp::Renderer& renderer = GetImageProvider().GetRenderer();
    renderer.Clear();

    // Only render the world after game has started
    if (state_ == Game::State::kStarted) {
        world_->Render(ms_passed, fps);
    } else if (state_ == Game::State::kInstantiated) {
        // Show the start menu before game starts
        start_menu_->Render();
    }

    renderer.Present();
}

void Game::ProcessInput(SDL_Event e) {
    if (e.type == SDL_QUIT ||
        (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)) {
        // When "Q" is pressed, quit
        quit_ = true;
    } else if (e.type >= SDL_KEYDOWN && e.type <= SDL_MOUSEMOTION) {
        // Only pass keyboard events to world
        if (state_ == Game::State::kStarted) {
            world_->ProcessInput(e);
        } else if (state_ == Game::State::kInstantiated) {
            start_menu_->ProcessInput(e, *this);
        }
    } else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN ||
               e.type == SDL_MOUSEBUTTONUP || e.type == SDL_FINGERMOTION ||
               e.type == SDL_FINGERUP || e.type == SDL_FINGERDOWN) {
        world_->ProcessMouseInput(e);
    }
}

void Game::ProcessSocketMessage(char* msg, int client_socket_filedescriptor) {
    printf("[Game][ProcessSocketMessage] Got message from socket: %s\n", msg);
    Json::Value message;
    std::istringstream(msg) >> message;

    Json::Value response;
    if (message.isMember("action_name") && message.isMember("args")) {
        response = world_->ProcessSocketCall(message["action_name"].asString(),
                                             message["args"]);
    } else {
        response = Utility::CreateSocketCallResponse(
            false,
            "Malformed request: expecting "
            "'action_name' and 'args' fields");
    }
    Json::StreamWriterBuilder builder;
    const std::string response_str = Json::writeString(builder, response);
    send(client_socket_filedescriptor, response_str.c_str(),
         response_str.length(), 0);
}

int Game::GuardSysCall(int ret, const std::string& msg) {
    if (ret == -1) {
        std::string err_msg = CreateErrMsgForSysCall(msg);
        fprintf(stderr, "%s\n", err_msg.c_str());
        throw std::system_error(errno, std::system_category(), err_msg);
    }
    return ret;
}

void Game::PollSocket() {
    // Socket polling state
    enum SocketState { kConnect, kRead, kError };
    // Socket state
    static SocketState socket_state_ = SocketState::kConnect;
    static char buffer[kSocketBufferSize];
    static int client_socket_fd;

    switch (socket_state_) {
        case SocketState::kConnect:
            memset(buffer, 0, kSocketBufferSize);
            client_socket_fd = accept(socket_filedescriptor, nullptr, nullptr);
            if (client_socket_fd == -1) {
                if (errno != EWOULDBLOCK) {
                    fprintf(stderr, "%s\n",
                            CreateErrMsgForSysCall(
                                "Error when connecting to socket")
                                .c_str());
                    socket_state_ = SocketState::kError;
                }
            } else {
                socket_state_ = SocketState::kRead;
            }
            break;
        case SocketState::kRead: {
            int ret_read = read(client_socket_fd, buffer, kSocketBufferSize);
            if (ret_read == -1) {
                if (errno != EAGAIN) {
                    fprintf(
                        stderr, "%s\n",
                        CreateErrMsgForSysCall("Error when reading from socket")
                            .c_str());
                    socket_state_ = SocketState::kError;
                }
            } else {
                ProcessSocketMessage(buffer, client_socket_fd);
                close(client_socket_fd);
                socket_state_ = SocketState::kConnect;
            }
            break;
        }
        case SocketState::kError:
            // Retry when error happens
            socket_state_ = SocketState::kConnect;
            break;
    }
}

std::string Game::CreateErrMsgForSysCall(const std::string& msg) {
    extern int errno;
    return msg + ": " + std::string(strerror(errno));
}

void Game::ReadAllConfiguration() {
    StartMenu::ReadConfiguration();
    LevelManager::ReadConfiguration();
    FallableObject::ReadConfiguration();
    TileManager::ReadConfiguration();
    Player::ReadConfiguration();
}
