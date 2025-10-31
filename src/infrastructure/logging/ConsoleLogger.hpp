#ifndef CONSOLE_LOGGER_HPP
#define CONSOLE_LOGGER_HPP

#include <iostream>
#include <string>
#include "../../domain/interfaces/ILogger.hpp"

namespace infrastructure {

class ConsoleLogger : public domain::ILogger {
public:
    void log(const std::string& message) override {
        std::cout << "[LOG] " << message << std::endl;
    }
};

} // namespace infrastructure

#endif
