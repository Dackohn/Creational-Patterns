#ifndef PUSH_NOTIFICATION_HPP
#define PUSH_NOTIFICATION_HPP

#include <string>
#include <iostream>
#include "../../domain/interfaces/INotificationChannel.hpp"

namespace infrastructure {

class PushNotification : public domain::INotificationChannel {
public:
    bool send(const std::string& recipient,
              const std::string& message) override 
    {
        std::cout << "[Push Notification] Sending to " << recipient << ":\n"
                  << message << std::endl;
        return true;
    }

    std::string getChannelName() const override {
        return "Push Notification";
    }
};

} // namespace infrastructure

#endif
