#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>

// Gera uma instancia com valores aleatorios (nao ordenada).
// O chamador e responsavel por liberar a memoria (delete[]).
int* gerarRandom(int tamanho) {
    int *vetor = new int[tamanho];
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = rand() % 1000000000;
    }
    return vetor;
}

#endif
