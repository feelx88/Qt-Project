#include "Clock.h"

#include <ctime>

int Clock::ticksPerSecond = CLOCKS_PER_SEC;

int Clock::getTime()
{
    return clock();
}
