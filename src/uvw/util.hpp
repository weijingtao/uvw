#pragma once


#include <stdexcept>
#include <cstddef>
#include <utility>
#include <string>
#include <uv.h>


namespace uvw {


namespace details {


struct IPv4 { };
struct IPv6 { };


template<typename>
struct IpTraits;

template<>
struct IpTraits<IPv4> {
    using Type = sockaddr_in;
    using AddrFuncType = int(*)(const char *, int, sockaddr_in *);
    using NameFuncType = int(*)(const sockaddr_in *, char *, std::size_t);
    static const AddrFuncType AddrFunc;
    static const NameFuncType NameFunc;
};

template<>
struct IpTraits<IPv6> {
    using Type = sockaddr_in6;
    using AddrFuncType = int(*)(const char *, int, sockaddr_in6 *);
    using NameFuncType = int(*)(const sockaddr_in6 *, char *, std::size_t);
    static const AddrFuncType AddrFunc;
    static const NameFuncType NameFunc;
};

const IpTraits<IPv4>::AddrFuncType IpTraits<IPv4>::AddrFunc = uv_ip4_addr;
const IpTraits<IPv6>::AddrFuncType IpTraits<IPv6>::AddrFunc = uv_ip6_addr;
const IpTraits<IPv4>::NameFuncType IpTraits<IPv4>::NameFunc = uv_ip4_name;
const IpTraits<IPv6>::NameFuncType IpTraits<IPv6>::NameFunc = uv_ip6_name;


}


class UVWException final: std::runtime_error {
public:
    explicit UVWException(int code)
        : runtime_error{uv_strerror(code)}, ec{code}
    { }

    const char* name() const noexcept {
        return uv_err_name(ec);
    }

    const char* error() const noexcept {
        return uv_strerror(ec);
    }

private:
    int ec;
};


/**
 * See Boost/Mutant idiom:
 *     https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Boost_mutant
 */
struct Addr { std::string ip; unsigned int port; };
struct WinSize { int width; int height; };


using TimeSpec = uv_timespec_t;
using Stat = uv_stat_t;


}
