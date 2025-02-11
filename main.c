#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define GPIO_PIN_SERVO       22     // Pino do servo
#define GPIO_PIN_LED         12     // Pino do led
#define PWM_DIVISER          125.0  // Divisor de clock para 50 Hz
#define WRAP_PERIOD          20000  // Período de 20 ms (50 Hz)
#define POSITION_SERVO_180   2400   // Posição de 180 graus
#define POSITION_SERVO_90    1470   // Posição de 90 graus
#define POSITION_SERVO_0     500    // Posição de 0 graus
#define TIME_DELAY_5000      5000   // Tempo de espera entre movimentos
#define TIME_DELAY_10        10     // Tempo de espera entre movimentos
#define STEP                 5      // Passo do movimento suave

uint slice; // Armazenar o slice do PWM

// Configuração do pino PWM
void init_pin_pwm_config(uint pin, float divider, uint16_t wrap) {

    gpio_set_function(pin, GPIO_FUNC_PWM);
    slice = pwm_gpio_to_slice_num(pin);
    pwm_set_clkdiv(slice, divider);
    pwm_set_wrap(slice, wrap);
    pwm_set_enabled(slice, true);
}

// Define o nível de PWM para um pino específico
void set_pwm_level(uint pin, uint16_t pulse_width) {
    uint level = (pulse_width * WRAP_PERIOD) / 20000;
    pwm_set_gpio_level(pin, level);
}

// Movimenta o servo suavemente entre dois ângulos
void move_smoothly(uint pin, uint16_t start, uint16_t end, int step, int delay) {
    if (start < end) {
        for (int i = start; i <= end; i += step) {
            set_pwm_level(pin, i);
            sleep_ms(delay);
        }
    } else {
        for (int i = start; i >= end; i -= step) {
            set_pwm_level(pin, i);
            sleep_ms(delay);
        }
    }
}

int main() {
    // Inicializa o sistema padrão de I/O
    stdio_init_all();

    // Inicializa o PWM no pino do servo
    init_pin_pwm_config(GPIO_PIN_SERVO, PWM_DIVISER, WRAP_PERIOD);

    // Inicializa o PWM no pino do LED
    init_pin_pwm_config(GPIO_PIN_LED, PWM_DIVISER, WRAP_PERIOD);

    // Move o servo para 180°
    pwm_set_gpio_level(GPIO_PIN_SERVO, (POSITION_SERVO_180 * WRAP_PERIOD) / 20000);
    sleep_ms(TIME_DELAY_5000);

    // Move o servo para 90°
    pwm_set_gpio_level(GPIO_PIN_SERVO, (POSITION_SERVO_90 * WRAP_PERIOD) / 20000);
    sleep_ms(TIME_DELAY_5000);

    // Move o servo para 0°
    pwm_set_gpio_level(GPIO_PIN_SERVO, (POSITION_SERVO_0 * WRAP_PERIOD) / 20000);
    sleep_ms(TIME_DELAY_5000);

    while (true) {
        // Movimenta o servo suavemente entre 0° e 180°
        // Pode ser modificado para adicionar o pino do led e fazer o led piscar
        move_smoothly(GPIO_PIN_SERVO, POSITION_SERVO_0, POSITION_SERVO_180, STEP, TIME_DELAY_10);
        move_smoothly(GPIO_PIN_SERVO, POSITION_SERVO_180, POSITION_SERVO_0, STEP, TIME_DELAY_10);
    }

    return 0;
}
