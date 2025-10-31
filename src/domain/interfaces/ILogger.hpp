#ifndef I_LOGGER_HPP
#define I_LOGGER_HPP

#include <string>

namespace domain {

class ILogger {
public:
    virtual ~ILogger() = default;
    virtual void log(const std::string& message) = 0;
};

} // namespace domain

#endif
