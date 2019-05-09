#ifndef PLATFORMER_RESOURCE_BASE_H
#define PLATFORMER_RESOURCE_BASE_H

#include <SDL2pp/Mixer.hh>
#include <memory>
#include <utility>
#include "resource/audio.h"
#include "resource/config.h"
#include "resource/font.h"
#include "resource/image.h"
#include "resource/text.h"

//! Service locator %base class
/*!
 * It provides shared resources, such as images, font, audio, etc.
 *
 * Implemented using Service Locator design pattern. Only classes that
 * inherit this class can get access to service locator.
 */
class Base {
   public:
    virtual ~Base();

   protected:
    //! Provides a mixer
    static void Provide(std::unique_ptr<SDL2pp::Mixer> mixer);
    //! Returns the mixer managed by this class
    static SDL2pp::Mixer& GetMixer();

    //! Provides an image provider to this locator.
    static void Provide(std::unique_ptr<Image> provider);
    //! Gets an image provider from this locator.
    static Image& GetImageProvider();

    //! Provides a font provider to this locator
    static void Provide(std::unique_ptr<Font> provider);
    //! Gets a font provider from this locator
    static Font& GetFontProvider();

    //! Provides a audio provider to this locator
    static void Provide(std::unique_ptr<Audio> provider);
    //! Gets a audio provider from this locator
    static Audio& GetAudioProvider();

    //! Provide an instruction provider to this locator
    static void Provide(std::unique_ptr<Text> provider);
    //! Gets an instruction provider from this locator
    static Text& GetTextProvider();

    //! Provide an instruction provider to this locator
    static void Provide(std::unique_ptr<Config> provider);
    //! Get configure provider
    static Config& GetConfigProvider();

   private:
    static std::shared_ptr<Image> null_image_provider_;
    static std::shared_ptr<Image> image_provider_;
    static std::unique_ptr<Font> font_provider_;
    static std::unique_ptr<Audio> audio_provider_;
    static std::unique_ptr<SDL2pp::Mixer> mixer_;
    static std::unique_ptr<Text> text_provider_;
    static std::unique_ptr<Config> config_provider_;
};

#endif
