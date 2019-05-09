#include "utility.h"
#include <SDL.h>
#include <cstdarg>
#include <string>

std::string Utility::ReadFileToString(const char* filename, const char* mode) {
    SDL_RWops* rw = SDL_RWFromFile(filename, mode);
    if (rw == NULL) return NULL;

    Sint64 res_size = SDL_RWsize(rw);
    char* res = (char*)malloc(res_size + 1);

    Sint64 nb_read_total = 0, nb_read = 1;
    char* buf = res;
    while (nb_read_total < res_size && nb_read != 0) {
        nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
        nb_read_total += nb_read;
        buf += nb_read;
    }
    SDL_RWclose(rw);
    if (nb_read_total != res_size) {
        free(res);
        return NULL;
    }

    res[nb_read_total] = '\0';
    return std::string(res);
}

std::string Utility::ToUpper(std::string str) {
    int size = str.size();
    char arr[size + 1];
    int i = 0;
    for (auto& c : str) {
        arr[i] = toupper(c);
        ++i;
    }
    arr[i] = '\0';
    return std::string(arr);
}

std::string Utility::ToUpperFirst(std::string str) {
    int size = str.size();
    char arr[size + 1];
    int i = 0;
    for (auto& c : str) {
        if (i == 0) {
            arr[i] = toupper(c);
        } else {
            arr[i] = c;
        }
        ++i;
    }
    arr[i] = '\0';
    return std::string(arr);
}

std::string Utility::StringFormat(const std::string fmt, ...) {
    int size =
        ((int)fmt.size()) * 2 + 50;  // Use a rubric appropriate for your code
    std::string str;
    va_list ap;
    while (1) {  // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap, fmt);
        int n = vsnprintf((char*)str.data(), size, fmt.c_str(), ap);
        va_end(ap);
        if (n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if (n > -1)        // Needed size returned
            size = n + 1;  // For null char
        else
            size *= 2;  // Guess at a larger size (OS specific)
    }
    return str;
}

Json::Value Utility::CreateSocketCallResponse(bool is_success,
                                              std::string message) {
    Json::Value response;
    response["is_success"] = is_success;
    response["message"] = message;
    return response;
}
