#ifndef PLATFORMER_UTILITY_H
#define PLATFORMER_UTILITY_H

#include <json/json.h>
#include <string>

//! A utility library for common operations in the program
namespace Utility {
//! Represents a rectangle.
struct Rect {
    double x, y, w, h;
};
//! Read a string from file
std::string ReadFileToString(const char* filename, const char* mode);
//! Change the text into upper case
std::string ToUpper(std::string str);
//! Change the first letter of the text into upper case
std::string ToUpperFirst(std::string str);
//! Regulate string format
std::string StringFormat(const std::string fmt, ...);
//! Create socket call responses
Json::Value CreateSocketCallResponse(bool is_success, std::string message);

};  // namespace Utility

#endif
