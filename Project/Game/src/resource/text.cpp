#include "resource/text.h"

const std::vector<Text::Lang::Value> Text::Lang::All {Text::Lang::kEnglish,
                                                      Text::Lang::kSpanish};

std::unordered_map<std::string, Text::Lang::Value>
    Text::Lang::from_string_map_ {{"eng", Text::Lang::kEnglish},
                                  {"es", Text::Lang::kSpanish}};
std::unordered_map<Text::Lang::Value, std::string> Text::Lang::to_string_map_ {
    {Text::Lang::kEnglish, "English"},
    {Text::Lang::kSpanish, "Spanish"},
};

std::unordered_map<std::string, Text::ID::Value> Text::ID::string_map_ {
    {"LifeCountLabel", Text::ID::kLifeCountLabel},
    {"LevelNumLabel", Text::ID::kLevelNumLabel},
    {"CoinLabel", Text::ID::kCoinLabel},
    {"LevelWinMsg", Text::ID::kLevelWinMsg},
    {"GameWinMsg", Text::ID::kGameWinMsg},
    {"GameLoseMsg", Text::ID::kGameLoseMsg},
    {"NextLevelPrompt", Text::ID::kNextLevelPrompt},
    {"QuitGamePrompt", Text::ID::kQuitGamePrompt},
};

Text::~Text() {}
