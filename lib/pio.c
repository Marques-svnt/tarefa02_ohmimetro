#include <stdio.h>
#include <stdlib.h>

// Bibliotecas de hardware do Raspberry Pi Pico
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"

// Headers do projeto
#include "pio.h"
#include "ws2812.pio.h"

/* Buffer para armazenar quais LEDs estão ligados matriz 5x5
(obs: devido a organização do diagrama, teremos que configurar as matrizes "espelhadas")*/
uint8_t led_buffer[NUM_PIXELS] = {
    // Matriz que forma as linhas de cor do resistor
    0, 0, 0, 0, 0, 
    0, 3, 3, 3, 0, 
    0, 0, 0, 0, 0, 
    0, 2, 2, 2, 0, 
    0, 1, 1, 1, 0,
};

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

// Definir as cores
static const Color colors[] = {
    {0, 0, 0},      // 0 - preto
    {20, 2, 0},     // 1 - marrom (praticamente laranja escuro)
    {20, 0, 0},     // 2 - vermelho
    {20, 5, 0},    // 3 - laranja
    {20, 10, 0},    // 4 - amarelo
    {0, 20, 0},     // 5 - verde
    {0, 0, 20},     // 6 - azul
    {10, 0, 20},    // 7 - violeta
    {10, 10, 10},   // 8 - cinza
    {20, 20, 20},   // 9 - branco
    {40, 40, 0},    // 10 - dourado (amarelo mais intenso)
    {40, 40, 40}    // 11 - prata (branco mais intenso)
};

void set_one_led(int digito1, int digito2, int multiplicador)
{
    uint32_t color1 = urgb_u32(colors[digito1].r, colors[digito1].g, colors[digito1].b);
    uint32_t color2 = urgb_u32(colors[digito2].r, colors[digito2].g, colors[digito2].b);
    uint32_t color3 = urgb_u32(colors[multiplicador].r, colors[multiplicador].g, colors[multiplicador].b);

    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (led_buffer[i] == 1)
        {
            put_pixel(color1);
        }
        else if (led_buffer[i] == 2)
        {
            put_pixel(color2);
        }
        else if (led_buffer[i] == 3)
        {
            put_pixel(color3);
        }
        else
        {
            put_pixel(0);
        }
    }
}


// Função que vai inicializar o PIO no microcontrolador
void initializePio()
{
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
}
