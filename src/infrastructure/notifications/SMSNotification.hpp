#ifndef SMS_NOTIFICATION_HPP
#define SMS_NOTIFICATION_HPP

#include <string>
#include <iostream>
#include "../../domain/interfaces/INotificationChannel.hpp"

namespace infrastructure {

class SMSNotification : public domain::INotificationChannel {
public:
    bool send(const std::string& recipient,
              const std::string& message) override 
    {
        std::cout << "[SMS] Sending to " << recipient << ":\n"
                  << message << std::endl;
        return true;
    }

    std::string getChannelName() const override {
        return "SMS";
    }
};

} // namespace infrastructure

#endif
