#ifndef I_CUSTOMER_REPOSITORY_HPP
#define I_CUSTOMER_REPOSITORY_HPP

#include <memory>
#include <vector>
#include <string>
#include "../models/Customer.hpp"

namespace domain {

class ICustomerRepository {
public:
    virtual ~ICustomerRepository() = default;

    virtual void save(const Customer& customer) = 0;
    virtual std::shared_ptr<Customer> findById(const std::string& id) = 0;
    virtual std::vector<std::shared_ptr<Customer>> findAll() = 0;
};

} // namespace domain

#endif
