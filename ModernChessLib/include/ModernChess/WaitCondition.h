#pragma once

#include <condition_variable>
#include <chrono>

namespace ModernChess
{

/**
 * @brief Equivalent to std::condition_variable with the difference that
 *        waitFor() is stable against time changes.
 *
 *        waitUntil() works only with std::chrono::time_point<std::chrono::steady_clock> in order
 *        to guarantee stable time measurements.
 */
    class WaitCondition {
    public:

        inline void notifyOne() noexcept
        {
            m_condVariable.notify_one();
        }

        inline void notifyAll() noexcept
        {
            m_condVariable.notify_all();
        }

        template<class Predicate>
        void wait(std::unique_lock<std::mutex>& lock, Predicate stopWaiting)
        {
            m_condVariable.wait(lock, stopWaiting);
        }

        template<class Rep, class Period, class Predicate>
        bool waitFor(std::unique_lock<std::mutex>& lock,
                     const std::chrono::duration<Rep, Period>& relTimeout,
                     Predicate stopWaiting)
        {
            // Use steady_clock for stable time measurement
            auto now = std::chrono::steady_clock::now();
            return m_condVariable.wait_until(lock, now + relTimeout, stopWaiting);
        }

        /**
         * @note supports only std::chrono::time_point<std::chrono::steady_clock> in order to
         *       guarantee stable time measurements
         */
        template<class Duration, class Predicate>
        bool waitUntil(std::unique_lock<std::mutex>& lock,
                       const std::chrono::time_point<std::chrono::steady_clock, Duration>& timeoutTime,
                       Predicate stopWaiting)
        {
            return m_condVariable.wait_until(lock, timeoutTime, stopWaiting);
        }

        auto nativeHandle()
        {
            return m_condVariable.native_handle();
        }

    private:
        std::condition_variable m_condVariable;
    };
}