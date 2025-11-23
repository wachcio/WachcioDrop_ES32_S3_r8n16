// Explicit button mapping (compile-time). MUST be BEFORE <Universal_Button> header include.
#define BUTTON_LIST(X) \
    X(TestButton, 4) ///< INPUT_PULLUP (pressed == LOW).

#include <Arduino.h>
#include <Universal_Button.h>

/**
 * Custom timings (ms): debounce, short, long, double-click-gap.
 * - double_click_ms: max gap BETWEEN two short presses to count as a Double event.
 */
constexpr ButtonTimingConfig kTiming{
    30,   ///< Debounce_ms: filter bounce / noisy edges.
    100,  ///< Short_press_ms: >= 100ms counts as short (quick tap).
    1000, ///< Long_press_ms:  >= 1500ms counts as long.
    400   ///< Double_click_ms: second short press within 400ms -> Double.
};

// Create handler sized to NUM_BUTTONS from mapping above, with custom timing.
static Button btns = makeButtons(kTiming);

uint8_t LED = 14;

void buttons_task(void *parameter)
{

    for (;;)
    {
        btns.update();

        // Get-and-consume the event for TestButton.
        const ButtonPressType evt = btns.getPressType(ButtonIndex::TestButton);

        switch (evt)
        {
        case ButtonPressType::Short:
            // Note: Short presses are deferred briefly to allow for a possible double click.
            // If no second press arrives within double_click_ms, this fires.
            digitalWrite(LED, !digitalRead(LED));
            Serial.println("Short press detected!");
            break;

        case ButtonPressType::Double:
            // Two short presses with gap <= double_click_ms.
            Serial.println("Double-click detected!");
            break;

        case ButtonPressType::Long:
            for (uint8_t i = 0; i <= 9; i++)
            {
                digitalWrite(LED, !digitalRead(LED));
                vTaskDelay(50 / portTICK_PERIOD_MS);
            }

            Serial.println("Long press detected!");
            break;

        default:
            // No event this iteration.
            break;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
