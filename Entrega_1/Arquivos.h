#ifndef ARQUIVOS_H
#define ARQUIVOS_H

// O Visual Studio trata fopen() como "inseguro" e interrompe a compilacao;
// este define desliga esse aviso (nos demais compiladores nao tem efeito).
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <string>
#include <sstream>
using namespace std;

// Converte um inteiro em texto. Substitui std::to_string, que so existe a
// partir do C++11 e nao funciona em alguns MinGW antigos (Dev-C++, etc.).
string inteiroParaTexto(int valor) {
    ostringstream saida;
    saida << valor;
    return saida.str();
}

// Abre um arquivo para escrita. Se nao conseguir (por exemplo, porque a
// pasta nao existe), explica o motivo na tela e retorna NULL, em vez de
// deixar o programa travar.
FILE* abrirParaEscrita(const string &nomeArquivo) {
    FILE *arq = fopen(nomeArquivo.c_str(), "w");
    if (arq == NULL) {
        printf("ERRO: nao foi possivel gravar o arquivo \"%s\" (%s).\n", nomeArquivo.c_str(), strerror(errno));
        printf("      Execute o programa na pasta que contem \"Arquivos de Entrada\", \"Arquivos de Saida\" e \"Arquivos de Tempo\".\n");
    }
    return arq;
}

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
bool salvarEntrada(int tipo, int tamanho, int *vetor) {
    string categoria = nomeCategoria(tipo);
    string nomeArquivo = "Arquivos de Entrada/" + categoria + "/Entrada" +
                          categoria + inteiroParaTexto(tamanho) + ".txt";

    FILE *arq = abrirParaEscrita(nomeArquivo);
    if (arq == NULL) return false;

    fprintf(arq, "%d\n", tamanho);
    for (int i = 0; i < tamanho; i++) {
        fprintf(arq, "%d\n", vetor[i]);
    }
    return fclose(arq) == 0;
}

// Salva o arquivo de saida: primeira linha com o tamanho da instancia,
// depois o vetor ja ordenado, um valor por linha.
// Caminho: Arquivos de Saida/<Categoria>/Saida<Categoria><Tamanho>.txt
bool salvarSaida(int tipo, int tamanho, int *vetor) {
    string categoria = nomeCategoria(tipo);
    string nomeArquivo = "Arquivos de Saida/" + categoria + "/Saida" +
                          categoria + inteiroParaTexto(tamanho) + ".txt";

    FILE *arq = abrirParaEscrita(nomeArquivo);
    if (arq == NULL) return false;

    fprintf(arq, "%d\n", tamanho);
    for (int i = 0; i < tamanho; i++) {
        fprintf(arq, "%d\n", vetor[i]);
    }
    return fclose(arq) == 0;
}

// Salva o arquivo de tempo: primeira linha com o tamanho da instancia,
// segunda linha com o tempo gasto (em segundos), conforme o enunciado.
// Caminho: Arquivos de Tempo/<Categoria>/Tempo<Categoria><Tamanho>.txt
bool salvarTempo(int tipo, int tamanho, double tempo) {
    string categoria = nomeCategoria(tipo);
    string nomeArquivo = "Arquivos de Tempo/" + categoria + "/Tempo" +
                          categoria + inteiroParaTexto(tamanho) + ".txt";

    FILE *arq = abrirParaEscrita(nomeArquivo);
    if (arq == NULL) return false;

    fprintf(arq, "%d\n", tamanho);
    fprintf(arq, "%f\n", tempo);
    return fclose(arq) == 0;
}

#endif
