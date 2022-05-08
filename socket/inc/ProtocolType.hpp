#pragma once

#include <netinet/in.h>

namespace SockLib
{
    enum class ProtocolType
    {
        ipv4 = PF_INET,
        ipv6 = PF_INET6,
        unspec = PF_UNSPEC
    };
}