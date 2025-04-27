#include <stdio.h>
#include <math.h>

#include "lib/pio.h"

// Mapeamento de valores para cores
const char *cores[] = {
    "preto", "marrom", "vermelho", "laranja", "amarelo",
    "verde", "azul", "violeta", "cinza", "branco"
};

const char *multiplicadores[] = {
    "preto", "marrom", "vermelho", "laranja", "amarelo",
    "verde", "azul", "violeta", "cinza", "branco",
    "ouro", "prata"
};

int codigoCores(float R_x) {
    R_x = 500;
    int digito1, digito2, multiplicador;
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
    digito1 = valor_temp / 10;
    digito2 = valor_temp % 10;

    // Ajustar o índice do multiplicador para o array
    int indice_multiplicador;
    if (multiplicador >= 0) {
        indice_multiplicador = multiplicador;
    } else if (multiplicador == -1) {
        indice_multiplicador = 10; // ouro
    } else if (multiplicador == -2) {
        indice_multiplicador = 11; // prata
    }

    printf("Códigos de cores para %.2f ohms:\n", R_x);
    printf("1ª faixa: %s\n", cores[digito1]);
    printf("2ª faixa: %s\n", cores[digito2]);
    printf("3ª faixa (multiplicador): %s\n", multiplicadores[indice_multiplicador]);
    printf("4ª faixa (tolerância): dourado (±5%%)\n");

    return 0;
}