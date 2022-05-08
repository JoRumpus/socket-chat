#pragma once

#include <netinet/in.h>

namespace SockLib
{
    enum class AddressType
    {
        ipv4 = AF_INET,
        ipv6 = AF_INET6,
        unspec = AF_UNSPEC
    };
}