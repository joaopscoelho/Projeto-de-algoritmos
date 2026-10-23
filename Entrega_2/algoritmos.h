#ifndef ALGORITMOS_H
#define ALGORITMOS_H

// ============================================================================
// algoritmos.h
// Contem: os quatro algoritmos de ordenacao (Insertion, Selection, Bubble e
// Shell Sort), as funcoes de manipulacao de arquivos (entrada, saida e
// tempo) e a criacao automatica da estrutura de pastas exigida pelo
// trabalho. As instancias (crescente/decrescente/random) ficam em
// crescente.h, decrescente.h e random.h.
//
// PORTABILIDADE: este codigo foi escrito para compilar em qualquer compilador
// C++ (C++98 em diante) e em qualquer sistema (Windows, Linux e macOS). Por
// isso NAO usa <filesystem>, std::to_string nem outros recursos de C++11/17:
// compiladores antigos de laboratorio (Dev-C++, Code::Blocks, MinGW antigo,
// Visual Studio com padrao C++14) nao os aceitam.
// ============================================================================

// O Visual Studio trata fopen() como "inseguro" e interrompe a compilacao;
// este define desliga esse aviso (nos demais compiladores nao tem efeito).
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <ctime>
#include <string>
#include <sstream>

// Funcoes do sistema operacional para criar pastas e descobrir a pasta atual.
#if defined(_WIN32)
    #include <direct.h>      // _mkdir, _getcwd
#else
    #include <sys/stat.h>    // mkdir
    #include <sys/types.h>
    #include <unistd.h>      // getcwd
#endif
using namespace std;

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

// Converte um inteiro em texto. Substitui std::to_string, que so existe a
// partir do C++11 e nao funciona em alguns MinGW antigos.
string inteiroParaTexto(int valor) {
    ostringstream saida;
    saida << valor;
    return saida.str();
}

// Retorna a pasta onde o programa esta rodando (os arquivos sao gravados
// a partir dela).
string pastaAtual() {
    char buffer[4096];
#if defined(_WIN32)
    char *resultado = _getcwd(buffer, sizeof(buffer));
#else
    char *resultado = getcwd(buffer, sizeof(buffer));
#endif
    if (resultado == NULL) return "(desconhecida)";
    return string(resultado);
}

// Cria UMA pasta. Retorna true se ela foi criada ou se ja existia.
bool criarPasta(const string &caminho) {
#if defined(_WIN32)
    int resultado = _mkdir(caminho.c_str());
#else
    int resultado = mkdir(caminho.c_str(), 0777);
#endif
    return resultado == 0 || errno == EEXIST;
}

// Cria uma pasta e todas as pastas "pai" que faltarem (equivalente a
// std::filesystem::create_directories, mas funciona em qualquer compilador).
// Usa "/" como separador, que o Windows tambem aceita.
// Retorna false se alguma pasta nao puder ser criada (ex.: sem permissao).
bool criarPastas(const string &caminho) {
    for (size_t i = 0; i <= caminho.size(); i++) {
        if (i == caminho.size() || caminho[i] == '/') {
            if (i > 0 && !criarPasta(caminho.substr(0, i))) return false;
        }
    }
    return true;
}

// Cria (se ainda nao existir) toda a arvore de pastas do algoritmo indicado:
//   <algoritmo>/Arquivos de Entrada/{Crescente,Decrescente,Random}
//   <algoritmo>/Arquivos de Saida/{Crescente,Decrescente,Random}
//   <algoritmo>/Arquivos de Tempo/{Crescente,Decrescente,Random}
// Nao gera erro caso as pastas ja existam. Retorna false (e explica o motivo
// na tela) se nao conseguir criar alguma pasta.
bool criarEstruturaPastas(const string &algoritmo) {
    const char *grupos[3] = {"Arquivos de Entrada", "Arquivos de Saida", "Arquivos de Tempo"};
    const char *categorias[3] = {"Crescente", "Decrescente", "Random"};

    for (int g = 0; g < 3; g++) {
        for (int c = 0; c < 3; c++) {
            string caminho = algoritmo + "/" + grupos[g] + "/" + categorias[c];
            if (!criarPastas(caminho)) {
                int erro = errno;
                printf("ERRO: nao foi possivel criar a pasta \"%s\" (%s).\n", caminho.c_str(), strerror(erro));
                printf("      Pasta atual: %s\n", pastaAtual().c_str());
                printf("      Execute o programa em uma pasta onde voce tenha permissao de escrita.\n");
                return false;
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// 4. Leitura/escrita dos arquivos de entrada, saida e tempo
// ---------------------------------------------------------------------------

// Abre um arquivo para escrita. Se nao conseguir, explica o motivo na tela
// e retorna NULL (em vez de deixar o programa travar).
FILE* abrirParaEscrita(const string &nomeArquivo) {
    FILE *arq = fopen(nomeArquivo.c_str(), "w");
    if (arq == NULL) {
        int erro = errno;
        printf("ERRO: nao foi possivel gravar o arquivo \"%s\" (%s).\n", nomeArquivo.c_str(), strerror(erro));
        printf("      Pasta atual: %s\n", pastaAtual().c_str());
        printf("      Execute o programa em uma pasta onde voce tenha permissao de escrita.\n");
    }
    return arq;
}

// Fecha o arquivo e confirma que tudo foi gravado (ex.: disco cheio).
bool fecharArquivo(FILE *arq, const string &nomeArquivo) {
    if (fclose(arq) != 0) {
        printf("ERRO: falha ao finalizar a gravacao do arquivo \"%s\".\n", nomeArquivo.c_str());
        return false;
    }
    return true;
}

// Salva o arquivo de entrada: primeira linha com o tamanho da instancia,
// depois um valor por linha. Retorna false se nao conseguir gravar.
// Caminho: <algoritmo>/Arquivos de Entrada/<Categoria>/Entrada<Categoria><Tamanho>.txt
bool salvarEntrada(const string &algoritmo, int tipo, int tamanho, int *vetor) {
    string categoria = nomeCategoria(tipo);
    string nomeArquivo = algoritmo + "/Arquivos de Entrada/" + categoria + "/Entrada" +
                          categoria + inteiroParaTexto(tamanho) + ".txt";

    FILE *arq = abrirParaEscrita(nomeArquivo);
    if (arq == NULL) return false;

    fprintf(arq, "%d\n", tamanho);
    for (int i = 0; i < tamanho; i++) {
        fprintf(arq, "%d\n", vetor[i]);
    }
    return fecharArquivo(arq, nomeArquivo);
}

// Salva o arquivo de saida: primeira linha com o tamanho da instancia,
// depois o vetor ja ordenado, um valor por linha. Retorna false se nao
// conseguir gravar.
// Caminho: <algoritmo>/Arquivos de Saida/<Categoria>/Saida<Categoria><Tamanho>.txt
bool salvarSaida(const string &algoritmo, int tipo, int tamanho, int *vetor) {
    string categoria = nomeCategoria(tipo);
    string nomeArquivo = algoritmo + "/Arquivos de Saida/" + categoria + "/Saida" +
                          categoria + inteiroParaTexto(tamanho) + ".txt";

    FILE *arq = abrirParaEscrita(nomeArquivo);
    if (arq == NULL) return false;

    fprintf(arq, "%d\n", tamanho);
    for (int i = 0; i < tamanho; i++) {
        fprintf(arq, "%d\n", vetor[i]);
    }
    return fecharArquivo(arq, nomeArquivo);
}

// Salva o arquivo de tempo: primeira linha com o tamanho da instancia,
// segunda linha com o tempo gasto (em segundos). Retorna false se nao
// conseguir gravar.
// Caminho: <algoritmo>/Arquivos de Tempo/<Categoria>/Tempo<Categoria><Tamanho>.txt
bool salvarTempo(const string &algoritmo, int tipo, int tamanho, double tempo) {
    string categoria = nomeCategoria(tipo);
    string nomeArquivo = algoritmo + "/Arquivos de Tempo/" + categoria + "/Tempo" +
                          categoria + inteiroParaTexto(tamanho) + ".txt";

    FILE *arq = abrirParaEscrita(nomeArquivo);
    if (arq == NULL) return false;

    fprintf(arq, "%d\n", tamanho);
    fprintf(arq, "%f\n", tempo);
    return fecharArquivo(arq, nomeArquivo);
}

#endif
