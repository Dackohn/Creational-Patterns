#ifndef TICKET_HPP
#define TICKET_HPP

#include <string>
#include <vector>
#include <ctime>
#include "Enums.hpp"

namespace domain {

class Ticket {
private:
    std::string id;
    std::string customerId;
    std::string description;
    TicketStatus status;
    Priority priority;
    TicketCategory category;
    std::time_t createdAt;
    std::string assignedTo;
    std::vector<std::string> tags;

public:
    Ticket(const std::string& id,
           const std::string& customerId,
           const std::string& description,
           Priority priority,
           TicketCategory category)
        : id(id), customerId(customerId), description(description),
          status(TicketStatus::OPEN), priority(priority), category(category),
          assignedTo("") 
    {
        createdAt = std::time(nullptr);
    }

    std::string getId() const { return id; }
    std::string getCustomerId() const { return customerId; }
    std::string getDescription() const { return description; }
    TicketStatus getStatus() const { return status; }
    Priority getPriority() const { return priority; }
    TicketCategory getCategory() const { return category; }
    std::string getAssignedTo() const { return assignedTo; }
    std::time_t getCreatedAt() const { return createdAt; }
    std::vector<std::string> getTags() const { return tags; }

    void setStatus(TicketStatus s) { status = s; }
    void setPriority(Priority p) { priority = p; }
    void setAssignedTo(const std::string& a) { assignedTo = a; }
    void addTag(const std::string& tag) { tags.push_back(tag); }
};

} // namespace domain

#endif
