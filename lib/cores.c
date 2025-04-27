#include <stdio.h>
#include <math.h>

#include "lib/pio.h"
#include "lib/ssd1306.h"
#include "lib/font.h"

void codigoCores(float R_x, int *digito1, int *digito2, int *indice_multiplicador) {
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

    int valor_temp = (int)(R_x / pow(10, multiplicador) + 0.5);
    *digito1 = valor_temp / 10;
    *digito2 = valor_temp % 10;

    // Ajustar o índice do multiplicador para o array
    if (multiplicador >= 0) {
        *indice_multiplicador = multiplicador;
    } else if (multiplicador == -1) {
        *indice_multiplicador = 10; // ouro
    } else if (multiplicador == -2) {
        *indice_multiplicador = 11; // prata
    }

    set_one_led(*digito1, *digito2, multiplicador);  // Chama a função com os novos valores
}