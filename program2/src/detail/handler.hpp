#ifndef PROGRAM2_HANDLER_HPP
#define PROGRAM2_HANDLER_HPP

#include <cstdint>

namespace detail
{
class Handler
{
public:
    Handler(int32_t, uint32_t);
    Handler(const Handler&) = delete;
    Handler(Handler&&) = delete;
    ~Handler();

    void start();

private:

    int32_t client_sock_;
    char* buf_;
    uint32_t buf_size_;
};
} // namespace detail
#endif // PROGRAM2_HANDLER_HPP
