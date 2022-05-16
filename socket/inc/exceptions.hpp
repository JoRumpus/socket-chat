#pragma once

#include <string>
#include <exception>
#include <netdb.h>

namespace SockLib
{
    class AddressQueryError : public std::exception
    {
    private:

        int _errCode = 0;

    public:

        AddressQueryError(int errCode) : _errCode {errCode} {};

        virtual const char* what() const noexcept final override
        {
            return gai_strerror(_errCode);
        }

        int errCode()
        {
            return _errCode;
        }
    };
}
