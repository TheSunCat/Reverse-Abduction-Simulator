#include "Timer.h"

#include "Util.h"

Timer::Timer(time_t duration, bool start) : m_paused(!start), m_timeLeft(duration)
{
    if(start)
        m_endTime = Util::currentTimeMillis() + m_timeLeft;

    m_currentTime = Util::currentTimeMillis();
}


void Timer::pause()
{
    m_paused = true;
}

void Timer::start()
{
    m_paused = false;
    m_endTime = Util::currentTimeMillis() + m_timeLeft;
    m_currentTime = Util::currentTimeMillis();
}

void Timer::setDuration(time_t duration)
{
    m_timeLeft = duration;

    if(!m_paused)
        m_endTime = Util::currentTimeMillis() + m_timeLeft;
}

void Timer::tick()
{
    time_t curTime = Util::currentTimeMillis();

    time_t deltaTime = curTime - m_currentTime;

    m_timeLeft -= deltaTime;

    if(m_timeLeft < 0)
    {
        m_timeLeft = 0;
        pause();
    }

    m_currentTime = curTime;
}

int Timer::getSeconds()
{
    return int(m_timeLeft / 1000.f) % 60;
}

int Timer::getMinutes()
{
    return m_timeLeft / 60000.f;
}
