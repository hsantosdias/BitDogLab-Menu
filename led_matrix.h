#ifndef LED_MATRIX_H
#define LED_MATRIX_H


//codigo base https://github.com/hsantosdias/BitDogLab-IRQ-WS2812
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "hardware/timer.h"
#include "hardware/sync.h"
#include "ws2812b.pio.h"

#define MATRIX_LED_PIN 7
#define LED_COUNT 25
#define ROWS 5
#define COLS 5

// Estrutura do LED GRB
typedef struct {
    uint8_t G, R, B;
} pixel_t;

typedef pixel_t npLED_t;

void led_matrix_init(void);
void led_matrix_clear(void);
void led_matrix_write(void);
void led_matrix_set_pixel(uint index, uint8_t r, uint8_t g, uint8_t b);
void led_matrix_display_number(int number);

#endif // LED_MATRIX_H
