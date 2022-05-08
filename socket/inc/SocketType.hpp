#pragma once

#include <netinet/in.h>

namespace SockLib
{
    enum class SocketType
    {
        stream = SOCK_STREAM,
        dgram = SOCK_DGRAM
    };
}