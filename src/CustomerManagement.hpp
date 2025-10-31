#ifndef CUSTOMER_MANAGEMENT_HPP
#define CUSTOMER_MANAGEMENT_HPP

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <sstream>

namespace CustomerManagement {
    
    enum class CustomerType {
        REGULAR,
        PREMIUM,
        VIP
    };
    
    class Customer {
    private:
        std::string id;
        std::string name;
        std::string email;
        std::string phone;
        CustomerType type;
        
    public:
        Customer(const std::string& customerId, const std::string& customerName,
                const std::string& customerEmail, const std::string& customerPhone,
                CustomerType customerType)
            : id(customerId), name(customerName), email(customerEmail), 
              phone(customerPhone), type(customerType) {}
        
        std::string getId() const { return id; }
        std::string getName() const { return name; }
        std::string getEmail() const { return email; }
        std::string getPhone() const { return phone; }
        CustomerType getType() const { return type; }
    };
    
    class CustomerBuilder {
    private:
        std::string id;
        std::string name;
        std::string email;
        std::string phone;
        CustomerType type;
        
    public:
        CustomerBuilder() : type(CustomerType::REGULAR) {}
        
        CustomerBuilder& withId(const std::string& customerId) {
            id = customerId;
            return *this;
        }
        
        CustomerBuilder& withName(const std::string& customerName) {
            name = customerName;
            return *this;
        }
        
        CustomerBuilder& withEmail(const std::string& customerEmail) {
            email = customerEmail;
            return *this;
        }
        
        CustomerBuilder& withPhone(const std::string& customerPhone) {
            phone = customerPhone;
            return *this;
        }
        
        CustomerBuilder& withType(CustomerType customerType) {
            type = customerType;
            return *this;
        }
        
        std::shared_ptr<Customer> build() {
            return std::make_shared<Customer>(id, name, email, phone, type);
        }
    };
    
    class ICustomerRepository {
    public:
        virtual ~ICustomerRepository() = default;
        virtual void save(const Customer& customer) = 0;
        virtual std::shared_ptr<Customer> findById(const std::string& id) = 0;
        virtual std::vector<std::shared_ptr<Customer>> findAll() = 0;
    };
    
    class InMemoryCustomerRepository : public ICustomerRepository {
    private:
        std::map<std::string, std::shared_ptr<Customer>> customers;
        
        InMemoryCustomerRepository() = default;
        InMemoryCustomerRepository(const InMemoryCustomerRepository&) = delete;
        InMemoryCustomerRepository& operator=(const InMemoryCustomerRepository&) = delete;
        
    public:
        static InMemoryCustomerRepository& getInstance() {
            static InMemoryCustomerRepository instance;
            return instance;
        }
        
        void save(const Customer& customer) override {
            customers[customer.getId()] = std::make_shared<Customer>(customer);
        }
        
        std::shared_ptr<Customer> findById(const std::string& id) override {
            auto it = customers.find(id);
            if (it != customers.end()) {
                return it->second;
            }
            return nullptr;
        }
        
        std::vector<std::shared_ptr<Customer>> findAll() override {
            std::vector<std::shared_ptr<Customer>> result;
            for (const auto& pair : customers) {
                result.push_back(pair.second);
            }
            return result;
        }
    };
    
    class ILogger {
    public:
        virtual ~ILogger() = default;
        virtual void log(const std::string& message) = 0;
    };
    
    class CustomerFactory {
    public:
        static std::string getTypePrefix(CustomerType type) {
            switch (type) {
                case CustomerType::PREMIUM:
                    return "[PREMIUM] ";
                case CustomerType::VIP:
                    return "[VIP] ";
                default:
                    return "";
            }
        }
        
        static std::string getTypeName(CustomerType type) {
            switch (type) {
                case CustomerType::PREMIUM:
                    return "Premium";
                case CustomerType::VIP:
                    return "VIP";
                default:
                    return "Regular";
            }
        }
    };
    
    class CustomerService {
    private:
        ICustomerRepository& customerRepo;
        std::shared_ptr<ILogger> logger;
        int customerCounter;
        
    public:
        CustomerService(ICustomerRepository& repo, std::shared_ptr<ILogger> log)
            : customerRepo(repo), logger(log), customerCounter(1000) {}
        
        std::string registerCustomer(const std::string& name, 
                                     const std::string& email,
                                     const std::string& phone,
                                     CustomerType type = CustomerType::REGULAR) {
            std::string customerId = "CUST-" + std::to_string(++customerCounter);
            std::string prefix = CustomerFactory::getTypePrefix(type);
            
            auto customer = CustomerBuilder().withId(customerId).withName(prefix + name).withEmail(email).withPhone(phone).withType(type).build();
            
            customerRepo.save(*customer);
            logger->log("Customer registered: " + customerId + " - " + name + 
                       " (Type: " + CustomerFactory::getTypeName(type) + ")");
            
            return customerId;
        }
        
        std::shared_ptr<Customer> getCustomer(const std::string& customerId) {
            return customerRepo.findById(customerId);
        }
        
        std::vector<std::shared_ptr<Customer>> getAllCustomers() {
            return customerRepo.findAll();
        }
    };
    
    class CustomerDisplayFormatter {
    public:
        static std::string format(const Customer& customer) {
            std::stringstream ss;
            ss << "Customer ID: " << customer.getId() << "\n";
            ss << "Name: " << customer.getName() << "\n";
            ss << "Email: " << customer.getEmail() << "\n";
            ss << "Phone: " << customer.getPhone() << "\n";
            ss << "Type: " << CustomerFactory::getTypeName(customer.getType()) << "\n";
            ss << "-----------------------------------\n";
            return ss.str();
        }
    };
}

#endif