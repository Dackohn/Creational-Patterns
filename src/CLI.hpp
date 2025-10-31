#ifndef CLI_HPP
#define CLI_HPP

#include <iostream>
#include <string>
#include <memory>
#include <limits>
#include "CustomerManagement.hpp"
#include "TicketManagement.hpp"

namespace CLI {
    
    class CommandLineInterface {
    private:
        std::shared_ptr<CustomerManagement::CustomerService> customerService;
        std::shared_ptr<TicketManagement::TicketService> ticketService;
        bool running;
        
        void clearInputStream() {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        
        void displayMainMenu() {
            std::cout << "\n========================================\n";
            std::cout << "   Customer Service Management System\n";
            std::cout << "========================================\n";
            std::cout << "1. Customer Management\n";
            std::cout << "2. Ticket Management\n";
            std::cout << "3. View All Customers\n";
            std::cout << "4. View All Tickets\n";
            std::cout << "0. Exit\n";
            std::cout << "========================================\n";
            std::cout << "Enter your choice: ";
        }
        
        void displayCustomerMenu() {
            std::cout << "\n--- Customer Management ---\n";
            std::cout << "1. Register New Customer\n";
            std::cout << "2. Find Customer by ID\n";
            std::cout << "0. Back to Main Menu\n";
            std::cout << "Enter your choice: ";
        }
        
        void displayTicketMenu() {
            std::cout << "\n--- Ticket Management ---\n";
            std::cout << "1. Create New Ticket\n";
            std::cout << "2. Update Ticket Status\n";
            std::cout << "3. Find Ticket by ID\n";
            std::cout << "0. Back to Main Menu\n";
            std::cout << "Enter your choice: ";
        }
        
        void registerCustomer() {
            std::string name, email, phone;
            int typeChoice;
            
            std::cout << "\n--- Register New Customer ---\n";
            std::cout << "Enter customer name: ";
            clearInputStream();
            std::getline(std::cin, name);
            
            std::cout << "Enter email: ";
            std::getline(std::cin, email);
            
            std::cout << "Enter phone: ";
            std::getline(std::cin, phone);
            
            std::cout << "\nSelect Customer Type:\n";
            std::cout << "1. Regular\n";
            std::cout << "2. Premium\n";
            std::cout << "3. VIP\n";
            std::cout << "Enter choice (1-3): ";
            std::cin >> typeChoice;
            
            CustomerManagement::CustomerType type;
            switch (typeChoice) {
                case 2: 
                    type = CustomerManagement::CustomerType::PREMIUM; 
                    break;
                case 3: 
                    type = CustomerManagement::CustomerType::VIP; 
                    break;
                default: 
                    type = CustomerManagement::CustomerType::REGULAR;
            }
            
            std::string customerId = customerService->registerCustomer(name, email, phone, type);
            std::cout << "\nCustomer registered successfully!\n";
            std::cout << "Customer ID: " << customerId << "\n";
        }
        
        void findCustomer() {
            std::string customerId;
            
            std::cout << "\n--- Find Customer ---\n";
            std::cout << "Enter customer ID: ";
            clearInputStream();
            std::getline(std::cin, customerId);
            
            auto customer = customerService->getCustomer(customerId);
            if (customer) {
                std::cout << "\n" << CustomerManagement::CustomerDisplayFormatter::format(*customer);
            } else {
                std::cout << "\nCustomer not found!\n";
            }
        }
        
        void createTicket() {
            std::string customerId, description;
            int priorityChoice, categoryChoice;
            
            std::cout << "\n--- Create New Ticket ---\n";
            std::cout << "Enter customer ID: ";
            clearInputStream();
            std::getline(std::cin, customerId);
            
            std::cout << "Enter ticket description: ";
            std::getline(std::cin, description);
            
            std::cout << "\nSelect Category:\n";
            std::cout << "1. Technical\n";
            std::cout << "2. Billing\n";
            std::cout << "3. General\n";
            std::cout << "4. Complaint\n";
            std::cout << "5. Feature Request\n";
            std::cout << "Enter choice (1-5): ";
            std::cin >> categoryChoice;
            
            TicketManagement::TicketCategory category;
            switch (categoryChoice) {
                case 1: category = TicketManagement::TicketCategory::TECHNICAL; break;
                case 2: category = TicketManagement::TicketCategory::BILLING; break;
                case 4: category = TicketManagement::TicketCategory::COMPLAINT; break;
                case 5: category = TicketManagement::TicketCategory::FEATURE_REQUEST; break;
                default: 
                    category = TicketManagement::TicketCategory::GENERAL;
            }
            
            std::cout << "\nSelect Priority:\n";
            std::cout << "1. Low\n";
            std::cout << "2. Medium\n";
            std::cout << "3. High\n";
            std::cout << "4. Critical\n";
            std::cout << "Enter choice (1-4): ";
            std::cin >> priorityChoice;
            
            TicketManagement::Priority priority;
            switch (priorityChoice) {
                case 1: priority = TicketManagement::Priority::LOW; break;
                case 2: priority = TicketManagement::Priority::MEDIUM; break;
                case 3: priority = TicketManagement::Priority::HIGH; break;
                case 4: priority = TicketManagement::Priority::CRITICAL; break;
                default: 
                    priority = TicketManagement::Priority::MEDIUM;
                    std::cout << "Invalid choice. Setting priority to Medium.\n";
            }
            
            std::string ticketId = ticketService->createTicket(customerId, description, priority, category);
            if (!ticketId.empty()) {
                std::cout << "\nTicket created successfully!\n";
                std::cout << "Ticket ID: " << ticketId << "\n";
            } else {
                std::cout << "\nFailed to create ticket. Customer not found.\n";
            }
        }
        
        void updateTicketStatus() {
            std::string ticketId;
            int statusChoice;
            
            std::cout << "\n--- Update Ticket Status ---\n";
            std::cout << "Enter ticket ID: ";
            clearInputStream();
            std::getline(std::cin, ticketId);
            
            std::cout << "\nSelect New Status:\n";
            std::cout << "1. Open\n";
            std::cout << "2. In Progress\n";
            std::cout << "3. Resolved\n";
            std::cout << "4. Closed\n";
            std::cout << "Enter choice (1-4): ";
            std::cin >> statusChoice;
            
            TicketManagement::TicketStatus status;
            switch (statusChoice) {
                case 1: status = TicketManagement::TicketStatus::OPEN; break;
                case 2: status = TicketManagement::TicketStatus::IN_PROGRESS; break;
                case 3: status = TicketManagement::TicketStatus::RESOLVED; break;
                case 4: status = TicketManagement::TicketStatus::CLOSED; break;
                default:
                    std::cout << "Invalid choice.\n";
                    return;
            }
            
            if (ticketService->updateTicketStatus(ticketId, status)) {
                std::cout << "\nTicket status updated successfully!\n";
            } else {
                std::cout << "\nFailed to update ticket. Ticket not found.\n";
            }
        }
        
        void findTicket() {
            std::string ticketId;
            
            std::cout << "\n--- Find Ticket ---\n";
            std::cout << "Enter ticket ID: ";
            clearInputStream();
            std::getline(std::cin, ticketId);
            
            auto tickets = ticketService->getAllTickets();
            bool found = false;
            for (const auto& ticket : tickets) {
                if (ticket->getId() == ticketId) {
                    std::cout << "\n" << TicketManagement::TicketDisplayFormatter::format(*ticket);
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                std::cout << "\nTicket not found!\n";
            }
        }
        
        void viewAllCustomers() {
            std::cout << "\n========== All Customers ==========\n";
            auto customers = customerService->getAllCustomers();
            
            if (customers.empty()) {
                std::cout << "No customers registered yet.\n";
            } else {
                for (const auto& customer : customers) {
                    std::cout << CustomerManagement::CustomerDisplayFormatter::format(*customer);
                }
            }
        }
        
        void viewAllTickets() {
            std::cout << "\n========== All Tickets ==========\n";
            auto tickets = ticketService->getAllTickets();
            
            if (tickets.empty()) {
                std::cout << "No tickets created yet.\n";
            } else {
                for (const auto& ticket : tickets) {
                    std::cout << TicketManagement::TicketDisplayFormatter::format(*ticket);
                }
            }
        }
        
        void handleCustomerManagement() {
            int choice;
            do {
                displayCustomerMenu();
                std::cin >> choice;
                
                switch (choice) {
                    case 1:
                        registerCustomer();
                        break;
                    case 2:
                        findCustomer();
                        break;
                    case 0:
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again.\n";
                }
            } while (choice != 0);
        }
        
        void handleTicketManagement() {
            int choice;
            do {
                displayTicketMenu();
                std::cin >> choice;
                
                switch (choice) {
                    case 1:
                        createTicket();
                        break;
                    case 2:
                        updateTicketStatus();
                        break;
                    case 3:
                        findTicket();
                        break;
                    case 0:
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again.\n";
                }
            } while (choice != 0);
        }
        
    public:
        CommandLineInterface(
            std::shared_ptr<CustomerManagement::CustomerService> custService,
            std::shared_ptr<TicketManagement::TicketService> tktService)
            : customerService(custService), ticketService(tktService), running(true) {}
        
        void run() {
            int choice;

            std::cout << " Welcome to Customer Service System \n";
            
            while (running) {
                displayMainMenu();
                std::cin >> choice;
                
                switch (choice) {
                    case 1:
                        handleCustomerManagement();
                        break;
                    case 2:
                        handleTicketManagement();
                        break;
                    case 3:
                        viewAllCustomers();
                        break;
                    case 4:
                        viewAllTickets();
                        break;
                    case 0:
                        std::cout << "\nThank you for using Customer Service System!\n";
                        std::cout << "Goodbye!\n\n";
                        running = false;
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again.\n";
                }
            }
        }
    };
}

#endif