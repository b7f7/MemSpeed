#ifndef STD_TIMER_H
#define STD_TIMER_H
#include <atomic>

#include <chrono>


class StdTimer
{
public:
    void Start()
    {
        m_StartTime.store(m_Clock.now());
    }
    double GetTime() const
    {
        auto now = m_Clock.now();
        double delta = std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_StartTime.load()).count();

        return delta/1000000000.0;
    }
private:
    typedef std::chrono::high_resolution_clock clock_type;
    static_assert(clock_type::is_steady, "we need a steady clock to work");

    typedef clock_type::time_point time_point_type;

    clock_type  m_Clock;
    std::atomic<time_point_type> m_StartTime;
};


#endif // STD_TIMER_H
