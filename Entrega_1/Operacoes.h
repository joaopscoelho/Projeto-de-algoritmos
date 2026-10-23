#ifndef OPERACOES_H
#define OPERACOES_H

#include <cstdio>
#include <ctime>
#include "Instancias.h"
#include "Arquivos.h"
#include "InsertionSort.h"

// Gera a instancia do tipo/tamanho indicados, executa o Insertion Sort
// medindo o tempo gasto (clock/CLOCKS_PER_SEC, padrao apresentado em aula)
// e salva os arquivos de entrada, tempo e saida correspondentes.
// Retorna false se algum arquivo nao puder ser gravado.
bool operacoes(int tipo, int tamanho) {
    clock_t start_t, end_t;
    double tempoGasto;

    int *vetor = gerarSequencia(tipo, tamanho);

    if (!salvarEntrada(tipo, tamanho, vetor)) {
        delete[] vetor;
        return false;
    }

    start_t = clock();
    InsertionSort_versao1(vetor, tamanho);
    end_t = clock();

    tempoGasto = (end_t - start_t) / (double)CLOCKS_PER_SEC;

    bool gravou = salvarTempo(tipo, tamanho, tempoGasto) &&
                  salvarSaida(tipo, tamanho, vetor);

    delete[] vetor;
    return gravou;
}

// Executa o Insertion Sort para um tipo de instancia, em todos os
// tamanhos exigidos pelo trabalho (10, 100, 1.000, 10.000, 100.000 e
// 1.000.000).
// Retorna false (e para) se algum arquivo nao puder ser gravado.
bool executarTodosOsTamanhos(int tipo) {
    int tamanhos[] = {10, 100, 1000, 10000, 100000, 1000000};

    for (int i = 0; i < 6; i++) {
        printf("  Executando tamanho %d (%s)...\n", tamanhos[i], nomeCategoria(tipo).c_str());
        if (!operacoes(tipo, tamanhos[i])) return false;
    }
    return true;
}

#endif
