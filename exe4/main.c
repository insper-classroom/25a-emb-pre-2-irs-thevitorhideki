#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;
const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile int red_pressed = 0;
volatile int green_released = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4 && gpio == BTN_PIN_R) { // fall edge
        red_pressed = 1;
    } else if (events == 0x08 && gpio == BTN_PIN_G) {
        green_released = 1;
    }
}

int main() {
    int led_r = 0;
    int led_g = 0;

    stdio_init_all();

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);

    // callback led r (first)
    gpio_set_irq_enabled_with_callback(
        BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    // callback led g (nao usar _with_callback)
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_RISE, true);

    while (true) {
        if (red_pressed) {
            led_r = !led_r;
            gpio_put(LED_PIN_R, led_r);
            red_pressed = 0;
        } else if (green_released) {
            led_g = !led_g;
            gpio_put(LED_PIN_G, led_g);
            green_released = 0;
        }
    }
}
