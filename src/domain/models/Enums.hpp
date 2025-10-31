#ifndef ENUMS_HPP
#define ENUMS_HPP

namespace domain {

enum class CustomerType {
    REGULAR,
    PREMIUM,
    VIP
};

enum class TicketStatus {
    OPEN,
    IN_PROGRESS,
    RESOLVED,
    CLOSED
};

enum class Priority {
    LOW,
    MEDIUM,
    HIGH,
    CRITICAL
};

enum class TicketCategory {
    TECHNICAL,
    BILLING,
    GENERAL,
    COMPLAINT,
    FEATURE_REQUEST
};

} // namespace domain

#endif
