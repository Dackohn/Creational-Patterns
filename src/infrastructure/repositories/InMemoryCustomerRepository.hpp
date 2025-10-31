#ifndef INMEMORY_CUSTOMER_REPOSITORY_HPP
#define INMEMORY_CUSTOMER_REPOSITORY_HPP

#include <map>
#include <memory>
#include <vector>

#include "../../domain/interfaces/ICustomerRepository.hpp"
#include "../../domain/models/Customer.hpp"

namespace infrastructure {

class InMemoryCustomerRepository : public domain::ICustomerRepository {
private:
    std::map<std::string, std::shared_ptr<domain::Customer>> customers;

    InMemoryCustomerRepository() = default;
    InMemoryCustomerRepository(const InMemoryCustomerRepository&) = delete;
    InMemoryCustomerRepository& operator=(const InMemoryCustomerRepository&) = delete;

public:
    static InMemoryCustomerRepository& getInstance() {
        static InMemoryCustomerRepository instance;
        return instance;
    }

    void save(const domain::Customer& customer) override {
        customers[customer.getId()] =
            std::make_shared<domain::Customer>(customer);
    }

    std::shared_ptr<domain::Customer> findById(const std::string& id) override {
        auto it = customers.find(id);
        if (it != customers.end()) {
            return it->second;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<domain::Customer>> findAll() override {
        std::vector<std::shared_ptr<domain::Customer>> list;
        for (auto& pair : customers) {
            list.push_back(pair.second);
        }
        return list;
    }
};

} // namespace infrastructure

#endif
