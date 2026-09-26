#ifndef BASE_H
#define BASE_H

// ============================================================================
// base.h
// Contem: os quatro algoritmos de ordenacao (Insertion, Selection, Bubble e
// Shell Sort), as funcoes de manipulacao de arquivos (entrada, saida e
// tempo) e a criacao automatica da estrutura de pastas exigida pelo
// trabalho. As instancias (crescente/decrescente/random) ficam em
// crescente.h, decrescente.h e random.h.
// ============================================================================

#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

// ---------------------------------------------------------------------------
// 1. Algoritmos de ordenacao
// ---------------------------------------------------------------------------

// Insertion Sort (versao tradicional apresentada em aula).
// Ordena o vetor "vetor" de "tamanho" elementos em ordem crescente.
void InsertionSort(int *vetor, int tamanho) {
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

// Selection Sort (versao tradicional apresentada em aula).
// A cada passo i, procura o menor elemento entre as posicoes [i, tamanho-1]
// e o troca de lugar com o elemento da posicao i.
void SelectionSort(int *vetor, int tamanho) {
    int menor, aux;

    for (int i = 0; i < tamanho - 1; i++) {
        menor = i; // posicao do menor elemento encontrado ate agora

        for (int j = i + 1; j < tamanho; j++) {
            if (vetor[j] < vetor[menor]) {
                menor = j;
            }
        }

        if (menor != i) { // so troca se de fato encontrou um menor
            aux = vetor[i];
            vetor[i] = vetor[menor];
            vetor[menor] = aux;
        }
    }
}

// Bubble Sort (versao otimizada com "flag" de parada antecipada).
// A cada passada, os elementos adjacentes fora de ordem sao trocados,
// "borbulhando" o maior elemento restante ate o final do vetor. Se uma
// passada inteira nao realizar nenhuma troca, o vetor ja esta ordenado e o
// algoritmo para (isso e o que torna o caso crescente O(n)).
void BubbleSort(int *vetor, int tamanho) {
    int aux;
    bool trocou;

    for (int i = 0; i < tamanho - 1; i++) {
        trocou = false;

        for (int j = 0; j < tamanho - 1 - i; j++) {
            if (vetor[j] > vetor[j + 1]) {
                aux = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = aux;
                trocou = true;
            }
        }

        if (!trocou) break; // nenhuma troca: o vetor ja esta ordenado
    }
}

// Shell Sort (sequencia de intervalos original de Shell: tamanho/2, /4, ...,
// 1). Para cada intervalo (gap), aplica-se, de forma intercalada, a logica
// do Insertion Sort aos subvetores formados por elementos espacados de
// "gap" posicoes, reduzindo o intervalo pela metade a cada passada ate
// chegar a 1 (passada final equivalente a um Insertion Sort completo, porem
// sobre um vetor ja quase ordenado).
void ShellSort(int *vetor, int tamanho) {
    int chave, j;

    for (int gap = tamanho / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < tamanho; i++) {
            chave = vetor[i];
            j = i - gap;

            while (j >= 0 && vetor[j] > chave) {
                vetor[j + gap] = vetor[j];
                j -= gap;
            }

            vetor[j + gap] = chave;
        }
    }
}

// ---------------------------------------------------------------------------
// 2. Utilitarios de nomenclatura
// ---------------------------------------------------------------------------

// Tipo de instancia:
// 1 - Crescente
// 2 - Decrescente
// 3 - Random
string nomeCategoria(int tipo) {
    switch (tipo) {
        case 1: return "Crescente";
        case 2: return "Decrescente";
        case 3: return "Random";
    }
    return "";
}

// ---------------------------------------------------------------------------
// 3. Criacao automatica da estrutura de pastas
// ---------------------------------------------------------------------------

// Cria (se ainda nao existir) toda a arvore de pastas do algoritmo indicado:
//   <algoritmo>/Arquivos de Entrada/{Crescente,Decrescente,Random}
//   <algoritmo>/Arquivos de Saida/{Crescente,Decrescente,Random}
//   <algoritmo>/Arquivos de Tempo/{Crescente,Decrescente,Random}
// fs::create_directories nao gera erro caso as pastas ja existam.
void criarEstruturaPastas(const string &algoritmo) {
    const string grupos[3] = {"Arquivos de Entrada", "Arquivos de Saida", "Arquivos de Tempo"};
    const string categorias[3] = {"Crescente", "Decrescente", "Random"};

    for (const string &grupo : grupos) {
        for (const string &categoria : categorias) {
            fs::create_directories(algoritmo + "/" + grupo + "/" + categoria);
        }
    }
}

// ---------------------------------------------------------------------------
// 4. Leitura/escrita dos arquivos de entrada, saida e tempo
// ---------------------------------------------------------------------------

// Salva o arquivo de entrada: primeira linha com o tamanho da instancia,
// depois um valor por linha.
// Caminho: <algoritmo>/Arquivos de Entrada/<Categoria>/Entrada<Categoria><Tamanho>.txt
void salvarEntrada(const string &algoritmo, int tipo, int tamanho, int *vetor) {
    string categoria = nomeCategoria(tipo);
    string nomeArquivo = algoritmo + "/Arquivos de Entrada/" + categoria + "/Entrada" +
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
// Caminho: <algoritmo>/Arquivos de Saida/<Categoria>/Saida<Categoria><Tamanho>.txt
void salvarSaida(const string &algoritmo, int tipo, int tamanho, int *vetor) {
    string categoria = nomeCategoria(tipo);
    string nomeArquivo = algoritmo + "/Arquivos de Saida/" + categoria + "/Saida" +
                          categoria + to_string(tamanho) + ".txt";

    FILE *arq = fopen(nomeArquivo.c_str(), "w");
    fprintf(arq, "%d\n", tamanho);
    for (int i = 0; i < tamanho; i++) {
        fprintf(arq, "%d\n", vetor[i]);
    }
    fclose(arq);
}

// Salva o arquivo de tempo: primeira linha com o tamanho da instancia,
// segunda linha com o tempo gasto (em segundos).
// Caminho: <algoritmo>/Arquivos de Tempo/<Categoria>/Tempo<Categoria><Tamanho>.txt
void salvarTempo(const string &algoritmo, int tipo, int tamanho, double tempo) {
    string categoria = nomeCategoria(tipo);
    string nomeArquivo = algoritmo + "/Arquivos de Tempo/" + categoria + "/Tempo" +
                          categoria + to_string(tamanho) + ".txt";

    FILE *arq = fopen(nomeArquivo.c_str(), "w");
    fprintf(arq, "%d\n", tamanho);
    fprintf(arq, "%f\n", tempo);
    fclose(arq);
}

#endif
