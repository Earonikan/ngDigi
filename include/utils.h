#pragma once

#include "stdafx.h"


long GetCurrentTime()
{
    struct timeval t1;
    gettimeofday(&t1, NULL);
    return (t1.tv_sec) * 1000 + t1.tv_usec / 1000;
}