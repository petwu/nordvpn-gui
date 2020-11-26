#include "envinfo.h"

std::string EnvInfo::toString() const {
    return std::string("EnvInfo {") +                     //
           "\n  internetConnected   = " +                 //
           (this->internetConnected ? "true" : "false") + //
           "\n  shellAvailable      = " +                 //
           (this->shellAvailable ? "true" : "false") +    //
           "\n  nordvpnInstalled    = " +                 //
           (this->nordvpnInstalled ? "true" : "false") +  //
           "\n  loggedIn            = " +                 //
           (this->loggedIn ? "true" : "false") +          //
           "\n}";
}
