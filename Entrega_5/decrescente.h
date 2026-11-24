#ifndef DECRESCENTE_H
#define DECRESCENTE_H

// Gera uma instancia em ordem decrescente: tamanho, tamanho-1, ..., 1.
// O chamador e responsavel por liberar a memoria (delete[]).
int* gerarDecrescente(int tamanho) {
    int *vetor = new int[tamanho];
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = tamanho - i;
    }
    return vetor;
}

#endif
