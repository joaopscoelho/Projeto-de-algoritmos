#ifndef INSTANCIAS_H
#define INSTANCIAS_H

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
            vetor[i] = numeroAleatorio();
        }
    }

    return vetor;
}

#endif
