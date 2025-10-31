#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <string>
#include "Enums.hpp"

namespace domain {

class Customer {
private:
    std::string id;
    std::string name;
    std::string email;
    std::string phone;
    CustomerType type;

public:
    Customer(const std::string& id,
             const std::string& name,
             const std::string& email,
             const std::string& phone,
             CustomerType type)
        : id(id), name(name), email(email), phone(phone), type(type) {}

    std::string getId() const { return id; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    CustomerType getType() const { return type; }
};

} // namespace domain

#endif
