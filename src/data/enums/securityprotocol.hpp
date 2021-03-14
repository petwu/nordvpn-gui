#ifndef TECHNOLOGYPROTOCOL_HPP
#define TECHNOLOGYPROTOCOL_HPP

/**
 * @brief The SecurityProtocol enum identifies a VPN security protocol used by
 * data from the NordVPN API.
 */
enum class SecurityProtocol {
    Undefined,
    // OpenVPN
    OpenVPN_UDP,
    OpenVPN_TCP,
    OpenVPN_UDP_IPv6,
    OpenVPN_TCP_IPv6,
    OpenVPN_UDP_TLS_Crypt,
    OpenVPN_TCP_TLS_Crypt,
    OpenVPN_UDP_Dedicated,
    OpenVPN_TCP_Dedicated,
    OpenVPN_UDP_Obfuscated,
    OpenVPN_TCP_Obfuscated,
    // NordLynx
    Wireguard,
    // Proxies
    Socks5,
    HTTP_Proxy,
    HTTP_Proxy_SSL,
    HTTP_CyberSec_Proxy,
    HTTP_CyberSec_Proxy_SSL,
    // IPSec
    IKEv2_IPSec,
    IKEv2_IPSec_IPv6,

    // PPTP       --> deprecated since 2018-12-01
    // L2TP_IPSec --> deprecated since 2018-12-01
    //                (https://nordvpn.com/de/blog/l2tp-pptp-protocol-update)
    // Skylark    --> experimantal?
    //                (only 1 server as of 2020-10-11 available)
};

#endif // TECHNOLOGYPROTOCOL_HPP
