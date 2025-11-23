#include "RTCTask.h"
#include <Wire.h>
#include "RTClib.h"
#include "TimeData.h"

// I2C do DS3231
#define I2C_SDA 39
#define I2C_SCL 38

RTC_DS3231 rtc;
TimeData g_timeData;
SemaphoreHandle_t g_timeMutex = nullptr;

void RTC_task(void *parameter)
{
    Wire.begin(I2C_SDA, I2C_SCL);

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC");
        vTaskDelete(nullptr);
    }

    if (rtc.lostPower())
    {
        // jednorazowe ustawienie na czas kompilacji (możesz zmienić)
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    g_timeMutex = xSemaphoreCreateMutex();

    for (;;)
    {
        DateTime now = rtc.now();
        float temp = rtc.getTemperature();

        if (g_timeMutex && xSemaphoreTake(g_timeMutex, pdMS_TO_TICKS(10)) == pdTRUE)
        {
            g_timeData.year = now.year();
            g_timeData.month = now.month();
            g_timeData.day = now.day();
            g_timeData.dow = now.dayOfTheWeek();
            g_timeData.hour = now.hour();
            g_timeData.minute = now.minute();
            g_timeData.second = now.second();
            g_timeData.temperature = temp;
            xSemaphoreGive(g_timeMutex);
        }

        // vTaskDelay(pdMS_TO_TICKS(1000));
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
