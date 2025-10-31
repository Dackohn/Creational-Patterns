#ifndef TICKET_FACTORY_HPP
#define TICKET_FACTORY_HPP

#include <memory>
#include <string>

#include "../builder/TicketBuilder.hpp"
#include "../models/Enums.hpp"

namespace domain {

class TicketFactory {
public:
    static std::shared_ptr<Ticket> createTicket(
        const std::string& id,
        const std::string& customerId,
        const std::string& description,
        Priority priority,
        TicketCategory category
    ) {
        TicketBuilder builder;

        builder
            .withId(id)
            .withCustomerId(customerId)
            .withDescription(description)
            .withPriority(priority)
            .withCategory(category);

        std::string agent = getAutoAssignedAgent(priority);
        if (!agent.empty())
            builder.withAssignedTo(agent);

        for (const auto& tag : getDefaultTags(category))
            builder.addTag(tag);

        return builder.build();
    }

    static std::string getAutoAssignedAgent(Priority p) {
        switch(p) {
            case Priority::CRITICAL: return "Senior-Agent-001";
            case Priority::HIGH: return "Agent-002";
            default: return "";
        }
    }

    static std::vector<std::string> getDefaultTags(TicketCategory c) {
        std::vector<std::string> tags = {"new"};

        switch(c) {
            case TicketCategory::TECHNICAL: tags.push_back("technical-support"); break;
            case TicketCategory::BILLING: tags.push_back("finance"); break;
            case TicketCategory::COMPLAINT: tags.push_back("urgent"); break;
            case TicketCategory::FEATURE_REQUEST: tags.push_back("product"); break;
            default: break;
        }

        return tags;
    }

    static std::string getCategoryName(TicketCategory c) {
        switch(c) {
            case TicketCategory::TECHNICAL: return "Technical";
            case TicketCategory::BILLING: return "Billing";
            case TicketCategory::GENERAL: return "General";
            case TicketCategory::COMPLAINT: return "Complaint";
            case TicketCategory::FEATURE_REQUEST: return "Feature Request";
        }
        return "Unknown";
    }

    static std::string getPriorityName(Priority p) {
        switch(p) {
            case Priority::LOW: return "Low";
            case Priority::MEDIUM: return "Medium";
            case Priority::HIGH: return "High";
            case Priority::CRITICAL: return "Critical";
        }
        return "Unknown";
    }

    static std::string getStatusName(TicketStatus s) {
        switch(s) {
            case TicketStatus::OPEN: return "Open";
            case TicketStatus::IN_PROGRESS: return "In Progress";
            case TicketStatus::RESOLVED: return "Resolved";
            case TicketStatus::CLOSED: return "Closed";
        }
        return "Unknown";
    }
};

} // namespace domain

#endif
