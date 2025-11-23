// src/RTCTask/RTCTask.cpp
#include "RTCTask.h"
#include <Wire.h>
#include "RTClib.h"

// piny I2C
#define I2C_SDA 39
#define I2C_SCL 38

RTC_DS3231 rtc;

static const char *daysOfTheWeek[7] = {
    "Niedziela", "Poniedziałek", "Wtorek", "Środa",
    "Czwartek", "Piątek", "Sobota"};

void RTC_task(void *parameter)
{
    // I2C + RTC init
    Wire.begin(I2C_SDA, I2C_SCL);

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        vTaskDelete(NULL); // zabij task jak nie ma RTC
    }

    // jeśli chcesz jednorazowo ustawić czas przy pierwszym starcie : if (rtc.lostPower())
    // {
    //     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // }

    for (;;)
    {
        DateTime now = rtc.now();

        Serial.print(now.year(), DEC);
        Serial.print('-');
        Serial.print(now.month(), DEC);
        Serial.print('-');
        Serial.print(now.day(), DEC);
        Serial.print(" (");
        Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
        Serial.print(") ");
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.println();

        Serial.print("Temperature: ");
        Serial.print(rtc.getTemperature());
        Serial.println(" C");
        Serial.println();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
