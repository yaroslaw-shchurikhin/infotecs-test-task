#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include <iosfwd>
#include <condition_variable>
#include <mutex>
#include <atomic>

#include "../detail/buffer.hpp"

namespace ui
{

class UserInterface
{
public:
   UserInterface(std::istream&);
   ~UserInterface() = default;

   UserInterface(const UserInterface&) = delete;
   UserInterface(UserInterface&&) = delete;
   
   void start(detail::Buffer<std::string>&); 

private:
   std::istream& in_; 
   
   // i wanted to add boost flag saver (ios_flags_saver)
   // but as far as I understood from the task, 
   // it is not worth using third-party libraries
};
} // namespace ui

#endif // USER_INTERFACE_HPP
