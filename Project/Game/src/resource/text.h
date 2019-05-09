#ifndef PLATFORMER_RESOURCE_TEXT_H
#define PLATFORMER_RESOURCE_TEXT_H

#include <string>
#include <unordered_map>
#include <vector>

//! Interface for a text provider which is provided to a service locator
class Text {
   public:
    //! Enum wrapper class for Language options
    class Lang {
       public:
        enum Value { kEnglish, kSpanish };
        Lang(Value lang) : value_(lang) {}

        static Lang from_string(std::string str) {
            return from_string_map_[str];
        }
        std::string to_string() { return to_string_map_[value_]; }
        Value value() { return value_; }
        static const std::vector<Value> All;

       private:
        Value value_;
        static std::unordered_map<std::string, Value> from_string_map_;
        static std::unordered_map<Value, std::string> to_string_map_;
    };
    //! Enum wrapper class for text %ID
    class ID {
       public:
        enum Value {
            kLifeCountLabel,
            kLevelNumLabel,
            kCoinLabel,
            kLevelWinMsg,
            kGameWinMsg,
            kGameLoseMsg,
            kNextLevelPrompt,
            kQuitGamePrompt
        };
        ID(Value value) : value_(value) {}
        Value value() { return value_; }
        static ID from_string(std::string str) { return string_map_[str]; }

       private:
        Value value_;
        static std::unordered_map<std::string, Value> string_map_;
    };
    virtual ~Text();

    //! Load localization text into this provider to be managed by it
    virtual void LoadText() = 0;

    //! Gets a localized text
    virtual std::string GetText(Text::ID text_content,
                                Text::Lang text_lang) = 0;
};

#endif
