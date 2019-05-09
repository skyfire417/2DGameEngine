#include "resource/game_text.h"
#include <sstream>
#include <vector>
#include "utility.h"

GameText::GameText() {
    assert(!instantiated_);
    instantiated_ = true;
}

void GameText::LoadText() {
    Json::Value root;
    std::istringstream(
        Utility::ReadFileToString(text_file_path_.c_str(), "rb")) >>
        root;
    assert(ValidateTextFile(root));

    const Json::Value langs = root["languages"];
    const Json::Value texts = root["texts"];

    for (int i = 0; i < texts.size(); ++i) {
        const Json::Value text = texts[i];
        for (int j = 0; j < langs.size(); ++j) {
            std::string lang_str = langs[j].asString();
            text_map_[std::make_pair<Text::ID::Value, Text::Lang::Value>(
                Text::ID::from_string(text["id"].asString()).value(),
                Text::Lang::from_string(lang_str).value())] =
                text["text"][lang_str].asString();
        }
    }
}

std::string GameText::GetText(Text::ID id, Text::Lang lang) {
    return text_map_[std::make_pair<Text::ID::Value, Text::Lang::Value>(
        id.value(), lang.value())];
}

bool GameText::ValidateTextFile(Json::Value root) {
    const Json::Value langs = root["languages"];
    if (langs.size() != Text::Lang::All.size()) {
        return false;
    }
    return true;
}
