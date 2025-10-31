#ifndef I_TICKET_REPOSITORY_HPP
#define I_TICKET_REPOSITORY_HPP

#include <memory>
#include <vector>
#include <string>
#include "../models/Ticket.hpp"

namespace domain {

class ITicketRepository {
public:
    virtual ~ITicketRepository() = default;

    virtual void save(const Ticket& ticket) = 0;
    virtual std::shared_ptr<Ticket> findById(const std::string& id) = 0;
    virtual std::vector<std::shared_ptr<Ticket>> findAll() = 0;
};

} // namespace domain

#endif
