#include <SDL2pp/SDL2pp.hh>
#include <exception>
#include <memory>
#include "game.h"
#include "world/world.h"

SDL2pp::SDL sdl(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
SDL2pp::SDLTTF ttf;
SDL2pp::SDLMixer mixer(MIX_INIT_OGG);

int main(int argc, char* argv[]) try {
    const int kWindowWidth = 1280;
    const int kWindowHeight = 720;
    // Due to circular dependencies between Game and World
    // (i.e. both instances of two classes need to keep a reference
    // of each other), we need to instantiate them separately.
    // We can't instantiate both in one constructor.
    auto game = std::make_shared<Game>(kWindowWidth, kWindowHeight);
    auto world = std::make_shared<World>(
        game, Math::Rect<int>(0, 0, kWindowWidth, kWindowHeight));
    game->SetWorld(world);
    game->Run();

} catch (std::exception& e) {
    std::cerr << "Unexpectedly exited: " << e.what() << "\n";
    return 1;
}
