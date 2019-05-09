#include "start_menu.h"
#include "resource/image.h"

void StartMenu::ReadConfiguration() {
    auto& image_provider = GetImageProvider();
    auto& config_provider = GetConfigProvider();

    auto logo = config_provider.GetValue("meta")["logo"];
    auto ret = config_provider.ParseImageValue(logo["image"]);
    auto image_id = std::get<0>(ret);
    auto image_filename = std::get<1>(ret);
    auto image_rects = std::get<2>(ret);
    image_provider.PutImage(image_id, image_filename, image_rects[0]);
}

StartMenu::StartMenu() {
    SDL2pp::Renderer& renderer {GetImageProvider().GetRenderer()};

    lang_title_texture_ = std::make_unique<SDL2pp::Texture>(
        renderer, title_font_.RenderText_Blended(
                      lang_title_text_, SDL_Color {0xFF, 0xFF, 0xFF, 0xFF}));

    lang_lines_texture_ = std::vector<std::unique_ptr<SDL2pp::Texture>>();
    lang_lines_texture_.push_back(std::make_unique<SDL2pp::Texture>(
        renderer, body_font_.RenderText_Blended(
                      lang_prompt_text_, SDL_Color {0xFF, 0xFF, 0xFF, 0xFF})));
    for (int i = 0; i < Text::Lang::All.size(); ++i) {
        Text::Lang l = Text::Lang::All[i];
        lang_lines_texture_.push_back(std::make_unique<SDL2pp::Texture>(
            renderer, body_font_.RenderText_Blended(
                          std::to_string(i + 1) + ". " + l.to_string(),
                          SDL_Color {0xFF, 0xFF, 0xFF, 0xFF})));
    }
}

int StartMenu::GetHeight() {
    int height = lang_title_texture_->GetHeight();
    for (auto& t : lang_lines_texture_) {
        height += t->GetHeight();
    }
    return height * 2;
}

int StartMenu::GetTitleWidth() { return lang_title_texture_->GetWidth(); }

void StartMenu::ProcessInput(SDL_Event e, Game& game) {
    if (e.type == SDL_KEYDOWN) {
        int selected = e.key.keysym.sym - SDLK_1;
        if (selected >= 0 && selected < Text::Lang::All.size()) {
            game.set_lang(Text::Lang::All[selected]);
            game.Start();
        }
    }
}

void StartMenu::Render() {
    int pos_x = pos_x_;
    int pos_y = pos_y_;
    SDL2pp::Renderer& renderer {GetImageProvider().GetRenderer()};

    // Render title
    renderer.Copy(*lang_title_texture_, SDL2pp::NullOpt,
                  SDL2pp::Rect(pos_x, pos_y, lang_title_texture_->GetWidth(),
                               lang_title_texture_->GetHeight()));

    // Render language selection
    int line_width = lang_lines_texture_[0]->GetWidth();
    pos_x += (lang_title_texture_->GetWidth() - line_width) / 2;
    pos_y += lang_title_texture_->GetHeight() * 2;
    // Align body to the center of title
    for (auto& t : lang_lines_texture_) {
        renderer.Copy(
            *t, SDL2pp::NullOpt,
            SDL2pp::Rect(pos_x, pos_y, t->GetWidth(), t->GetHeight()));
        pos_y += t->GetHeight() * 2;
    }
    // Render logo
    auto logo_dimen = GetImageProvider().GetImageDimen(logo_image_id_);
    GetImageProvider().RenderImage(
        logo_image_id_,
        {pos_x_ + (lang_title_texture_->GetWidth() - logo_dimen.first) / 2.0,
         (double)pos_y + lang_title_texture_->GetHeight()});
}

void StartMenu::set_pos_x(int pos_x) { pos_x_ = pos_x; }

void StartMenu::set_pos_y(int pos_y) { pos_y_ = pos_y; }
