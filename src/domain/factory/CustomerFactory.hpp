#ifndef CUSTOMER_FACTORY_HPP
#define CUSTOMER_FACTORY_HPP

#include <memory>
#include <string>

#include "../builder/CustomerBuilder.hpp"
#include "../models/Enums.hpp"

namespace domain {

class CustomerFactory {
public:
    static std::shared_ptr<Customer> createCustomer(
        const std::string& id,
        const std::string& name,
        const std::string& email,
        const std::string& phone,
        CustomerType type
    ) {
        std::string finalName = getTypePrefix(type) + name;

        return CustomerBuilder()
            .withId(id)
            .withName(finalName)
            .withEmail(email)
            .withPhone(phone)
            .withType(type)
            .build();
    }

    static std::string getTypePrefix(CustomerType type) {
        switch(type) {
            case CustomerType::PREMIUM: return "[PREMIUM] ";
            case CustomerType::VIP: return "[VIP] ";
            default: return "";
        }
    }

    static std::string getTypeName(CustomerType type) {
        switch(type) {
            case CustomerType::PREMIUM: return "Premium";
            case CustomerType::VIP: return "VIP";
            default: return "Regular";
        }
    }
};

} // namespace domain

#endif
