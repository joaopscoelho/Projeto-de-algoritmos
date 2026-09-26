#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <cstdio>
#include <string>
using namespace std;

// Retorna o nome da categoria do tipo de instancia (usado em pastas e
// nomes de arquivo), seguindo a arvore de arquivos do trabalho:
// Crescente, Decrescente ou Random.
string nomeCategoria(int tipo) {
    switch (tipo) {
        case 1: return "Crescente";
        case 2: return "Decrescente";
        case 3: return "Random";
    }
    return "";
}

// Salva o arquivo de entrada: primeira linha com o tamanho da instancia,
// depois um valor por linha.
// Caminho: Arquivos de Entrada/<Categoria>/Entrada<Categoria><Tamanho>.txt
void salvarEntrada(int tipo, int tamanho, int *vetor) {
    string categoria = nomeCategoria(tipo);
    string nomeArquivo = "Arquivos de Entrada/" + categoria + "/Entrada" +
                          categoria + to_string(tamanho) + ".txt";

    FILE *arq = fopen(nomeArquivo.c_str(), "w");
    fprintf(arq, "%d\n", tamanho);
    for (int i = 0; i < tamanho; i++) {
        fprintf(arq, "%d\n", vetor[i]);
    }
    fclose(arq);
}

// Salva o arquivo de saida: primeira linha com o tamanho da instancia,
// depois o vetor ja ordenado, um valor por linha.
// Caminho: Arquivos de Saida/<Categoria>/Saida<Categoria><Tamanho>.txt
void salvarSaida(int tipo, int tamanho, int *vetor) {
    string categoria = nomeCategoria(tipo);
    string nomeArquivo = "Arquivos de Saida/" + categoria + "/Saida" +
                          categoria + to_string(tamanho) + ".txt";

    FILE *arq = fopen(nomeArquivo.c_str(), "w");
    fprintf(arq, "%d\n", tamanho);
    for (int i = 0; i < tamanho; i++) {
        fprintf(arq, "%d\n", vetor[i]);
    }
    fclose(arq);
}

// Salva o arquivo de tempo: primeira linha com o tamanho da instancia,
// segunda linha com o tempo gasto (em segundos), conforme o enunciado.
// Caminho: Arquivos de Tempo/<Categoria>/Tempo<Categoria><Tamanho>.txt
void salvarTempo(int tipo, int tamanho, double tempo) {
    string categoria = nomeCategoria(tipo);
    string nomeArquivo = "Arquivos de Tempo/" + categoria + "/Tempo" +
                          categoria + to_string(tamanho) + ".txt";

    FILE *arq = fopen(nomeArquivo.c_str(), "w");
    fprintf(arq, "%d\n", tamanho);
    fprintf(arq, "%f\n", tempo);
    fclose(arq);
}

#endif
