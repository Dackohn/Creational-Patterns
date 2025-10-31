#ifndef CLI_HPP
#define CLI_HPP

#include <iostream>
#include <string>
#include <memory>

#include "../domain/services/CustomerService.hpp"
#include "../domain/services/TicketService.hpp"
#include "../domain/models/Enums.hpp"

namespace client {

class CommandLineInterface {
private:
    std::shared_ptr<domain::CustomerService> customerService;
    std::shared_ptr<domain::TicketService> ticketService;

public:
    CommandLineInterface(
        std::shared_ptr<domain::CustomerService> cs,
        std::shared_ptr<domain::TicketService> ts)
        : customerService(cs), ticketService(ts) {}

    void showMenu() {
        std::cout << "\n===== CUSTOMER & TICKET MANAGEMENT =====\n";
        std::cout << "1. Register Customer\n";
        std::cout << "2. List Customers\n";
        std::cout << "3. Create Ticket\n";
        std::cout << "4. List Tickets\n";
        std::cout << "5. Update Ticket Status\n";
        std::cout << "0. Exit\n";
        std::cout << "Choose option: ";
    }

    void run() {
        int option = -1;

        while (option != 0) {
            showMenu();
            std::cin >> option;
            std::cin.ignore();

            switch (option) {
                case 1:
                    registerCustomerUI();
                    break;
                case 2:
                    listCustomersUI();
                    break;
                case 3:
                    createTicketUI();
                    break;
                case 4:
                    listTicketsUI();
                    break;
                case 5:
                    updateTicketStatusUI();
                    break;
                case 0:
                    std::cout << "Exiting...\n";
                    break;
                default:
                    std::cout << "Invalid option!\n";
            }
        }
    }

private:

    void registerCustomerUI() {
        std::string name, email, phone;
        int type;

        std::cout << "Enter customer name: ";
        std::getline(std::cin, name);

        std::cout << "Enter email: ";
        std::getline(std::cin, email);

        std::cout << "Enter phone: ";
        std::getline(std::cin, phone);

        std::cout << "Customer type (0=REGULAR, 1=PREMIUM, 2=VIP): ";
        std::cin >> type;
        std::cin.ignore();

        auto id = customerService->registerCustomer(
            name, email, phone, static_cast<domain::CustomerType>(type)
        );

        std::cout << "Customer registered with ID: " << id << "\n";
    }

    void listCustomersUI() {
        auto list = customerService->getAllCustomers();

        std::cout << "\n--- Customers ---\n";
        for (auto& c : list) {
            std::cout << c->getId() << " | "
                      << c->getName() << " | "
                      << c->getEmail() << " | "
                      << c->getPhone() << "\n";
        }
        if (list.empty()) {
            std::cout << "No customers found.\n";
        }
    }

    void createTicketUI() {
        std::string customerId, description;
        int priority, category;

        std::cout << "Enter customer ID: ";
        std::getline(std::cin, customerId);

        std::cout << "Enter description: ";
        std::getline(std::cin, description);

        std::cout << "Priority (0=LOW, 1=MEDIUM, 2=HIGH, 3=CRITICAL): ";
        std::cin >> priority;

        std::cout << "Category (0=TECH, 1=BILL, 2=GEN, 3=COMP, 4=FEAT): ";
        std::cin >> category;
        std::cin.ignore();

        auto id = ticketService->createTicket(
            customerId,
            description,
            static_cast<domain::Priority>(priority),
            static_cast<domain::TicketCategory>(category)
        );

        if (id.empty()) {
            std::cout << "Failed to create ticket.\n";
        } else {
            std::cout << "Ticket created with ID: " << id << "\n";
        }
    }

    void listTicketsUI() {
        auto list = ticketService->getAllTickets();

        std::cout << "\n--- Tickets ---\n";
        for (auto& t : list) {
            std::cout << t->getId() << " | "
                      << t->getCustomerId() << " | "
                      << t->getDescription() << " | "
                      << domain::TicketFactory::getCategoryName(t->getCategory()) << " | "
                      << domain::TicketFactory::getPriorityName(t->getPriority()) << " | "
                      << domain::TicketFactory::getStatusName(t->getStatus()) << "\n";
        }
        if (list.empty()) {
            std::cout << "No tickets found.\n";
        }
    }

    void updateTicketStatusUI() {
        std::string ticketId;
        int newStatus;

        std::cout << "Enter Ticket ID: ";
        std::getline(std::cin, ticketId);

        std::cout << "New Status (0=OPEN,1=IN_PROGRESS,2=RESOLVED,3=CLOSED): ";
        std::cin >> newStatus;
        std::cin.ignore();

        bool ok = ticketService->updateTicketStatus(
            ticketId,
            static_cast<domain::TicketStatus>(newStatus)
        );

        if (ok) std::cout << "Status updated.\n";
        else std::cout << "Failed to update.\n";
    }
};

} // namespace client

#endif
