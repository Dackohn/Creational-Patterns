#include <iostream>
#include "CustomerManagement.hpp"
#include "TicketManagement.hpp"
#include "NotificationChannels.hpp"
#include "Logging.hpp"
#include "CLI.hpp"

int main() {
    using namespace CustomerManagement;
    using namespace TicketManagement;
    using namespace NotificationChannels;
    using namespace Logging;
    using namespace CLI;
    
    auto logger = std::make_shared<ConsoleLogger>();
    
    auto& ticketRepo = InMemoryTicketRepository::getInstance();
    auto& customerRepo = InMemoryCustomerRepository::getInstance();
    
    auto& notificationService = NotificationService::getInstance(logger);
    notificationService.addChannel(std::make_shared<EmailNotification>());
    notificationService.addChannel(std::make_shared<SMSNotification>());
    notificationService.addChannel(std::make_shared<PushNotification>());
    
    auto customerService = std::make_shared<CustomerService>(customerRepo, logger);
    auto ticketService = std::make_shared<TicketService>(ticketRepo, customerRepo, notificationService, logger);
    
    CommandLineInterface cli(customerService, ticketService);
    cli.run();
    
    return 0;
}