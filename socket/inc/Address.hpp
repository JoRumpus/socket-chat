#pragma once

#include <netdb.h>
#include <netinet/in.h>
#include <memory>

#include "AddressType.hpp"
#include "exceptions.hpp"
#include "ProtocolType.hpp"
#include "SocketType.hpp"


namespace SockLib
{
    class Address
    {
    private:

        sockaddr_storage _addr {};
        size_t _addrLen {0};
        AddressType _addrType {AddressType::unspec};
        ProtocolType _protType {ProtocolType::unspec};
        SocketType _sockType {SocketType::stream};      

    public:

        Address(const addrinfo& addrInfo) : 
        Address(
            *(addrInfo.ai_addr),
            addrInfo.ai_addrlen,
            static_cast<AddressType>(addrInfo.ai_family),
            static_cast<ProtocolType>(addrInfo.ai_protocol),
            static_cast<SocketType>(addrInfo.ai_socktype)) {};

        Address(const sockaddr_in& addr, SocketType sockType) : 
        Address(
            reinterpret_cast<const sockaddr&>(addr),
            sizeof(addr),
            AddressType::ipv4,
            ProtocolType::ipv4,
            sockType) {};

        Address(const sockaddr_in6& addr, SocketType sockType) : 
        Address(
            reinterpret_cast<const sockaddr&>(addr),
            sizeof(addr),
            AddressType::ipv6,
            ProtocolType::ipv6,
            sockType) {};

        Address(const sockaddr& addr, 
                size_t addrLen, 
                AddressType addrType, 
                ProtocolType protType, 
                SocketType sockType) : 
                _addrLen {addrLen},
                _addrType {addrType},
                _protType {protType},
                _sockType {sockType} 
        {
            memcpy(&_addr, &addr, addrLen);
        };


        const sockaddr& addr() const
        {
            return reinterpret_cast<const sockaddr&>(_addr);
        }

        size_t addrLen() const
        {
            return _addrLen;
        }

        AddressType addrType() const
        {
            return _addrType;
        }

        ProtocolType protType() const
        {
            return _protType;
        }

        SocketType sockType() const
        {
            return _sockType;
        }
    };


    struct AddressHint
    {
        AddressType addrType {AddressType::unspec};
        SocketType sockType {SocketType::stream};
    };

    std::vector<Address> getAvailableAddresses(const std::string& node, 
                                               const std::string& port, 
                                               const AddressHint& hint)
    {
        addrinfo _hint 
        {
            .ai_family = static_cast<int>(hint.addrType),
            .ai_socktype = static_cast<int>(hint.sockType)
        };

        std::unique_ptr<addrinfo, void(*)(addrinfo*)> result(nullptr, freeaddrinfo);
        int errCode = getaddrinfo(node.data(), 
                                 port.data(), 
                                 &_hint, 
                                 reinterpret_cast<addrinfo**>(&result));
        if (errCode != 0)
        {
            throw AddressQueryError(errCode);
        }

        std::vector<Address> validAddresses;
        for (addrinfo* addr = result.get(); addr != nullptr; addr = addr->ai_next)
        {
            validAddresses.push_back(Address(*addr));
        }

        return validAddresses; 
    }
}
