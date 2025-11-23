#define CONFIG_TCPIP_LWIP 1 // fix na ip6_addr_t

#include <Arduino.h>
#include "ButtonsTask/ButtonsTask.h"
#include "RTCTask/RTCTask.h"

void setup()
{
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    delay(50);

    // (Optional) Per-button override example:
    // ButtonPerConfig pc{};
    // pc.double_click_ms = 250; ///< Faster double-click just for this button.
    // btns.setPerConfig(ButtonIndex::TestButton, pc);

    xTaskCreatePinnedToCore(
        buttons_task, // Task function
        "buttons",    // Task name
        8192,         // Stack size
        NULL,         // Task input parameters
        1,            // Task priority, be carefull when changing this
        NULL,         // Task handle, add one if you want control over the task (resume or suspend the task)
        1             // Core to run the task on
    );

    xTaskCreatePinnedToCore(
        RTC_task, // Task function
        "RTC",    // Task name
        8192,     // Stack size
        NULL,     // Task input parameters
        1,        // Task priority, be carefull when changing this
        NULL,     // Task handle, add one if you want control over the task (resume or suspend the task)
        1         // Core to run the task on
    );
}

void loop()
{
}
