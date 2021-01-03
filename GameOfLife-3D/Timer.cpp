#include "Timer.h"

using namespace std::chrono;

Timer::Timer() noexcept
    :
    last( steady_clock::now() )
{}

float Timer::Mark() noexcept
{
    const auto now = steady_clock::now();
    const duration<float> period = now - std::exchange( last,now );
    return period.count();
}

float Timer::Peek() noexcept
{
    const auto now = steady_clock::now();
    const duration<float> period = now - last;
    return period.count();
}
