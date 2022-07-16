#pragma once

#include <ctime>

class Timer
{
    bool m_ended = false;

    bool m_paused = true;

    time_t m_endTime;
    time_t m_timeLeft;
    time_t m_currentTime;

public:
    Timer(time_t duration = 0, bool start = false);

    void setDuration(time_t duration);

    void pause();
    void start();

    void tick();

    int getSeconds();
    int getMinutes();

    bool ended();
};
