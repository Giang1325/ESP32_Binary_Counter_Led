#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define GPIO_OUT_W1TS_REG (*(volatile uint32_t *)0x3FF44008)
#define GPIO_OUT_W1TC_REG (*(volatile uint32_t *)0x3FF4400C)
#define GPIO_ENABLE_REG (*(volatile uint32_t *)0x3FF44020)

#define LED_GPIO_NUM 18

#define GPIO_SET_BIT(bit) (GPIO_OUT_W1TS_REG = (1 << (bit)))
#define GPIO_CLEAR_BIT(bit) (GPIO_OUT_W1TC_REG = (1 << (bit)))

#define LED_COUNT 6
#define TIME_DELAY_0 500
#define TIME_DELAY_1 200
#define TIME_DELAY_2 100
#define TIME_DELAY_4 1000

void app_main(void)
{
    int led_pins[LED_COUNT] = {23, 21, 19, 18, 4, 2};

    for (int i = 0; i < LED_COUNT; i++) {
        GPIO_ENABLE_REG |= (1 << led_pins[i]);
    }

    int counter = 0;

    while (1) {
        for (int i = 0; i < LED_COUNT; i++) {
            if ((counter >> i) & 1) {
                GPIO_SET_BIT(led_pins[i]);      // bit = 1 → bật LED
            } else {
                GPIO_CLEAR_BIT(led_pins[i]);    // bit = 0 → PHẢI tắt LED (không được bỏ qua)
            }
        }

        vTaskDelay(pdMS_TO_TICKS(TIME_DELAY_0));   // đợi để mắt nhìn kịp mỗi số

        counter++;
        counter %= 64;   // 6 LED → 2^6 = 64 giá trị (0 đến 63), quay vòng giống cách bạn đã làm với index
    }
}
