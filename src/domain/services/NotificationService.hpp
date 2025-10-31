#ifndef NOTIFICATION_SERVICE_HPP
#define NOTIFICATION_SERVICE_HPP

#include <memory>
#include <string>
#include <vector>

#include "../interfaces/INotificationChannel.hpp"
#include "../interfaces/ILogger.hpp"

namespace domain {

class NotificationService {
private:
    std::vector<std::shared_ptr<INotificationChannel>> channels;
    std::shared_ptr<ILogger> logger;

    NotificationService(std::shared_ptr<ILogger> log)
        : logger(log) {}

public:
    static NotificationService& getInstance(std::shared_ptr<ILogger> logger = nullptr) {
        static NotificationService instance(logger);
        return instance;
    }

    void addChannel(std::shared_ptr<INotificationChannel> channel) {
        channels.push_back(channel);
        if (logger) {
            logger->log("Added notification channel: " + channel->getChannelName());
        }
    }

    void notify(const std::string& recipient, const std::string& message) {
        for (auto& channel : channels) {
            bool ok = channel->send(recipient, message);
            if (ok && logger) {
                logger->log("Notification sent via " + channel->getChannelName() +
                            " to " + recipient);
            }
        }
    }
};

} // namespace domain

#endif
