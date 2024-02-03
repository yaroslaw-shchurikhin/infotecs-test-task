#include <thread>
#include <mutex>
#include <exception>
#include <iostream>

#include "detail/handler.hpp"
#include "detail/buffer.hpp"
#include "ui/user_interface.hpp"

int32_t main()
{
    try
    {
        detail::Buffer<std::string> buf;

        std::exception_ptr exc_ptr = nullptr;
        std::mutex exc_mutex;
        std::mutex exc_occurred;
        exc_occurred.lock();

        // initialize ui thread
        std::thread ui_thread([&buf, &exc_ptr, &exc_mutex, &exc_occurred]()
        {
            try
            {
                ui::UserInterface ui(std::cin);
                ui.start(buf);
            }
            catch(...)
            {
                exc_mutex.lock();
                exc_ptr = std::current_exception();
                exc_occurred.unlock();
                exc_mutex.unlock();
            }
        });

        // initialize handler thread
        std::thread handler_thread([&buf, &exc_ptr, &exc_mutex, &exc_occurred]()
        {   
            try
            {
                detail::Handler handler;
                handler.start(buf);
            }
            catch(...)
            {
                exc_mutex.lock();
                exc_ptr = std::current_exception();
                exc_occurred.unlock();
                exc_mutex.unlock();
            }
        });

        // start ui and handler threads
        ui_thread.detach();
        handler_thread.detach();

        // Waiting for exceptions from ui or handler
        exc_occurred.lock();
        if(exc_ptr)
        {
            std::rethrow_exception(exc_ptr);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}
