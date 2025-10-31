#ifndef CUSTOMER_SERVICE_HPP
#define CUSTOMER_SERVICE_HPP

#include <memory>
#include <string>

#include "../interfaces/ICustomerRepository.hpp"
#include "../interfaces/ILogger.hpp"
#include "../factory/CustomerFactory.hpp"
#include "../models/Customer.hpp"
#include "../models/Enums.hpp"

namespace domain {

class CustomerService {
private:
    ICustomerRepository& repository;
    std::shared_ptr<ILogger> logger;
    int counter = 1000;

public:
    CustomerService(ICustomerRepository& repo,
                    std::shared_ptr<ILogger> logger)
        : repository(repo), logger(logger) {}

    std::string registerCustomer(const std::string& name,
                                 const std::string& email,
                                 const std::string& phone,
                                 CustomerType type = CustomerType::REGULAR)
    {
        std::string id = "CUST-" + std::to_string(++counter);

        auto customer = CustomerFactory::createCustomer(
            id, name, email, phone, type
        );

        repository.save(*customer);

        if (logger) {
            logger->log("Registered customer " + id + " (" +
                        CustomerFactory::getTypeName(type) + ")");
        }

        return id;
    }

    std::shared_ptr<Customer> getCustomer(const std::string& id) {
        return repository.findById(id);
    }

    std::vector<std::shared_ptr<Customer>> getAllCustomers() {
        return repository.findAll();
    }
};

} // namespace domain

#endif
