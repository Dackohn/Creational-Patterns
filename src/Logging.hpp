#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <iostream>
#include <iomanip>
#include <ctime>
#include "CustomerManagement.hpp"

namespace Logging {
    
    class ConsoleLogger : public CustomerManagement::ILogger {
    public:
        void log(const std::string& message) override {
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);
            std::cout << "[LOG " << std::put_time(&tm, "%H:%M:%S") << "] " << message << "\n";
        }
    };
}

#endif