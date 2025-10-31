#ifndef NOTIFICATION_CHANNELS_HPP
#define NOTIFICATION_CHANNELS_HPP

#include <iostream>
#include <string>
#include "TicketManagement.hpp"

namespace NotificationChannels {
    
    class EmailNotification : public TicketManagement::INotificationChannel {
    public:
        bool send(const std::string& recipient, const std::string& message) override {
            std::cout << "[EMAIL] To: " << recipient << "\n";
            std::cout << "[EMAIL] Message: " << message << "\n\n";
            return true;
        }
        
        std::string getChannelName() const override {
            return "Email";
        }
    };
    
    class SMSNotification : public TicketManagement::INotificationChannel {
    public:
        bool send(const std::string& recipient, const std::string& message) override {
            std::cout << "[SMS] To: " << recipient << "\n";
            std::cout << "[SMS] Message: " << message << "\n\n";
            return true;
        }
        
        std::string getChannelName() const override {
            return "SMS";
        }
    };
    
    class PushNotification : public TicketManagement::INotificationChannel {
    public:
        bool send(const std::string& recipient, const std::string& message) override {
            std::cout << "[PUSH] To: " << recipient << "\n";
            std::cout << "[PUSH] Message: " << message << "\n\n";
            return true;
        }
        
        std::string getChannelName() const override {
            return "Push";
        }
    };
}

#endif