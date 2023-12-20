//
// Created by ivankhripunov on 29.11.23.
//

#ifndef BALLISTICS2023_EXCEPTIONS_H
#define BALLISTICS2023_EXCEPTIONS_H

#include <exception>
#include <string>

namespace Ballistics::Exceptions {

class Exception : public std::exception {
private:
    std::string message_;

public:
    explicit Exception(const std::string &message) : message_(message) {}

    [[nodiscard]] const char *what() const noexcept { return message_.c_str(); }
};

void sofaErrorHandler(const int status) {
    if (status != 0) {
        throw Ballistics::Exceptions::Exception("SOFA ERROR");
    }
}

void ephemerisErrorHandler(const int status) {
    if (status != 1) {
        throw Ballistics::Exceptions::Exception("EPHEMERIS ERROR");
    }
}

}

#endif //BALLISTICS2023_EXCEPTIONS_H
