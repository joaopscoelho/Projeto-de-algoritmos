#ifndef CRESCENTE_H
#define CRESCENTE_H

// Gera uma instancia ja ordenada em ordem crescente: 0, 1, 2, ..., tamanho-1.
// O chamador e responsavel por liberar a memoria (delete[]).
int* gerarCrescente(int tamanho) {
    int *vetor = new int[tamanho];
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = i;
    }
    return vetor;
}

#endif
