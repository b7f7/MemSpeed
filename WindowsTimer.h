#ifndef WINDOWSTIMER_H
#define WINDOWSTIMER_H
#include <Windows.h>
#include <atomic>

class WindowsTimer
{
public:
    WindowsTimer()
    {
        QueryPerformanceFrequency(&m_Frq);
    }
    void Start()
    {
        LARGE_INTEGER s;
        QueryPerformanceCounter(&s);
        m_StartTime.store(s);
    }
    double GetTime() const
    {
        LARGE_INTEGER start = m_StartTime.load();
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);

        return static_cast<double>(currentTime.QuadPart - start.QuadPart) / m_Frq.QuadPart;
    }
private:
    LARGE_INTEGER m_Frq;
    std::atomic<LARGE_INTEGER> m_StartTime;
};

#endif // WINDOWSTIMER_H
