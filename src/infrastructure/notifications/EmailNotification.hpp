#ifndef EMAIL_NOTIFICATION_HPP
#define EMAIL_NOTIFICATION_HPP

#include <string>
#include <iostream>
#include "../../domain/interfaces/INotificationChannel.hpp"

namespace infrastructure {

class EmailNotification : public domain::INotificationChannel {
public:
    bool send(const std::string& recipient,
              const std::string& message) override 
    {
        std::cout << "[Email] Sending to " << recipient << ":\n"
                  << message << std::endl;
        return true; 
    }

    std::string getChannelName() const override {
        return "Email";
    }
};

} // namespace infrastructure

#endif
