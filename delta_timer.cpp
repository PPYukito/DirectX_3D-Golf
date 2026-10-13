/*==============================================================================

   デルタタイマー [delta_timer.cpp]
                                                         Author : Youhei Sato
                                                         Date   : 2026/04/15
--------------------------------------------------------------------------------

==============================================================================*/
#include <Windows.h>
#include "delta_timer.h"

DeltaTimer::DeltaTimer()
    : m_frequency(0)
    , m_previous_counter(0)
{
    LARGE_INTEGER frequency{};
    QueryPerformanceFrequency(&frequency);
    m_frequency = frequency.QuadPart;

    Reset();
}

void DeltaTimer::Reset()
{
    LARGE_INTEGER current_counter{};
    QueryPerformanceCounter(&current_counter);
    m_previous_counter = current_counter.QuadPart;
}

float DeltaTimer::Tick()
{
    if (m_frequency <= 0) {
        return 0.0f;
    }

    LARGE_INTEGER current_counter{};
    QueryPerformanceCounter(&current_counter);

    const float delta_time = static_cast<float>(
        static_cast<double>(current_counter.QuadPart - m_previous_counter) /
        static_cast<double>(m_frequency));

    m_previous_counter = current_counter.QuadPart;

    return delta_time;
}
