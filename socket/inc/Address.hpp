#pragma once

#include <netinet/in.h>

#include "AddressType.hpp"
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

        Address(const addrinfo& addrInfo);

        Address(const sockaddr_in& addr, SocketType sockType);

        Address(const sockaddr_in6& addr, SocketType sockType);

        Address(const sockaddr& addr, 
                size_t addrLen, 
                AddressType addrType, 
                ProtocolType protType, 
                SocketType sockType);


        const sockaddr& addr() const;

        size_t addrLen() const;

        AddressType addrType() const;

        ProtocolType protType() const;

        SocketType sockType() const;
    };


    struct AddressHint
    {
        AddressType addrType {AddressType::unspec};
        SocketType sockType {SocketType::stream};
    };

    std::vector<Address> getAvailableAddresses(const std::string& node, 
                                               const std::string& port, 
                                               const AddressHint& hint);
}
