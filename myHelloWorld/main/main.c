#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/ledc.h"
#include "esp_timer.h"

#define LED GPIO_NUM_2

void app_main(void)
{

    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_8_BIT, // Resolução de 8 bits
        .freq_hz = 5000,                      // Frequência do PWM
        .speed_mode = LEDC_HIGH_SPEED_MODE,   // Timer de alta velocidade
        .timer_num = LEDC_TIMER_0             // Timer 0
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .channel = LEDC_CHANNEL_0,
        .duty = 0,       // Começar com o ciclo de trabalho em 0%
        .gpio_num = LED, // GPIO onde o PWM será gerado
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0};
    ledc_channel_config(&ledc_channel);

    int64_t start_time = 0;
    int64_t end_time = 0;

    while (1)
    {
        for (size_t i = ledc_channel.duty; i <= 255; i++)
        {
            start_time = esp_timer_get_time();
            ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, i);
            ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
            vTaskDelay(10 / portTICK_PERIOD_MS);
            end_time = esp_timer_get_time();
            printf("Iteração: %zu | Tempo da iteração: %lld us\n", i, (end_time - start_time));
        }
        for (size_t i = 255; i > 0; i--)
        { // Corrigido para iniciar de 1023 diretamente
            start_time = esp_timer_get_time();
            ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, i);
            ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
            vTaskDelay(10 / portTICK_PERIOD_MS);
            end_time = esp_timer_get_time(); // Correção: Medir end_time dentro do loop
            printf("Iteração: %zu | Tempo da iteração: %lld us\n", i, (end_time - start_time));
        }
    }
}
