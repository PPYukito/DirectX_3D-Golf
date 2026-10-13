/*==============================================================================

   デルタタイマー [delta_timer.h]
                                                         Author : Youhei Sato
                                                         Date   : 2026/04/15
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef DELTA_TIMER_H
#define DELTA_TIMER_H

class DeltaTimer
{
public:
    DeltaTimer();

    void Reset();
    float Tick();

private:
    long long m_frequency;
    long long m_previous_counter;
};

#endif // DELTA_TIMER_H
