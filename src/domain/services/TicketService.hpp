#ifndef TICKET_SERVICE_HPP
#define TICKET_SERVICE_HPP

#include <memory>
#include <string>

#include "../interfaces/ITicketRepository.hpp"
#include "../interfaces/ICustomerRepository.hpp"
#include "../interfaces/ILogger.hpp"
#include "../services/NotificationService.hpp"
#include "../factory/TicketFactory.hpp"
#include "../models/Ticket.hpp"
#include "../models/Enums.hpp"

namespace domain {

class TicketService {
private:
    ITicketRepository& ticketRepo;
    ICustomerRepository& customerRepo;
    NotificationService& notificationService;
    std::shared_ptr<ILogger> logger;
    int counter = 1000;

public:
    TicketService(ITicketRepository& tr,
                  ICustomerRepository& cr,
                  NotificationService& ns,
                  std::shared_ptr<ILogger> logger)
        : ticketRepo(tr), customerRepo(cr), 
          notificationService(ns), logger(logger) {}

    std::string createTicket(const std::string& customerId,
                             const std::string& description,
                             Priority priority,
                             TicketCategory category = TicketCategory::GENERAL)
    {
        auto customer = customerRepo.findById(customerId);

        if (!customer) {
            if (logger) logger->log("Cannot create ticket: customer not found");
            return "";
        }

        std::string id = "TKT-" + std::to_string(++counter);

        auto ticket = TicketFactory::createTicket(
            id, customerId, description, priority, category
        );

        ticketRepo.save(*ticket);

        if (logger) {
            logger->log("Created ticket " + id +
                        " (Category=" + TicketFactory::getCategoryName(category) +
                        ", Priority=" + TicketFactory::getPriorityName(priority) +
                        ")");
        }

        std::string msg =
            "Your ticket " + id + " has been created.\n"
            "Category: " + TicketFactory::getCategoryName(category) + "\n"
            "Description: " + description;

        notificationService.notify(customer->getEmail(), msg);

        return id;
    }

    bool updateTicketStatus(const std::string& ticketId, TicketStatus status) {
        auto ticket = ticketRepo.findById(ticketId);
        if (!ticket) {
            if (logger) logger->log("Ticket not found: " + ticketId);
            return false;
        }

        ticket->setStatus(status);
        ticketRepo.save(*ticket);

        auto customer = customerRepo.findById(ticket->getCustomerId());
        if (customer) {
            std::string msg = "Your ticket " + ticketId +
                              " status changed to " +
                              TicketFactory::getStatusName(status);
            notificationService.notify(customer->getEmail(), msg);
        }

        if (logger) {
            logger->log("Ticket " + ticketId + " updated to " +
                        TicketFactory::getStatusName(status));
        }

        return true;
    }

    std::vector<std::shared_ptr<Ticket>> getAllTickets() {
        return ticketRepo.findAll();
    }
};

} // namespace domain

#endif
