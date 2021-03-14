#include "whitelistportentry.hpp"

auto WhitelistPortEntry::operator==(const WhitelistPortEntry &other) const
    -> bool {
    return this->portFrom == other.portFrom && this->portTo == other.portTo &&
           this->protocolFlag == other.protocolFlag;
}

auto WhitelistPortEntry::operator<(const WhitelistPortEntry &other) const
    -> bool {
    return this->portFrom < other.portFrom || this->portTo < other.portTo;
}
