#include <iostream>
#include <memory>

// Client
#include "client/CLI.hpp"

// Domain
#include "domain/services/CustomerService.hpp"
#include "domain/services/TicketService.hpp"
#include "domain/services/NotificationService.hpp"

// Infrastructure - repositories
#include "infrastructure/repositories/InMemoryCustomerRepository.hpp"
#include "infrastructure/repositories/InMemoryTicketRepository.hpp"

// Infrastructure - logging
#include "infrastructure/logging/ConsoleLogger.hpp"

// Infrastructure - notifications
#include "infrastructure/notifications/EmailNotification.hpp"
#include "infrastructure/notifications/SMSNotification.hpp"
#include "infrastructure/notifications/PushNotification.hpp"

int main() {
    // Logger
    auto logger = std::make_shared<infrastructure::ConsoleLogger>();

    // Repositories (Singletons)
    auto& customerRepo = infrastructure::InMemoryCustomerRepository::getInstance();
    auto& ticketRepo   = infrastructure::InMemoryTicketRepository::getInstance();

    // Notification service (Singleton)
    auto& notificationService = domain::NotificationService::getInstance(logger);

    // Add notification channels
    notificationService.addChannel(
        std::make_shared<infrastructure::EmailNotification>());
    notificationService.addChannel(
        std::make_shared<infrastructure::SMSNotification>());
    notificationService.addChannel(
        std::make_shared<infrastructure::PushNotification>());

    // Domain services
    auto customerService = std::make_shared<domain::CustomerService>(
        customerRepo, logger
    );

    auto ticketService = std::make_shared<domain::TicketService>(
        ticketRepo, customerRepo, notificationService, logger
    );

    // CLI
    client::CommandLineInterface cli(customerService, ticketService);
    cli.run();

    return 0;
}
