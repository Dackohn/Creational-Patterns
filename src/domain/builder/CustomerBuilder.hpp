#ifndef CUSTOMER_BUILDER_HPP
#define CUSTOMER_BUILDER_HPP

#include <string>
#include <memory>
#include "../models/Customer.hpp"
#include "../models/Enums.hpp"

namespace domain {

class CustomerBuilder {
private:
    std::string id;
    std::string name;
    std::string email;
    std::string phone;
    CustomerType type = CustomerType::REGULAR;

public:
    CustomerBuilder& withId(const std::string& value) {
        id = value;
        return *this;
    }

    CustomerBuilder& withName(const std::string& value) {
        name = value;
        return *this;
    }

    CustomerBuilder& withEmail(const std::string& value) {
        email = value;
        return *this;
    }

    CustomerBuilder& withPhone(const std::string& value) {
        phone = value;
        return *this;
    }

    CustomerBuilder& withType(CustomerType value) {
        type = value;
        return *this;
    }

    std::shared_ptr<Customer> build() {
        return std::make_shared<Customer>(id, name, email, phone, type);
    }
};

} // namespace domain

#endif
