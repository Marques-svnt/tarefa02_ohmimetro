#ifndef PIO_H
#define PIO_H

//Declaração das funções do PIO
void set_one_led(int digito1, int digito2, int multiplicador);
void initializePio();

// Definições da matriz de led
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define IS_RGBW false

#endif