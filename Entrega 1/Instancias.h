#ifndef INSTANCIAS_H
#define INSTANCIAS_H

#include <cstdlib>

// Tipo de instancia:
// 1 - Crescente
// 2 - Decrescente
// 3 - Random

// Gera um vetor de "tamanho" elementos de acordo com o tipo de instancia.
// O chamador e responsavel por liberar a memoria (delete[]).
int* gerarSequencia(int tipo, int tamanho) {
    int *vetor = new int[tamanho];

    if (tipo == 1) { // Crescente: 0, 1, 2, ..., tamanho-1
        for (int i = 0; i < tamanho; i++) {
            vetor[i] = i;
        }
    } else if (tipo == 2) { // Decrescente: tamanho, tamanho-1, ..., 1
        for (int i = 0; i < tamanho; i++) {
            vetor[i] = tamanho - i;
        }
    } else if (tipo == 3) { // Random: valores aleatorios
        for (int i = 0; i < tamanho; i++) {
            vetor[i] = rand() % 1000000000;
        }
    }

    return vetor;
}

#endif
