#pragma once

#include <chrono>

namespace ModernChess
{
    template<class DT = std::chrono::milliseconds,
             class ClockT = std::chrono::steady_clock>
    class Timer
    {
    public:
        void start()
        {
            _start = ClockT::now();
        }

        template<class T = DT>
        auto duration() const
        {
            return std::chrono::duration_cast<T>(ClockT::now() - _start);
        }

    private:
        using TimePoint = typename ClockT::time_point;
        TimePoint _start = ClockT::now();
    };
}