#ifndef PLATFORMER_RESOURCE_GAME_TEXT_H
#define PLATFORMER_RESOURCE_GAME_TEXT_H

#include <json/json.h>
#include <map>
#include <string>
#include "resource/base.h"
#include "resource/text.h"

//! An implementation of Text
class GameText : public Text, public Base {
   public:
    GameText();
    GameText(const GameText &) = delete;
    GameText &operator=(const GameText &) = delete;
    //! Loads localization texts from a file
    void LoadText();
    //! Returns a text in specific language
    std::string GetText(Text::ID id, Text::Lang text);

   private:
    //! Tests if text file's content is valid
    bool ValidateTextFile(Json::Value root);

    const std::string text_file_path_ {"../Assets/texts.json"};
    bool instantiated_ {false};
    std::map<std::pair<Text::ID::Value, Text::Lang::Value>, std::string>
        text_map_;
};

#endif
