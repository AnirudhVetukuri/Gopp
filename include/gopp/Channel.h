#pragma once
#include <mutex>
#include <atomic>
#include <deque>
#include <stdexcept>
#include <cstdint>

namespace gopp
{

enum class ChannelState : uint8_t
{
    DEFAULT          = 0b0000,
    WRITER_BLOCKING  = 0b0001,
    READER_BLOCKING  = 0b0010,
    WRITE_COMPLETE   = 0b0100,
    CLOSED           = 0b1000
};

template <typename T>
class ChannelBase
{
public:
    virtual operator bool() = 0;
};

template <typename T>
class ReadChannel : public virtual ChannelBase<T>
{
public:
    virtual bool readReady() = 0;
    virtual bool read(T &out) = 0;
};

template <typename T>
class WriteChannel : public virtual ChannelBase<T>
{
public:
    virtual bool writeReady() = 0;
    virtual bool write(const T &in) = 0;
    virtual void close() = 0;
};

template <typename T>
class Channel : public ReadChannel<T>, public WriteChannel<T>
{
private:
    std::mutex m_lock;
    T m_data{};
    const size_t m_bufferSize;
    std::deque<T> m_buffer;
    std::atomic<uint8_t> m_state{static_cast<uint8_t>(ChannelState::DEFAULT)};

    // Helper functions for state management
    bool writeComplete() const { return m_state.load() & static_cast<uint8_t>(ChannelState::WRITE_COMPLETE); }
    bool isClosed() const { return m_state.load() & static_cast<uint8_t>(ChannelState::CLOSED); }

    void setState(ChannelState st)
    {
        m_state.fetch_or(static_cast<uint8_t>(st));
    }
    void unsetState(ChannelState st)
    {
        m_state.fetch_and(~static_cast<uint8_t>(st));
    }

    // Non-blocking read attempt.
    bool tryRead(T &out)
    {
        std::lock_guard<std::mutex> lock(m_lock);
        if (!m_buffer.empty())
        {
            out = m_buffer.front();
            m_buffer.pop_front();
            return true;
        }
        else if (writeComplete())
        {
            out = m_data;
            unsetState(ChannelState::WRITE_COMPLETE);
            return true;
        }
        else if (isClosed())
        {
            out = T{};
        }
        return false;
    }

    // Non-blocking write attempt.
    bool tryWrite(const T &in)
    {
        if (isClosed())
            throw std::runtime_error("Attempted write on a closed channel!");

        std::lock_guard<std::mutex> lock(m_lock);
        if (m_buffer.size() < m_bufferSize)
        {
            m_buffer.push_back(in);
            return true;
        }
        else if (!writeComplete())
        {
            m_data = in;
            setState(ChannelState::WRITE_COMPLETE);
            return true;
        }
        return false;
    }

public:
    Channel(size_t bufferSize = 0)
        : m_bufferSize(bufferSize)
    {
    }

    // Deleted copy/move constructors.
    Channel(const Channel &) = delete;
    Channel(Channel &&) = delete;

    // Read channel interface.
    bool readReady() override
    {
        setState(ChannelState::READER_BLOCKING);
        return true;
    }

    bool read(T &out) override
    {
        setState(ChannelState::READER_BLOCKING);
        while (true)
        {
            if (tryRead(out))
            {
                unsetState(ChannelState::READER_BLOCKING);
                return true;
            }
            else if (isClosed())
            {
                unsetState(ChannelState::READER_BLOCKING);
                return false;
            }
            Machines::yieldToScheduler();
        }
    }

    // Write channel interface.
    bool writeReady() override
    {
        setState(ChannelState::WRITER_BLOCKING);
        return true;
    }

    bool write(const T &in) override
    {
        setState(ChannelState::WRITER_BLOCKING);
        while (true)
        {
            if (tryWrite(in))
            {
                while (writeComplete())
                {   
                    // Add scheduler yield functionality
                    Device::yieldToScheduler();
                }
                unsetState(ChannelState::WRITER_BLOCKING);
                return true;
            }
            Device::yieldToScheduler();
        }
    }

    void close() override
    {
        m_state.store(static_cast<uint8_t>(ChannelState::CLOSED));
    }

    operator bool() override
    {
        // For a closed channel we return false.
        return !isClosed();
    }
};

// Operator overloads for convenience.
template <typename T, template <typename> class WriteChannelType>
auto &operator<<(WriteChannelType<T> &ch, const T &in)
{
    ch.write(in);
    return ch;
}

template <typename T, template <typename> class ReadChannelType>
auto &operator>>(ReadChannelType<T> &ch, T &out)
{
    if (!ch.read(out))
    {
        // This returns a special end-of-channel marker.
        static Channel<T> eoc;
        return eoc;
    }
    return ch;
}

} 
