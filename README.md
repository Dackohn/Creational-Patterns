# Creational Design Patterns

## Project Overview

A customer service management system built in C++ that demonstrates three fundamental creational design patterns through practical implementation.

**Key Features:**
- Customer registration and management with multiple customer types
- Support ticket creation with complex configurations
- Multi-channel notifications (Email, SMS, Push)
- Interactive command-line interface
- Centralized logging and repository systems

---

## 1. Singleton Pattern

**Definition:** Ensures a class has only one instance and provides a global point of access to it.

### Implementation

Three critical components use the Singleton pattern to maintain single instances throughout the application lifecycle.

#### Example 1: InMemoryCustomerRepository
```cpp
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
        return (it != customers.end()) ? it->second : nullptr;
    }
};
```
**Purpose:** Ensures all customer data is stored in a single, centralized location throughout the application.

**Key Characteristics:**
- **Private Constructor:** Prevents external instantiation
- **Deleted Copy Operations:** Prevents copying the singleton
- **Static getInstance():** Returns reference to the single instance
- **Thread-Safe:** C++11 guarantees thread-safe static initialization

#### Example 2: InMemoryTicketRepository
```cpp
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
        return (it != tickets.end()) ? it->second : nullptr;
    }
};
```
**Purpose:** Maintains a single source of truth for all ticket data.

#### Example 3: NotificationService
```cpp
class NotificationService {
private:
    std::vector<std::shared_ptr<INotificationChannel>> channels;
    std::shared_ptr<CustomerManagement::ILogger> logger;
    
    NotificationService(std::shared_ptr<CustomerManagement::ILogger> log)
        : logger(log) {}
    
    NotificationService(const NotificationService&) = delete;
    NotificationService& operator=(const NotificationService&) = delete;
    
public:
    static NotificationService& getInstance(
        std::shared_ptr<CustomerManagement::ILogger> log = nullptr) {
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
```
**Purpose:** Centralizes all notification channel management and ensures consistent notification behavior.

### Usage in main.cpp
```cpp
int main() {
    auto logger = std::make_shared<ConsoleLogger>();
    
    auto& ticketRepo = InMemoryTicketRepository::getInstance();
    auto& customerRepo = InMemoryCustomerRepository::getInstance();
    auto& notificationService = NotificationService::getInstance(logger);
    
    notificationService.addChannel(std::make_shared<EmailNotification>());
    notificationService.addChannel(std::make_shared<SMSNotification>());
    notificationService.addChannel(std::make_shared<PushNotification>());
    
    auto customerService = std::make_shared<CustomerService>(customerRepo, logger);
    auto ticketService = std::make_shared<TicketService>(
        ticketRepo, customerRepo, notificationService, logger);
    
    CommandLineInterface cli(customerService, ticketService);
    cli.run();
    
    return 0;
}
```

### Why This Matters

- **Controlled Access:** Only one repository instance prevents data inconsistency
- **Global State Management:** All parts of the application access the same data
- **Resource Efficiency:** No duplicate repositories consuming memory
- **Consistency:** All operations work on the same notification channels
- **Simplified Testing:** Easy to reset state by accessing the single instance

### Benefits

- **Memory Efficiency:** Only one instance exists throughout the application  
- **Data Consistency:** All operations work on the same data store  
- **Global Access:** Any part of the application can access repositories  
- **Lazy Initialization:** Instance created only when first needed  
- **Thread-Safe:** Modern C++ guarantees safe static initialization  

---

## 2. Builder Pattern

**Definition:** Separates the construction of a complex object from its representation, allowing the same construction process to create different representations.

### Implementation

Two builder classes construct complex objects with multiple optional parameters using a fluent interface.

#### Example 1: CustomerBuilder
```cpp
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
```

**Usage in CustomerService:**
```cpp
std::string registerCustomer(const std::string& name, 
                             const std::string& email,
                             const std::string& phone,
                             CustomerType type = CustomerType::REGULAR) {
    std::string customerId = "CUST-" + std::to_string(++customerCounter);
    
    auto customer = CustomerFactory::createCustomer(
        customerId,
        name,
        email,
        phone,
        type
    );
    
    customerRepo.save(*customer);
    logger->log("Customer registered: " + customerId + " - " + name + 
               " (Type: " + CustomerFactory::getTypeName(type) + ")");
    
    return customerId;
}
```

**Purpose:** Constructs Customer objects with clear, readable code while handling type-specific prefixes.

#### Example 2: TicketBuilder
```cpp
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
```

**Usage in TicketService:**
```cpp
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
    builder.withId(ticketId)
           .withCustomerId(customerId)
           .withDescription(description)
           .withPriority(priority)
           .withCategory(category);
    
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
```

**Purpose:** Constructs complex Ticket objects with many optional parameters (assigned agent, category, multiple tags) in a readable, maintainable way.

### Why This Matters

- **Readability:** Clear, self-documenting code showing what each parameter means
- **Flexibility:** Optional parameters can be omitted or added in any order
- **Immutability:** Final object is constructed in one step after configuration
- **Validation:** Can add validation logic in the build() method
- **No Telescoping Constructors:** Avoids multiple constructor overloads

### Builder Pattern Advantages

- **Fluent Interface:** Method chaining creates readable code  
- **Optional Parameters:** Not all fields need to be specified  
- **Default Values:** Constructor provides sensible defaults  
- **Complex Construction:** Handles multi-step object creation  
- **Separation of Concerns:** Building logic separate from object logic  

### Comparison: With vs Without Builder

**Without Builder (Telescoping Constructor Problem):**
```cpp
Ticket(id, customerId, description, priority, category);
Ticket(id, customerId, description, priority, category, assignedTo);
Ticket(id, customerId, description, priority, category, assignedTo, tag1);
Ticket(id, customerId, description, priority, category, assignedTo, tag1, tag2);
```

**With Builder:**
```cpp
TicketBuilder()
    .withId(ticketId)
    .withCustomerId(customerId)
    .withDescription(description)
    .withPriority(priority)
    .withCategory(category)
    .withAssignedTo(agent)
    .addTag("urgent")
    .addTag("technical")
    .build();
```

---

## 3. Factory Method Pattern

**Definition:** Defines an interface for creating objects, but lets subclasses or static methods decide which class to instantiate or how to construct objects.

### Implementation

Three factory classes centralize object creation logic and business rules. The factories work seamlessly with builders to create complex objects.

#### Example 1: CustomerFactory (Combined with Builder)
```cpp
class CustomerFactory {
public:
    static std::shared_ptr<Customer> createCustomer(
        const std::string& id,
        const std::string& name,
        const std::string& email,
        const std::string& phone,
        CustomerType type
    ) {
        std::string prefixedName = getTypePrefix(type) + name;
        return CustomerBuilder()
            .withId(id)
            .withName(prefixedName)
            .withEmail(email)
            .withPhone(phone)
            .withType(type)
            .build();
    }
    
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
```

**Usage Locations:**

**In CustomerService::registerCustomer():**
```cpp
std::string customerId = "CUST-" + std::to_string(++customerCounter);

auto customer = CustomerFactory::createCustomer(
    customerId,
    name,
    email,
    phone,
    type
);

customerRepo.save(*customer);
logger->log("Customer registered: " + customerId + " - " + name + 
           " (Type: " + CustomerFactory::getTypeName(type) + ")");
```

**In CustomerDisplayFormatter:**
```cpp
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
```

**Purpose:** Centralizes all customer type-related logic and string conversions.

#### Example 2: TicketFactory
```cpp
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
```

**Usage in TicketService::createTicket():**
```cpp
std::string assignedAgent = TicketFactory::getAutoAssignedAgent(priority);

TicketBuilder builder;
builder.withId(ticketId)
       .withCustomerId(customerId)
       .withDescription(description)
       .withPriority(priority)
       .withCategory(category);

if (!assignedAgent.empty()) {
    builder.withAssignedTo(assignedAgent);
}

auto defaultTags = TicketFactory::getDefaultTags(category);
for (const auto& tag : defaultTags) {
    builder.addTag(tag);
}

logger->log("Ticket created: " + ticketId + " for customer " + customer->getName() +
           " (Category: " + TicketFactory::getCategoryName(category) + 
           ", Priority: " + TicketFactory::getPriorityName(priority) + ")");
```

**Usage in TicketService::updateTicketStatus():**
```cpp
std::string statusStr = TicketFactory::getStatusName(newStatus);
std::string message = "Your ticket " + ticketId + 
                    " status has been updated to: " + statusStr;
notificationService.notify(customer->getEmail(), message);

logger->log("Ticket status updated: " + ticketId + " to " + 
           TicketFactory::getStatusName(newStatus));
```

**Usage in TicketDisplayFormatter:**
```cpp
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
    return ss.str();
}
```

**Purpose:** Centralizes all ticket-related creation logic, string conversions, and business rules.

### Why This Matters

- **Centralized Logic:** All type-specific behavior in one place
- **Business Rules:** Auto-assignment and tagging logic encapsulated
- **Consistency:** Same string representations used everywhere
- **Maintainability:** Change display format in one location
- **Extensibility:** Easy to add new types or categories

### Factory Method Benefits

- **Single Source of Truth:** Type-specific logic centralized  
- **Easy Modification:** Change behavior by modifying factory  
- **Consistent Output:** Same logic used throughout application  
- **Business Logic Encapsulation:** Rules hidden from clients  
- **Reduced Duplication:** No scattered switch statements  

### Real-World Factory Behavior

**Auto-Assignment Logic:**
```
Priority: CRITICAL → Assigned to: Senior-Agent-001
Priority: HIGH     → Assigned to: Agent-002
Priority: MEDIUM   → No auto-assignment
Priority: LOW      → No auto-assignment
```

**Auto-Tagging Logic:**
```
Category: TECHNICAL        → Tags: [new, technical-support]
Category: BILLING          → Tags: [new, finance]
Category: COMPLAINT        → Tags: [new, urgent]
Category: FEATURE_REQUEST  → Tags: [new, product]
Category: GENERAL          → Tags: [new]
```

---

## Pattern Interactions

### How Patterns Work Together

The three patterns complement each other to create a robust system:

```
User Creates Ticket
    ↓
CLI receives input
    ↓
TicketService.createTicket()
    ↓
┌─────────────────────────────────────────────────────┐
│ SINGLETON: Get repositories and notification service │
│   - InMemoryTicketRepository::getInstance()         │
│   - InMemoryCustomerRepository::getInstance()       │
│   - NotificationService::getInstance()              │
└─────────────────────────────────────────────────────┘
    ↓
┌─────────────────────────────────────────────────────┐
│ FACTORY: Get business logic                         │
│   - TicketFactory::getAutoAssignedAgent(priority)   │
│   - TicketFactory::getDefaultTags(category)         │
└─────────────────────────────────────────────────────┘
    ↓
┌─────────────────────────────────────────────────────┐
│ BUILDER: Construct complex ticket object            │
│   TicketBuilder()                                   │
│     .withId(ticketId)                               │
│     .withCustomerId(customerId)                     │
│     .withDescription(description)                   │
│     .withPriority(priority)                         │
│     .withCategory(category)                         │
│     .withAssignedTo(agent)                          │
│     .addTag(tag1)                                   │
│     .addTag(tag2)                                   │
│     .build()                                        │
└─────────────────────────────────────────────────────┘
    ↓
Save to Singleton Repository
    ↓
Notify via Singleton NotificationService
```

### Pattern Synergy

**Singleton + Builder:**
- Singleton repositories store objects created by builders
- Single notification service uses builder-created objects

**Factory + Builder:**
- Factory encapsulates builder usage for clean object creation
- CustomerFactory uses CustomerBuilder internally
- TicketFactory provides business logic (agent, tags) and uses TicketBuilder

**Singleton + Factory:**
- Singleton ensures consistent factory usage
- Factory methods work with singleton repositories

---

## Sample Output

### Starting the Application

```
 Welcome to Customer Service System  

========================================
   Customer Service Management System
========================================
1. Customer Management
2. Ticket Management
3. View All Customers
4. View All Tickets
0. Exit
========================================
Enter your choice: 1
```

### Registering a VIP Customer (Builder + Factory)

```
--- Customer Management ---
1. Register New Customer
2. Find Customer by ID
0. Back to Main Menu
Enter your choice: 1

--- Register New Customer ---
Enter customer name: Alice Johnson
Enter email: alice.johnson@email.com
Enter phone: +1234567890

Select Customer Type:
1. Regular
2. Premium
3. VIP
Enter choice (1-3): 3

[LOG 14:32:15] Customer registered: CUST-1001 - Alice Johnson (Type: VIP)

Customer registered successfully!
Customer ID: CUST-1001
```

**Behind the Scenes:**
1. **Factory** creates customer using internal Builder
2. **Factory** applies "[VIP] " prefix during construction
3. **Singleton** repository stores the customer

### Creating a Critical Technical Ticket (All Patterns)

```
--- Create New Ticket ---
Enter customer ID: CUST-1001
Enter ticket description: Server is down, production affected

Select Category:
1. Technical
2. Billing
3. General
4. Complaint
5. Feature Request
Enter choice (1-5): 1

Select Priority:
1. Low
2. Medium
3. High
4. Critical
Enter choice (1-4): 4

[LOG 14:35:22] Ticket created: TKT-1001 for customer [VIP] Alice Johnson (Category: Technical, Priority: Critical)

[EMAIL] To: alice.johnson@email.com
[EMAIL] Message: Your ticket TKT-1001 has been created. Category: Technical. Description: Server is down, production affected

[SMS] To: alice.johnson@email.com
[SMS] Message: Your ticket TKT-1001 has been created. Category: Technical. Description: Server is down, production affected

[PUSH] To: alice.johnson@email.com
[PUSH] Message: Your ticket TKT-1001 has been created. Category: Technical. Description: Server is down, production affected

[LOG 14:35:22] Notification sent via Email to alice.johnson@email.com
[LOG 14:35:22] Notification sent via SMS to alice.johnson@email.com
[LOG 14:35:22] Notification sent via Push to alice.johnson@email.com

Ticket created successfully!
Ticket ID: TKT-1001
```

**Behind the Scenes:**
1. **Singleton** customer repository finds customer
2. **Factory** auto-assigns "Senior-Agent-001" (CRITICAL priority)
3. **Factory** generates default tags: ["new", "technical-support"]
4. **Builder** constructs ticket with all properties
5. **Singleton** ticket repository saves ticket
6. **Singleton** notification service sends notifications

### Viewing the Created Ticket

```
========== All Tickets ==========
Ticket ID: TKT-1001
Customer ID: CUST-1001
Description: Server is down, production affected
Category: Technical
Priority: Critical
Status: Open
Assigned To: Senior-Agent-001
Tags: new, technical-support
-----------------------------------
```

**Factory in Action:**
- Category name: `TicketFactory::getCategoryName()`
- Priority name: `TicketFactory::getPriorityName()`
- Status name: `TicketFactory::getStatusName()`

---

## Benefits Achieved

### 1. Controlled Instantiation (Singleton)
- Single point of data access prevents inconsistencies
- Global access without global variables
- Lazy initialization saves resources
- Thread-safe by design

### 2. Readable Construction (Builder)
- Complex objects built with clear, fluent syntax
- Optional parameters handled elegantly
- No constructor overload explosion
- Self-documenting code

### 3. Centralized Logic (Factory)
- Business rules in one location
- Consistent behavior across application
- Easy to modify type-specific behavior
- Reduces code duplication

### 4. Maintainability
- Change factory logic without touching clients
- Modify builder steps without breaking existing code
- Singleton ensures data consistency
- Each pattern has a clear responsibility

### 5. Extensibility
- Add new customer types in factory
- Add new ticket properties in builder
- Singleton pattern doesn't limit functionality
- Patterns work independently and together

### 6. Testability
- Mock singleton instances for testing
- Test builder construction logic separately
- Verify factory output independently
- Patterns enable unit testing

---

## Pattern Comparison Table

| Pattern | Purpose | Key Benefit | Use Case in Project |
|---------|---------|-------------|---------------------|
| **Singleton** | One instance only | Global access, consistency | Repositories, NotificationService |
| **Builder** | Step-by-step construction | Readable complex object creation | Customer, Ticket creation |
| **Factory Method** | Centralize creation logic | Business rule encapsulation | Type conversions, auto-assignment |


## Key Takeaways

### Singleton Pattern
- Use when exactly one instance is needed  
- Provides global access without global variables  
- Ensures data consistency across application  
- Thread-safe with modern C++ static initialization  

### Builder Pattern
- Use for objects with many parameters  
- Makes construction code readable and maintainable  
- Handles optional parameters elegantly  
- Separates construction from representation  

### Factory Method Pattern
- Use to centralize creation logic  
- Encapsulates business rules  
- Provides consistent behavior  
- Makes code easier to modify and extend  

---

## Conclusion

This customer service application demonstrates three fundamental creational design patterns working in harmony:

1. **Singleton Pattern**: Ensures single instances of critical components (repositories, notification service)
2. **Builder Pattern**: Constructs complex objects (customers, tickets) with readable, fluent code
3. **Factory Method Pattern**: Centralizes object creation logic and business rules

The combination of these patterns results in a system that is:
- **Consistent:** Single instances ensure data integrity
- **Readable:** Fluent builders create self-documenting code
- **Maintainable:** Centralized factories make changes easy
- **Extensible:** New types and features integrate smoothly
- **Professional:** Demonstrates industry-standard design practices

These patterns solve real problems in object creation and management, making the codebase more robust and easier to work with as the system grows.