#include "whitelistportentry.h"

bool WhitelistPortEntry::operator==(const WhitelistPortEntry &other) const {
    return this->portFrom == other.portFrom && this->portTo == other.portTo &&
           this->protocolFlag == other.protocolFlag;
}

bool WhitelistPortEntry::operator<(const WhitelistPortEntry &other) const {
    return this->portFrom < other.portFrom || this->portTo < other.portTo;
}
