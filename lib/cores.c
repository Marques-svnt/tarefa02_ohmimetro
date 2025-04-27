#include <stdio.h>
#include <math.h>

#include "lib/pio.h"
#include "lib/ssd1306.h"
#include "lib/font.h"

// Função que normaliza o valor para o e24 (o 100 foi adicionado)
int normalizar_para_e24(int valor_norm) {
    const int e24[] = {10, 11, 12, 13, 15, 16, 18, 20, 22, 24, 27, 30,
                       33, 36, 39, 43, 47, 51, 56, 62, 68, 75, 82, 91, 100};

    int decada = 1;

    // Reduz para faixa 10–100
    while (valor_norm >= 100) {
        valor_norm /= 10;
        decada *= 10;
    }

    // Agora valor_norm está entre 10 e 99
    int melhor = e24[0];
    int menor_diferenca = abs(valor_norm - e24[0]);

    for (int i = 1; i < 25; i++) {
        int diferenca = abs(valor_norm - e24[i]);
        if (diferenca < menor_diferenca) {
            melhor = e24[i];
            menor_diferenca = diferenca;
        }
    }

    return melhor * decada;
}


// Função que converte a resistencia desconhecida nos digitos para a faixa de cores e no valor normalizado
void codigoCores(float R_x, int *digito1, int *digito2, int *indice_multiplicador, int *valor_norm) {
    //R_x = 100000; // Teste para os valores
    int multiplicador;
    int i;

    // Encontrar o multiplicador adequado
    for (i = -2; i <= 9; i++) {
        double valor = pow(10, i);
        double temp = R_x / valor;
        if (temp >= 10 && temp < 100) {
            multiplicador = i;
            break;
        }
    }

    // Obtem os numeros das faixas
    int valor_temp = (int)(R_x / pow(10, multiplicador) + 0.5);
    *digito1 = valor_temp / 10;
    *digito2 = valor_temp % 10;

    // Ajustar o índice do multiplicador para o array de cores
    if (multiplicador >= 0) {
        *indice_multiplicador = multiplicador;
    } else if (multiplicador == -1) {
        *indice_multiplicador = 10; // ouro
    } else if (multiplicador == -2) {
        *indice_multiplicador = 11; // prata
    }

    *valor_norm = normalizar_para_e24(((*digito1 * 10) + *digito2) * pow(10, multiplicador));// Calcula o valor normalizado

    set_one_led(*digito1, *digito2, multiplicador);  // Chama a função que vai colorir a matriz de leds
}