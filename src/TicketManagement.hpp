#ifndef TICKET_MANAGEMENT_HPP
#define TICKET_MANAGEMENT_HPP

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <ctime>
#include <sstream>
#include "CustomerManagement.hpp"

namespace TicketManagement {
    
    enum class TicketStatus {
        OPEN,
        IN_PROGRESS,
        RESOLVED,
        CLOSED
    };
    
    enum class Priority {
        LOW,
        MEDIUM,
        HIGH,
        CRITICAL
    };
    
    enum class TicketCategory {
        TECHNICAL,
        BILLING,
        GENERAL,
        COMPLAINT,
        FEATURE_REQUEST
    };
    
    class Ticket {
    private:
        std::string id;
        std::string customerId;
        std::string description;
        TicketStatus status;
        Priority priority;
        std::time_t createdAt;
        std::string assignedTo;
        TicketCategory category;
        std::vector<std::string> tags;
        
    public:
        Ticket(const std::string& ticketId, const std::string& custId, 
               const std::string& desc, Priority prio, TicketCategory cat)
            : id(ticketId), customerId(custId), description(desc), 
              priority(prio), status(TicketStatus::OPEN), assignedTo(""), category(cat) {
            createdAt = std::time(nullptr);
        }
        
        std::string getId() const { return id; }
        std::string getCustomerId() const { return customerId; }
        std::string getDescription() const { return description; }
        TicketStatus getStatus() const { return status; }
        Priority getPriority() const { return priority; }
        std::time_t getCreatedAt() const { return createdAt; }
        std::string getAssignedTo() const { return assignedTo; }
        TicketCategory getCategory() const { return category; }
        std::vector<std::string> getTags() const { return tags; }
        
        void setStatus(TicketStatus newStatus) { status = newStatus; }
        void setPriority(Priority newPriority) { priority = newPriority; }
        void setAssignedTo(const std::string& agent) { assignedTo = agent; }
        void setCategory(TicketCategory cat) { category = cat; }
        void addTag(const std::string& tag) { tags.push_back(tag); }
    };
    
    class TicketBuilder {
    private:
        std::string id;
        std::string customerId;
        std::string description;
        Priority priority;
        std::string assignedTo;
        TicketCategory category;
        std::vector<std::string> tags;
        
    public:
        TicketBuilder() : priority(Priority::MEDIUM), category(TicketCategory::GENERAL) {}
        
        TicketBuilder& withId(const std::string& ticketId) {
            id = ticketId;
            return *this;
        }
        
        TicketBuilder& withCustomerId(const std::string& custId) {
            customerId = custId;
            return *this;
        }
        
        TicketBuilder& withDescription(const std::string& desc) {
            description = desc;
            return *this;
        }
        
        TicketBuilder& withPriority(Priority prio) {
            priority = prio;
            return *this;
        }
        
        TicketBuilder& withAssignedTo(const std::string& agent) {
            assignedTo = agent;
            return *this;
        }
        
        TicketBuilder& withCategory(TicketCategory cat) {
            category = cat;
            return *this;
        }
        
        TicketBuilder& addTag(const std::string& tag) {
            tags.push_back(tag);
            return *this;
        }
        
        std::shared_ptr<Ticket> build() {
            auto ticket = std::make_shared<Ticket>(id, customerId, description, priority, category);
            if (!assignedTo.empty()) {
                ticket->setAssignedTo(assignedTo);
            }
            for (const auto& tag : tags) {
                ticket->addTag(tag);
            }
            return ticket;
        }
    };
    
    class TicketFactory {
    public:
        static std::string getCategoryName(TicketCategory category) {
            switch (category) {
                case TicketCategory::TECHNICAL: return "Technical";
                case TicketCategory::BILLING: return "Billing";
                case TicketCategory::COMPLAINT: return "Complaint";
                case TicketCategory::FEATURE_REQUEST: return "Feature Request";
                default: return "General";
            }
        }
        
        static std::string getPriorityName(Priority priority) {
            switch (priority) {
                case Priority::LOW: return "Low";
                case Priority::MEDIUM: return "Medium";
                case Priority::HIGH: return "High";
                case Priority::CRITICAL: return "Critical";
                default: return "Unknown";
            }
        }
        
        static std::string getStatusName(TicketStatus status) {
            switch (status) {
                case TicketStatus::OPEN: return "Open";
                case TicketStatus::IN_PROGRESS: return "In Progress";
                case TicketStatus::RESOLVED: return "Resolved";
                case TicketStatus::CLOSED: return "Closed";
                default: return "Unknown";
            }
        }
        
        static std::string getAutoAssignedAgent(Priority priority) {
            switch (priority) {
                case Priority::CRITICAL: return "Senior-Agent-001";
                case Priority::HIGH: return "Agent-002";
                default: return "";
            }
        }
        
        static std::vector<std::string> getDefaultTags(TicketCategory category) {
            std::vector<std::string> tags;
            tags.push_back("new");
            
            switch (category) {
                case TicketCategory::TECHNICAL:
                    tags.push_back("technical-support");
                    break;
                case TicketCategory::BILLING:
                    tags.push_back("finance");
                    break;
                case TicketCategory::COMPLAINT:
                    tags.push_back("urgent");
                    break;
                case TicketCategory::FEATURE_REQUEST:
                    tags.push_back("product");
                    break;
                default:
                    break;
            }
            
            return tags;
        }
    };
    
    class ITicketRepository {
    public:
        virtual ~ITicketRepository() = default;
        virtual void save(const Ticket& ticket) = 0;
        virtual std::shared_ptr<Ticket> findById(const std::string& id) = 0;
        virtual std::vector<std::shared_ptr<Ticket>> findAll() = 0;
    };
    
    class InMemoryTicketRepository : public ITicketRepository {
    private:
        std::map<std::string, std::shared_ptr<Ticket>> tickets;
        
        InMemoryTicketRepository() = default;
        InMemoryTicketRepository(const InMemoryTicketRepository&) = delete;
        InMemoryTicketRepository& operator=(const InMemoryTicketRepository&) = delete;
        
    public:
        static InMemoryTicketRepository& getInstance() {
            static InMemoryTicketRepository instance;
            return instance;
        }
        
        void save(const Ticket& ticket) override {
            tickets[ticket.getId()] = std::make_shared<Ticket>(ticket);
        }
        
        std::shared_ptr<Ticket> findById(const std::string& id) override {
            auto it = tickets.find(id);
            if (it != tickets.end()) {
                return it->second;
            }
            return nullptr;
        }
        
        std::vector<std::shared_ptr<Ticket>> findAll() override {
            std::vector<std::shared_ptr<Ticket>> result;
            for (const auto& pair : tickets) {
                result.push_back(pair.second);
            }
            return result;
        }
    };
    
    class INotificationChannel {
    public:
        virtual ~INotificationChannel() = default;
        virtual bool send(const std::string& recipient, const std::string& message) = 0;
        virtual std::string getChannelName() const = 0;
    };
    
    class NotificationService {
    private:
        std::vector<std::shared_ptr<INotificationChannel>> channels;
        std::shared_ptr<CustomerManagement::ILogger> logger;
        
        NotificationService(std::shared_ptr<CustomerManagement::ILogger> log)
            : logger(log) {}
        
        NotificationService(const NotificationService&) = delete;
        NotificationService& operator=(const NotificationService&) = delete;
        
    public:
        static NotificationService& getInstance(std::shared_ptr<CustomerManagement::ILogger> log = nullptr) {
            static NotificationService instance(log);
            return instance;
        }
        
        void addChannel(std::shared_ptr<INotificationChannel> channel) {
            channels.push_back(channel);
            if (logger) {
                logger->log("Added notification channel: " + channel->getChannelName());
            }
        }
        
        void notify(const std::string& recipient, const std::string& message) {
            for (const auto& channel : channels) {
                if (channel->send(recipient, message)) {
                    if (logger) {
                        logger->log("Notification sent via " + channel->getChannelName() + 
                                  " to " + recipient);
                    }
                }
            }
        }
    };
    
    class TicketService {
    private:
        ITicketRepository& ticketRepo;
        CustomerManagement::ICustomerRepository& customerRepo;
        NotificationService& notificationService;
        std::shared_ptr<CustomerManagement::ILogger> logger;
        int ticketCounter;
        
    public:
        TicketService(
            ITicketRepository& tRepo,
            CustomerManagement::ICustomerRepository& cRepo,
            NotificationService& nService,
            std::shared_ptr<CustomerManagement::ILogger> log)
            : ticketRepo(tRepo), customerRepo(cRepo), 
              notificationService(nService), logger(log), ticketCounter(1000) {}
        
        std::string createTicket(const std::string& customerId, 
                                const std::string& description,
                                Priority priority,
                                TicketCategory category = TicketCategory::GENERAL) {
            auto customer = customerRepo.findById(customerId);
            if (!customer) {
                logger->log("Failed to create ticket: Customer not found - " + customerId);
                return "";
            }
            
            std::string ticketId = "TKT-" + std::to_string(++ticketCounter);
            std::string assignedAgent = TicketFactory::getAutoAssignedAgent(priority);
            
            TicketBuilder builder;
            builder.withId(ticketId).withCustomerId(customerId).withDescription(description).withPriority(priority).withCategory(category);
            
            if (!assignedAgent.empty()) {
                builder.withAssignedTo(assignedAgent);
            }
            
            auto defaultTags = TicketFactory::getDefaultTags(category);
            for (const auto& tag : defaultTags) {
                builder.addTag(tag);
            }
            
            auto ticket = builder.build();
            
            ticketRepo.save(*ticket);
            logger->log("Ticket created: " + ticketId + " for customer " + customer->getName() +
                       " (Category: " + TicketFactory::getCategoryName(category) + 
                       ", Priority: " + TicketFactory::getPriorityName(priority) + ")");
            
            std::string message = "Your ticket " + ticketId + " has been created. " +
                                "Category: " + TicketFactory::getCategoryName(category) + ". " +
                                "Description: " + description;
            notificationService.notify(customer->getEmail(), message);
            
            return ticketId;
        }
        
        bool updateTicketStatus(const std::string& ticketId, TicketStatus newStatus) {
            auto ticket = ticketRepo.findById(ticketId);
            if (!ticket) {
                logger->log("Failed to update ticket: Ticket not found - " + ticketId);
                return false;
            }
            
            ticket->setStatus(newStatus);
            ticketRepo.save(*ticket);
            
            auto customer = customerRepo.findById(ticket->getCustomerId());
            if (customer) {
                std::string statusStr = TicketFactory::getStatusName(newStatus);
                std::string message = "Your ticket " + ticketId + 
                                    " status has been updated to: " + statusStr;
                notificationService.notify(customer->getEmail(), message);
            }
            
            logger->log("Ticket status updated: " + ticketId + " to " + 
                       TicketFactory::getStatusName(newStatus));
            return true;
        }
        
        std::vector<std::shared_ptr<Ticket>> getAllTickets() {
            return ticketRepo.findAll();
        }
    };
    
    class TicketDisplayFormatter {
    public:
        static std::string format(const Ticket& ticket) {
            std::stringstream ss;
            ss << "Ticket ID: " << ticket.getId() << "\n";
            ss << "Customer ID: " << ticket.getCustomerId() << "\n";
            ss << "Description: " << ticket.getDescription() << "\n";
            ss << "Category: " << TicketFactory::getCategoryName(ticket.getCategory()) << "\n";
            ss << "Priority: " << TicketFactory::getPriorityName(ticket.getPriority()) << "\n";
            ss << "Status: " << TicketFactory::getStatusName(ticket.getStatus()) << "\n";
            if (!ticket.getAssignedTo().empty()) {
                ss << "Assigned To: " << ticket.getAssignedTo() << "\n";
            }
            if (!ticket.getTags().empty()) {
                ss << "Tags: ";
                for (size_t i = 0; i < ticket.getTags().size(); ++i) {
                    ss << ticket.getTags()[i];
                    if (i < ticket.getTags().size() - 1) ss << ", ";
                }
                ss << "\n";
            }
            ss << "-----------------------------------\n";
            return ss.str();
        }
    };
}

#endif