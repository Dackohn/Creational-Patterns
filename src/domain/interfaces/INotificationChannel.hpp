#ifndef I_NOTIFICATION_CHANNEL_HPP
#define I_NOTIFICATION_CHANNEL_HPP

#include <string>

namespace domain {

class INotificationChannel {
public:
    virtual ~INotificationChannel() = default;

    virtual bool send(const std::string& recipient,
                      const std::string& message) = 0;

    virtual std::string getChannelName() const = 0;
};

} // namespace domain

#endif
