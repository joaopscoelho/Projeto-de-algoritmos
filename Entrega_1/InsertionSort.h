#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

// Implementacao tradicional do Insertion Sort (versao apresentada em aula).
// Ordena o vetor "vetor" de "tamanho" elementos em ordem crescente.
void InsertionSort_versao1(int *vetor, int tamanho) {
    int chave, j;

    for (int i = 1; i < tamanho; i++) {
        chave = vetor[i];   // elemento a ser inserido na posicao correta
        j = i - 1;

        // desloca os elementos maiores que a chave uma posicao a frente
        while (j >= 0 && vetor[j] > chave) {
            vetor[j + 1] = vetor[j];
            j--;
        }

        vetor[j + 1] = chave; // insere a chave na posicao correta
    }
}

#endif
