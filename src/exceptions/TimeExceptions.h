//
// Created by ivankhripunov on 29.11.23.
//

#ifndef BALLISTICS2023_TIMEEXCEPTIONS_H
#define BALLISTICS2023_TIMEEXCEPTIONS_H

#include <exception>
#include <string>

namespace Ballistics::Exceptions {

class TimeModuleException : public std::exception {
private:
    std::string message_;

public:
    explicit TimeModuleException(const std::string &message) : message_(message) {}

    [[nodiscard]] const char *what() const noexcept { return message_.c_str(); }
};
}

#endif //BALLISTICS2023_TIMEEXCEPTIONS_H
