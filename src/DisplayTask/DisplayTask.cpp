// src/DisplayTask/DisplayTask.cpp
#include "DisplayTask.h"
#include <U8g2lib.h>
#include "TimeData.h"

// Piny OLED dokładnie jak w Twoim działającym kodzie:
#define PIN_CLK 36  // SCK
#define PIN_MOSI 35 // MOSI (DIN)
#define PIN_RESET 40
#define PIN_DC 41
#define PIN_CS 42

// U8g2: SSD1306 128x64, 4-przewodowy SPI, software
U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(
    U8G2_R0,
    /* clock=*/PIN_CLK,
    /* data=*/PIN_MOSI,
    /* cs=*/PIN_CS,
    /* dc=*/PIN_DC,
    /* reset=*/PIN_RESET);

static const char *daysOfTheWeek[7] = {
    "Nie", "Pon", "Wto", "Śro", "Czw", "Pią", "Sob"};

void Display_task(void *parameter)
{
    u8g2.begin();

    TimeData local{};
    uint8_t lastSecond = 255; // niemożliwa wartość na start

    for (;;)
    {
        // pobierz dane czasu
        if (g_timeMutex && xSemaphoreTake(g_timeMutex, pdMS_TO_TICKS(20)) == pdTRUE)
        {
            local = g_timeData;
            xSemaphoreGive(g_timeMutex);
        }

        // ▶▶ Aktualizuj ekran TYLKO gdy zmieni się sekunda
        if (local.second != lastSecond)
        {
            lastSecond = local.second;

            u8g2.clearBuffer();

            // nagłówek
            u8g2.setFont(u8g2_font_ncenB08_tr);
            u8g2.setCursor(0, 10);
            u8g2.print(daysOfTheWeek[local.dow]);
            u8g2.print(' ');
            u8g2.print(local.day);
            u8g2.print('-');
            u8g2.print(local.month);
            u8g2.print('-');
            u8g2.print(local.year);

            // duża godzina
            u8g2.setFont(u8g2_font_fub20_tr);
            char timeBuf[9];
            snprintf(timeBuf, sizeof(timeBuf), "%02u:%02u:%02u",
                     local.hour, local.minute, local.second);
            u8g2.drawStr(0, 40, timeBuf);

            // temperatura
            u8g2.setFont(u8g2_font_ncenB08_tr);
            char infoBuf[16];
            snprintf(infoBuf, sizeof(infoBuf), "%2.1fC", local.temperature);
            u8g2.drawStr(0, 60, infoBuf);

            u8g2.sendBuffer();
        }

        // odświeżanie logiczne co 100ms (bez migania ekranu)
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
