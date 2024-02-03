#include <exception>
#include <iostream>
#include <cstdint>

#include "detail/server.hpp"
#include "../../config.hpp"

int32_t main()
{
    constexpr char SOCK_PATH[] = EXCHANGE_FILE;
    constexpr uint32_t BUF_SIZE = 256;
    constexpr uint32_t MAX_QUEUED_CONNECTIONS = 1;

    try
    {
        detail::Server server(SOCK_PATH, BUF_SIZE, MAX_QUEUED_CONNECTIONS);
        server.start();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
