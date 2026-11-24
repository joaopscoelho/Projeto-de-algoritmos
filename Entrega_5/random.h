#ifndef RANDOM_H
#define RANDOM_H

#include <cstdlib>

// rand() so garante valores de 0 a 32767 e e exatamente isso que acontece no
// Windows (RAND_MAX = 32767), enquanto no Linux/macOS ele chega a ~2 bilhoes.
// Para a instancia aleatoria ter a mesma faixa de valores em qualquer
// maquina, juntamos duas chamadas de 15 bits em um numero de 30 bits
// (0 a 1.073.741.823) e reduzimos ao intervalo 0 a 999.999.999.
int numeroAleatorio() {
    int alto = rand() & 0x7FFF;
    int baixo = rand() & 0x7FFF;
    return ((alto << 15) | baixo) % 1000000000;
}

// Gera uma instancia com valores aleatorios (nao ordenada).
// O chamador e responsavel por liberar a memoria (delete[]).
int* gerarRandom(int tamanho) {
    int *vetor = new int[tamanho];
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = numeroAleatorio();
    }
    return vetor;
}

#endif
