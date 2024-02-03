#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <sys/un.h>

#include "buffer.hpp"

namespace detail
{

class Handler
{
public:
    Handler();
    Handler(const Handler&) = delete;
    Handler(Handler&&) = delete;
    ~Handler();

    void start(Buffer<std::string>&);

private:
    int32_t client_sock_;
    char* socket_buf_;
    struct sockaddr_un server_sockaddr_{};
    bool connected_;

    void connectToServer();
};

} // namespace detail
#endif // HANDLER_HPP
