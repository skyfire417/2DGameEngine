#include "resource/base.h"
#include "resource/null_image.h"

std::shared_ptr<Image> Base::null_image_provider_ =
    std::make_shared<NullImage>();
std::shared_ptr<Image> Base::image_provider_ = null_image_provider_;
std::unique_ptr<Font> Base::font_provider_;
std::unique_ptr<Audio> Base::audio_provider_;
std::unique_ptr<SDL2pp::Mixer> Base::mixer_;
std::unique_ptr<Text> Base::text_provider_;
std::unique_ptr<Config> Base::config_provider_;

Base::~Base() {}

void Base::Provide(std::unique_ptr<Image> provider) {
    image_provider_ = std::move(provider);
}

SDL2pp::Mixer& Base::GetMixer() { return *mixer_; }

void Base::Provide(std::unique_ptr<SDL2pp::Mixer> mixer) {
    mixer_ = std::move(mixer);
}

Image& Base::GetImageProvider() { return *image_provider_; }

void Base::Provide(std::unique_ptr<Font> provider) {
    font_provider_ = std::move(provider);
}

Font& Base::GetFontProvider() { return *font_provider_; }

void Base::Provide(std::unique_ptr<Audio> provider) {
    audio_provider_ = std::move(provider);
}

Audio& Base::GetAudioProvider() { return *audio_provider_; }

void Base::Provide(std::unique_ptr<Text> provider) {
    text_provider_ = std::move(provider);
}

Text& Base::GetTextProvider() { return *text_provider_; }

void Base::Provide(std::unique_ptr<Config> provider) {
    config_provider_ = std::move(provider);
}

Config& Base::GetConfigProvider() { return *config_provider_; }
