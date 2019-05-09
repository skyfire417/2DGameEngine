#ifndef PLATFORMER_RESOURCE_FONT_H
#define PLATFORMER_RESOURCE_FONT_H

#include <SDL2pp/Font.hh>
#include <string>

//! Interface for a font provider which is provided to a service locator
class Font {
   public:
    enum class ID { kInfo, kWinLose, kLevelText };
    virtual ~Font() {}
    //! Put Font into Map
    virtual void PutFont(Font::ID font_id, const std::string& file_path,
                         int size) = 0;
    //! Get Font by ID
    virtual SDL2pp::Font& GetFont(Font::ID font_id) = 0;
};

#endif
