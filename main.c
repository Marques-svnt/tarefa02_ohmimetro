/*
 * Por: Wilton Lacerda Silva
 *    Ohmímetro utilizando o ADC da BitDogLab
 *
 *
 * Neste exemplo, utilizamos o ADC do RP2040 para medir a resistência de um resistor
 * desconhecido, utilizando um divisor de tensão com dois resistores.
 * O resistor conhecido é de 10k ohm e o desconhecido é o que queremos medir.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"
#include "lib/font.h"
#include "lib/pio.h"
#include "lib/cores.h"
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define ADC_PIN 28 // GPIO para o voltímetro
#define Botao_A 5  // GPIO para botão A

int R_conhecido = 10000;   // Resistor de 10k ohm
float R_x = 0.0;           // Resistor desconhecido
float ADC_VREF = 3.31;     // Tensão de referência do ADC
int ADC_RESOLUTION = 4095; // Resolução do ADC (12 bits)
float tensao;
char str_x[5]; // Buffer para armazenar a string
char str_y[5]; // Buffer para armazenar a string
char resist_norm[5]; // Buffer para armazenar a string
int digito1, digito2, indice_multiplicador, valor_norm; // Declaração das variaveis do codigo de cores

// Mapeamento de valores para cores
const char *cores[] = {
  "Preto", "Marrom", "Vermelho", "Laranja", "Amarelo",
  "Verde", "Azul", "Violeta", "Cinza", "Branco"
};

const char *multiplicadores[] = {
  "Preto", "Marrom", "Vermelho", "Laranja", "Amarelo",
  "Verde", "Azul", "Violeta", "Cinza", "Branco",
  "Ouro", "Prata"
};

int main()
{
  initializePio(); // Inicializar a matriz de leds
  stdio_init_all();

  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA);                                        // Pull up the data line
  gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
  ssd1306_t ssd;                                                // Inicializa a estrutura do display
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd);                                         // Configura o display
  ssd1306_send_data(&ssd);                                      // Envia os dados para o display

  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  adc_init();
  adc_gpio_init(ADC_PIN); // GPIO 28 como entrada analógica


  bool cor = true;
  while (true)
  {
    adc_select_input(2); // Seleciona o ADC para eixo X. O pino 28 como entrada analógica

    float soma = 0.0f;
    for (int i = 0; i < 500; i++)
    {
      soma += adc_read();
      sleep_ms(1);
    }
    float media = soma / 500.0f;

    // Fórmula simplificada: R_x = R_conhecido * ADC_encontrado /(ADC_RESOLUTION - adc_encontrado)
    R_x = (R_conhecido * media) / (ADC_RESOLUTION - media);

    codigoCores(R_x, &digito1, &digito2, &indice_multiplicador, &valor_norm); // Chama a função que vai retornar os digitos para encontrar as cores dos vetores e o valor normalizado

    // Converte os valores em float e int para strings para usar no display
    sprintf(resist_norm, "%d", valor_norm);
    sprintf(str_x, "%1.0f", media); 
    sprintf(str_y, "%1.0f", R_x); 

    //  Atualiza o conteúdo do display com animações
    ssd1306_fill(&ssd, !cor);                          // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);      // Desenha um retângulo
    ssd1306_line(&ssd, 3, 37, 123, 37, cor);           // Desenha uma linha
    ssd1306_draw_string(&ssd, cores[digito1], 8, 6); // Desenha uma string
    ssd1306_draw_string(&ssd, cores[digito2], 8, 16);  // Desenha uma string
    ssd1306_draw_string(&ssd, multiplicadores[indice_multiplicador], 8, 28);  // Desenha uma string
    ssd1306_draw_string(&ssd, "E24", 72, 6);          // Desenha uma string
    ssd1306_draw_string(&ssd, resist_norm, 72, 16);  // Desenha uma string
    ssd1306_draw_string(&ssd, "ADC", 13, 41);          // Desenha uma string
    ssd1306_draw_string(&ssd, "Resisten.", 50, 41);    // Desenha uma string
    ssd1306_line(&ssd, 44, 37, 44, 60, cor);           // Desenha uma linha vertical
    ssd1306_line(&ssd, 70, 3, 70, 37, cor);           // Desenha uma linha vertical
    ssd1306_draw_string(&ssd, str_x, 8, 52);           // Desenha uma string
    ssd1306_draw_string(&ssd, str_y, 59, 52);          // Desenha uma string
    ssd1306_send_data(&ssd);                           // Atualiza o display*/
    sleep_ms(700);
  }
}