#ifndef TICKET_BUILDER_HPP
#define TICKET_BUILDER_HPP

#include <string>
#include <memory>
#include <vector>

#include "../models/Ticket.hpp"
#include "../models/Enums.hpp"

namespace domain {

class TicketBuilder {
private:
    std::string id;
    std::string customerId;
    std::string description;
    Priority priority = Priority::MEDIUM;
    TicketCategory category = TicketCategory::GENERAL;
    std::string assignedTo;
    std::vector<std::string> tags;

public:
    TicketBuilder& withId(const std::string& v) { id = v; return *this; }
    TicketBuilder& withCustomerId(const std::string& v) { customerId = v; return *this; }
    TicketBuilder& withDescription(const std::string& v) { description = v; return *this; }
    TicketBuilder& withPriority(Priority v) { priority = v; return *this; }
    TicketBuilder& withCategory(TicketCategory v) { category = v; return *this; }
    TicketBuilder& withAssignedTo(const std::string& v) { assignedTo = v; return *this; }
    TicketBuilder& addTag(const std::string& tag) { tags.push_back(tag); return *this; }

    std::shared_ptr<Ticket> build() {
        auto ticket = std::make_shared<Ticket>(id, customerId, description, priority, category);

        if (!assignedTo.empty())
            ticket->setAssignedTo(assignedTo);

        for (const auto& t : tags)
            ticket->addTag(t);

        return ticket;
    }
};

} // namespace domain

#endif
