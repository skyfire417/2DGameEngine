#ifndef PLATFORMER_MENU_H
#define PLATFORMER_MENU_H

#include <SDL2pp/Font.hh>
#include <SDL2pp/Texture.hh>
#include <memory>
#include <vector>
#include "game.h"
#include "resource/base.h"

class Game;
//! The menu that shows up right after game starts
class StartMenu : public Base {
   public:
    //! Read from configure files
    static void ReadConfiguration();
    //! Constructor
    StartMenu();
    //! Return menu width
    int GetTitleWidth();
    //! Return menu height
    int GetHeight();
    //! Render menu
    void Render();
    //! Process menu input
    void ProcessInput(SDL_Event e, Game& game);
    //! Setter for menu position x
    void set_pos_x(int pos_x);
    //! Setter for menu position y
    void set_pos_y(int pos_y);

   private:
    const std::string kAssetDir {"../Assets/"};

    SDL2pp::Font title_font_ {kAssetDir + "Roboto-Regular.ttf", 20};
    SDL2pp::Font body_font_ {kAssetDir + "Roboto-Regular.ttf", 16};
    std::string lang_title_text_ {"Welcome to Platformer Game"};
    std::string lang_prompt_text_ {
        "Please choose a lanugage by typing a number"};

    std::unique_ptr<SDL2pp::Texture> lang_title_texture_;
    std::vector<std::unique_ptr<SDL2pp::Texture>> lang_lines_texture_;

    //! Rendering position x
    int pos_x_ {0};
    //! Rendering position y
    int pos_y_ {0};
    Image::ImageID logo_image_id_ {"team_logo"};
};

#endif
