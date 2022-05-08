#include <gtest/gtest.h>
#include <netdb.h>
#include <netinet/in.h>
#include <cstring>

#include <AddressType.hpp>
#include <ProtocolType.hpp>
#include <SocketType.hpp>

/* MOCKING C API - DECLARATIONS */
#define getaddrinfo(a, b, c, d) mock_getaddrinfo(d)
int mock_getaddrinfo(addrinfo** outAddrInfo_p);
sockaddr mock_sockaddrValue();
addrinfo mock_addrinfoValue();

#include <Address.hpp>

TEST(Address, ConstructorAddrInfo)
{
    sockaddr mockAddr{};
    addrinfo mockAddrInfo = {
        .ai_family = static_cast<int>(SockLib::AddressType::ipv4),
        .ai_protocol = static_cast<int>(SockLib::ProtocolType::ipv4),
        .ai_socktype = static_cast<int>(SockLib::SocketType::stream),
        .ai_addrlen = sizeof(sockaddr_in),
        .ai_addr = &mockAddr
    };

    SockLib::Address addr(mockAddrInfo);

    // Check that all members correctly describe address
    ASSERT_EQ(addr.addrLen(), mockAddrInfo.ai_addrlen);
    ASSERT_EQ(addr.addrType(), static_cast<SockLib::AddressType>(mockAddrInfo.ai_family));
    ASSERT_EQ(addr.protType(), static_cast<SockLib::ProtocolType>(mockAddrInfo.ai_protocol));
    ASSERT_EQ(addr.sockType(), static_cast<SockLib::SocketType>(mockAddrInfo.ai_socktype));

    // Check if address itself is correct
    ASSERT_TRUE(memcmp(&addr.addr(), mockAddrInfo.ai_addr, addr.addrLen()) == 0);
}

TEST(Address, ConstructorIn)
{
    sockaddr_in addrBaseIn {};
    memset(&addrBaseIn, 167, sizeof(addrBaseIn));
    SockLib::Address addr(addrBaseIn, SockLib::SocketType::stream);
    
    // Check that all members correctly describe address
    ASSERT_EQ(addr.addrLen(), sizeof(addrBaseIn));
    ASSERT_EQ(addr.addrType(), SockLib::AddressType::ipv4);
    ASSERT_EQ(addr.protType(), SockLib::ProtocolType::ipv4);
    ASSERT_EQ(addr.sockType(), SockLib::SocketType::stream);

    // Check if address itself is correct
    ASSERT_TRUE(memcmp(&addr.addr(), &addrBaseIn, addr.addrLen()) == 0);
}

TEST(Address, ConstructorIn6)
{
    sockaddr_in6 addrBaseIn6 {};
    memset(&addrBaseIn6, 127, sizeof(addrBaseIn6));
    SockLib::Address addr(addrBaseIn6, SockLib::SocketType::dgram);
    
    // Check that all members correctly describe address
    ASSERT_EQ(addr.addrLen(), sizeof(addrBaseIn6));
    ASSERT_EQ(addr.addrType(), SockLib::AddressType::ipv6);
    ASSERT_EQ(addr.protType(), SockLib::ProtocolType::ipv6);
    ASSERT_EQ(addr.sockType(), SockLib::SocketType::dgram);
    
    // Check if address itself is correct
    ASSERT_TRUE(memcmp(&addr.addr(), &addrBaseIn6, addr.addrLen()) == 0);
}

TEST(Address, GetAvailableAddresses)
{
    auto validAddresses = SockLib::getAvailableAddresses("", "", SockLib::AddressHint{});
    ASSERT_EQ(validAddresses.size(), 1);
    SockLib::Address addr = validAddresses[0];

    addrinfo mockAddrInfo = mock_addrinfoValue();
    sockaddr mockAddr = mock_sockaddrValue();

    // Check that all members correctly describe address
    ASSERT_EQ(addr.addrLen(), mockAddrInfo.ai_addrlen);
    ASSERT_EQ(addr.addrType(), static_cast<SockLib::AddressType>(mockAddrInfo.ai_family));
    ASSERT_EQ(addr.protType(), static_cast<SockLib::ProtocolType>(mockAddrInfo.ai_protocol));
    ASSERT_EQ(addr.sockType(), static_cast<SockLib::SocketType>(mockAddrInfo.ai_socktype));
    
    // Check if address itself is correct
    ASSERT_TRUE(memcmp(&addr.addr(), &mockAddr, addr.addrLen()) == 0);
}

/* MOCKING C API - DEFINITIONS */

sockaddr mock_sockaddrValue()
{
    return {};
}

addrinfo mock_addrinfoValue()
{
 return {
        .ai_family = static_cast<int>(SockLib::AddressType::ipv4),
        .ai_protocol = static_cast<int>(SockLib::ProtocolType::ipv4),
        .ai_socktype = static_cast<int>(SockLib::SocketType::stream),
        .ai_addrlen = sizeof(sockaddr_in)
    };
}

int mock_getaddrinfo(addrinfo** outAddrInfo_p)
{
    // Make copy of target addrinfo to be freed
    // It's easier than mocking freeaddrinfo (especially with unique_ptr)
    addrinfo* mockAddrInfo = static_cast<addrinfo*>(malloc(sizeof(*mockAddrInfo)));
    sockaddr* mockAddr = static_cast<sockaddr*>(malloc(sizeof(*mockAddr)));
    *mockAddrInfo = mock_addrinfoValue();
    *mockAddr = mock_sockaddrValue();
    mockAddrInfo->ai_addr = mockAddr;

    *outAddrInfo_p = mockAddrInfo;
    return 0;
}

