#ifndef PROGRAM2_SERVER_HPP
#define PROGRAM2_SERVER_HPP

#include <cstdint>
#include <string>

namespace detail
{
class Server
{
public:
    Server(const std::string&, const uint32_t, const uint32_t);
    Server(const Server&) = delete;
    Server(Server&&) = delete;
    ~Server();

    void start();
    
private:

    int32_t server_sock_;
    char* buf_;
    uint32_t buf_size_;
    uint32_t max_queued_connections_;

    std::string server_path_;

    bool openToConnect();
};
} // namespace detail

#endif // PROGRAM2_SERVER_HPP
