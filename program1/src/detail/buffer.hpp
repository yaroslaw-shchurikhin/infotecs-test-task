#ifndef PROGRAM1_BUFFER_HPP
#define PROGRAM1_BUFFER_HPP

#include <deque>
#include <mutex>

namespace detail
{
template<class T>
class Buffer
{
public:
    Buffer() { readable_.lock(); };
    Buffer(const Buffer&) = delete;
    Buffer(Buffer&&) = delete;
    ~Buffer() = default;

    // adds element in the end of buffer
    void push_back(T&);

    // adds element in the beginning of buffer
    void push_front(T&);

    // returns first element of the queue and delete this element
    T pop();
    bool empty() const;

private:

std::deque<T> deque_;

// mutex used for synchronization in critical sections in methods
// recursive to be able to call methods out of other methods
// mutable for lock_guard in empty() method
// I think that it isn't obviously for user when empty() method don't have 'const' modifier
// so I decided to add 'mutable'
mutable std::recursive_mutex m_;

// mutex - locked if buffer is empty, otherwise unlocked
std::mutex readable_;
};

template<class T>
void Buffer<T>::push_back(T& value)
{
    std::lock_guard<std::recursive_mutex> lg(m_);
    deque_.push_back(value);
    readable_.unlock();
}

template<class T>
void Buffer<T>::push_front(T& value)
{
    std::lock_guard<std::recursive_mutex> lg(m_);
    deque_.push_front(value);
    readable_.unlock();
}

template<class T>
T Buffer<T>::pop()
{
    readable_.lock();
    std::lock_guard<std::recursive_mutex> lg(m_);

    T value = std::move(deque_.front());
    deque_.pop_front();

    if(!empty())
    {
        readable_.unlock();
    }
    return value;
}

template<class T>
bool Buffer<T>::empty() const
{
    std::lock_guard<std::recursive_mutex> lg(m_);
    return deque_.empty();
}
} // namespace detail
#endif // BUFFER_HPP
