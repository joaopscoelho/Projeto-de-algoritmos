// O Visual Studio trata fopen() como "inseguro" e interrompe a compilacao;
// este define precisa vir ANTES de qualquer #include (nos demais compiladores
// nao tem efeito).
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include "algoritmos.h"
#include "crescente.h"
#include "decrescente.h"
#include "random.h"

using namespace std;

typedef void (*FuncaoOrdenacao)(int*, int);

// Os 4 algoritmos do trabalho, na ordem sugerida pelo enunciado.
const string NOMES_ALGORITMOS[4] = {"Insertion Sort", "Selection Sort", "Shell Sort", "Bubble Sort"};
const FuncaoOrdenacao FUNCOES_ALGORITMOS[4] = {
    InsertionSort,
    SelectionSort,
    ShellSort,
    BubbleSort
};

const int TAMANHOS[6] = {10, 100, 1000, 10000, 100000, 1000000};

// Le uma opcao digitada no menu (uma linha inteira).
//  - Se o texto nao for um numero, retorna -1 (o menu mostra "Opcao invalida").
//  - Se a entrada acabar (Ctrl+D / Ctrl+Z ou fim do arquivo redirecionado),
//    retorna 0 (Voltar/Sair), para o programa nunca ficar preso num laco.
int lerOpcao() {
    string linha;
    if (!getline(cin, linha)) return 0;

    char *fim;
    long valor = strtol(linha.c_str(), &fim, 10);
    if (fim == linha.c_str()) return -1; // nao havia nenhum numero
    return (int) valor;
}

// Gera a instancia do tipo indicado (1-Crescente, 2-Decrescente, 3-Random).
int* gerarSequencia(int tipo, int tamanho) {
    if (tipo == 1) return gerarCrescente(tamanho);
    if (tipo == 2) return gerarDecrescente(tamanho);
    return gerarRandom(tamanho);
}

// Gera a instancia, executa o algoritmo de ordenacao indicado medindo o
// tempo gasto (clock()/CLOCKS_PER_SEC) e salva os arquivos de entrada,
// tempo e saida correspondentes dentro da pasta do algoritmo.
// Retorna false se alguma pasta/arquivo nao puder ser criado.
bool operacoes(const string &algoritmo, FuncaoOrdenacao funcao, int tipo, int tamanho) {
    if (!criarEstruturaPastas(algoritmo)) return false; // pastas criadas pelo codigo

    clock_t start_t, end_t;
    double tempoGasto;

    int *vetor = gerarSequencia(tipo, tamanho);

    if (!salvarEntrada(algoritmo, tipo, tamanho, vetor)) {
        delete[] vetor;
        return false;
    }

    start_t = clock();
    funcao(vetor, tamanho);
    end_t = clock();

    tempoGasto = (end_t - start_t) / (double)CLOCKS_PER_SEC;

    bool gravou = salvarTempo(algoritmo, tipo, tamanho, tempoGasto) &&
                  salvarSaida(algoritmo, tipo, tamanho, vetor);

    delete[] vetor;

    if (gravou) {
        printf("  [%s] tamanho=%d tipo=%s tempo=%f s\n", algoritmo.c_str(), tamanho, nomeCategoria(tipo).c_str(), tempoGasto);
    }
    return gravou;
}

// Executa um algoritmo para um tipo de instancia, em todos os tamanhos
// exigidos pelo trabalho (10, 100, 1.000, 10.000, 100.000 e 1.000.000).
bool executarTodosOsTamanhos(const string &algoritmo, FuncaoOrdenacao funcao, int tipo) {
    for (int i = 0; i < 6; i++) {
        if (!operacoes(algoritmo, funcao, tipo, TAMANHOS[i])) return false;
    }
    return true;
}

// Executa um algoritmo para todos os tipos de instancia (Crescente,
// Decrescente e Random), em todos os tamanhos.
bool executarTodosOsTipos(const string &algoritmo, FuncaoOrdenacao funcao) {
    for (int tipo = 1; tipo <= 3; tipo++) {
        if (!executarTodosOsTamanhos(algoritmo, funcao, tipo)) return false;
    }
    return true;
}

// Executa os 4 algoritmos, para os 3 tipos e os 6 tamanhos: gera toda a
// estrutura de pastas e arquivos do trabalho em uma unica chamada.
bool executarTudo() {
    for (int a = 0; a < 4; a++) {
        printf("\n===== Executando %s =====\n", NOMES_ALGORITMOS[a].c_str());
        if (!executarTodosOsTipos(NOMES_ALGORITMOS[a], FUNCOES_ALGORITMOS[a])) return false;
    }
    return true;
}

int menuTipo() {
    cout << endl << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "=-=-=  Tipo de Entradas  =-=-=" << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "1 => Crescente" << endl;
    cout << "2 => Decrescente" << endl;
    cout << "3 => Randomico" << endl;
    cout << "4 => Todos os tipos" << endl;
    cout << "0 => Voltar" << endl;
    cout << "Escolha uma opcao: ";
    return lerOpcao();
}

int menuTamanho() {
    cout << endl << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "=-=-=      Tamanhos      =-=-=" << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "1 => 10" << endl;
    cout << "2 => 100" << endl;
    cout << "3 => 1.000" << endl;
    cout << "4 => 10.000" << endl;
    cout << "5 => 100.000" << endl;
    cout << "6 => 1.000.000" << endl;
    cout << "7 => Todos os tamanhos" << endl;
    cout << "0 => Voltar" << endl;
    cout << "Escolha uma opcao: ";
    return lerOpcao();
}

// Menu de execucao de um algoritmo especifico (tipo -> tamanho -> executa).
void menuAlgoritmo(int indiceAlgoritmo) {
    const string &nome = NOMES_ALGORITMOS[indiceAlgoritmo];
    FuncaoOrdenacao funcao = FUNCOES_ALGORITMOS[indiceAlgoritmo];
    bool ok = true;

    int tipo = menuTipo();
    if (tipo == 0) return;
    if (tipo < 1 || tipo > 4) { cout << "Opcao invalida." << endl; return; }

    int tamOpc = menuTamanho();
    if (tamOpc == 0) return;
    if (tamOpc < 1 || tamOpc > 7) { cout << "Opcao invalida." << endl; return; }

    if (tipo == 4) { // todos os tipos
        if (tamOpc == 7) {
            ok = executarTodosOsTipos(nome, funcao);
        } else {
            for (int t = 1; t <= 3 && ok; t++) ok = operacoes(nome, funcao, t, TAMANHOS[tamOpc - 1]);
        }
    } else {
        if (tamOpc == 7) {
            ok = executarTodosOsTamanhos(nome, funcao, tipo);
        } else {
            ok = operacoes(nome, funcao, tipo, TAMANHOS[tamOpc - 1]);
        }
    }

    if (ok) {
        cout << "Execucao concluida. Pastas e arquivos de " << nome << " atualizados." << endl;
    } else {
        cout << "Execucao interrompida por erro (veja a mensagem acima)." << endl;
    }
}

// Executa todos os algoritmos e tipos para um unico tamanho (utilitario
// interno de automacao/testes; o menu interativo continua sendo a forma
// principal de uso do programa, conforme pedido no enunciado).
bool executarTudoParaTamanho(int tamanho) {
    for (int a = 0; a < 4; a++) {
        for (int tipo = 1; tipo <= 3; tipo++) {
            if (!operacoes(NOMES_ALGORITMOS[a], FUNCOES_ALGORITMOS[a], tipo, tamanho)) return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    srand((unsigned int) time(NULL));

    // Modo de automacao (opcional, alem do menu interativo abaixo):
    //   programa --tudo                      -> roda os 4 algoritmos, 3 tipos, 6 tamanhos
    //   programa --tamanho N                 -> roda os 4 algoritmos e 3 tipos so para o tamanho N
    //   programa --tamanho N --alg I         -> idem, so para o algoritmo I
    //                                           (0=Insertion 1=Selection 2=Shell 3=Bubble)
    if (argc > 1 && string(argv[1]) == "--tudo") {
        return executarTudo() ? 0 : 1;
    }
    if (argc > 4 && string(argv[1]) == "--tamanho" && string(argv[3]) == "--alg") {
        int tamanho = atoi(argv[2]);
        int idx = atoi(argv[4]);
        if (tamanho <= 0 || idx < 0 || idx > 3) {
            printf("Uso: %s --tamanho N --alg I   (N > 0; I: 0=Insertion 1=Selection 2=Shell 3=Bubble)\n", argv[0]);
            return 1;
        }
        for (int tipo = 1; tipo <= 3; tipo++) {
            if (!operacoes(NOMES_ALGORITMOS[idx], FUNCOES_ALGORITMOS[idx], tipo, tamanho)) return 1;
        }
        return 0;
    }
    if (argc > 2 && string(argv[1]) == "--tamanho") {
        int tamanho = atoi(argv[2]);
        if (tamanho <= 0) {
            printf("Uso: %s --tamanho N   (N > 0)\n", argv[0]);
            return 1;
        }
        return executarTudoParaTamanho(tamanho) ? 0 : 1;
    }

    int opcao;
    bool sair = false;

    // Avisa onde os arquivos serao gravados (pasta onde o programa foi aberto).
    cout << "Os arquivos serao gravados em: " << pastaAtual() << endl;

    while (!sair) {
        cout << endl << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "=-=-=-=-=       Menu        =-=-=-=-=-=" << endl;
        cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
        cout << "1 => Insertion Sort" << endl;
        cout << "2 => Selection Sort" << endl;
        cout << "3 => Shell Sort" << endl;
        cout << "4 => Bubble Sort" << endl;
        cout << "5 => Executar TODOS os algoritmos (gera toda a estrutura automaticamente)" << endl;
        cout << "0 => Sair do Programa" << endl;
        cout << "Escolha uma opcao: ";
        opcao = lerOpcao();

        switch (opcao) {
            case 1: case 2: case 3: case 4:
                menuAlgoritmo(opcao - 1);
                break;
            case 5:
                if (executarTudo()) {
                    cout << endl << "Execucao completa concluida para os 4 algoritmos." << endl;
                } else {
                    cout << endl << "Execucao interrompida por erro (veja a mensagem acima)." << endl;
                }
                break;
            case 0:
                sair = true;
                break;
            default:
                cout << "Opcao invalida." << endl;
        }
    }

    return 0;
}
