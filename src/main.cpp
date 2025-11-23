#include <Arduino.h>
#include "ButtonsTask/ButtonsTask.h"
#include "RTCTask/RTCTask.h"
#include "DisplayTask/DisplayTask.h"

void setup()
{
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    delay(50);

    xTaskCreatePinnedToCore(
        buttons_task, "buttons",
        8192, nullptr, 1, nullptr, 1);

    xTaskCreatePinnedToCore(
        RTC_task, "RTC",
        8192, nullptr, 1, nullptr, 1);

    xTaskCreatePinnedToCore(
        Display_task, "Display",
        8192, nullptr, 1, nullptr, 1);
}

void loop()
{
}
