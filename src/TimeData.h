#pragma once
#include <Arduino.h>

struct TimeData
{
    uint16_t year;
    uint8_t month, day, dow;
    uint8_t hour, minute, second;
    float temperature;
};

extern TimeData g_timeData;
extern SemaphoreHandle_t g_timeMutex;
