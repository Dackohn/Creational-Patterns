#ifndef INMEMORY_TICKET_REPOSITORY_HPP
#define INMEMORY_TICKET_REPOSITORY_HPP

#include <map>
#include <memory>
#include <vector>

#include "../../domain/interfaces/ITicketRepository.hpp"
#include "../../domain/models/Ticket.hpp"

namespace infrastructure {

class InMemoryTicketRepository : public domain::ITicketRepository {
private:
    std::map<std::string, std::shared_ptr<domain::Ticket>> tickets;

    InMemoryTicketRepository() = default;
    InMemoryTicketRepository(const InMemoryTicketRepository&) = delete;
    InMemoryTicketRepository& operator=(const InMemoryTicketRepository&) = delete;

public:
    static InMemoryTicketRepository& getInstance() {
        static InMemoryTicketRepository instance;
        return instance;
    }

    void save(const domain::Ticket& ticket) override {
        tickets[ticket.getId()] =
            std::make_shared<domain::Ticket>(ticket);
    }

    std::shared_ptr<domain::Ticket> findById(const std::string& id) override {
        auto it = tickets.find(id);
        if (it != tickets.end()) {
            return it->second;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<domain::Ticket>> findAll() override {
        std::vector<std::shared_ptr<domain::Ticket>> list;
        for (auto& pair : tickets) {
            list.push_back(pair.second);
        }
        return list;
    }
};

} // namespace infrastructure

#endif
