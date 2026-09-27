#include <iostream>
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

// Gera a instancia do tipo indicado (1-Crescente, 2-Decrescente, 3-Random).
int* gerarSequencia(int tipo, int tamanho) {
    if (tipo == 1) return gerarCrescente(tamanho);
    if (tipo == 2) return gerarDecrescente(tamanho);
    return gerarRandom(tamanho);
}

// Gera a instancia, executa o algoritmo de ordenacao indicado medindo o
// tempo gasto (clock()/CLOCKS_PER_SEC) e salva os arquivos de entrada,
// tempo e saida correspondentes dentro da pasta do algoritmo.
void operacoes(const string &algoritmo, FuncaoOrdenacao funcao, int tipo, int tamanho) {
    criarEstruturaPastas(algoritmo); // garante que as pastas existem (criadas pelo codigo)

    clock_t start_t, end_t;
    double tempoGasto;

    int *vetor = gerarSequencia(tipo, tamanho);

    salvarEntrada(algoritmo, tipo, tamanho, vetor);

    start_t = clock();
    funcao(vetor, tamanho);
    end_t = clock();

    tempoGasto = (end_t - start_t) / (double)CLOCKS_PER_SEC;

    salvarTempo(algoritmo, tipo, tamanho, tempoGasto);
    salvarSaida(algoritmo, tipo, tamanho, vetor);

    delete[] vetor;

    printf("  [%s] tamanho=%d tipo=%s tempo=%f s\n", algoritmo.c_str(), tamanho, nomeCategoria(tipo).c_str(), tempoGasto);
}

// Executa um algoritmo para um tipo de instancia, em todos os tamanhos
// exigidos pelo trabalho (10, 100, 1.000, 10.000, 100.000 e 1.000.000).
void executarTodosOsTamanhos(const string &algoritmo, FuncaoOrdenacao funcao, int tipo) {
    for (int i = 0; i < 6; i++) {
        operacoes(algoritmo, funcao, tipo, TAMANHOS[i]);
    }
}

// Executa um algoritmo para todos os tipos de instancia (Crescente,
// Decrescente e Random), em todos os tamanhos.
void executarTodosOsTipos(const string &algoritmo, FuncaoOrdenacao funcao) {
    for (int tipo = 1; tipo <= 3; tipo++) {
        executarTodosOsTamanhos(algoritmo, funcao, tipo);
    }
}

// Executa os 4 algoritmos, para os 3 tipos e os 6 tamanhos: gera toda a
// estrutura de pastas e arquivos do trabalho em uma unica chamada.
void executarTudo() {
    for (int a = 0; a < 4; a++) {
        printf("\n===== Executando %s =====\n", NOMES_ALGORITMOS[a].c_str());
        executarTodosOsTipos(NOMES_ALGORITMOS[a], FUNCOES_ALGORITMOS[a]);
    }
}

int menuTipo() {
    int tipo;
    cout << endl << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "=-=-=  Tipo de Entradas  =-=-=" << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "1 => Crescente" << endl;
    cout << "2 => Decrescente" << endl;
    cout << "3 => Randomico" << endl;
    cout << "4 => Todos os tipos" << endl;
    cout << "0 => Voltar" << endl;
    cout << "Escolha uma opcao: ";
    cin >> tipo;
    return tipo;
}

int menuTamanho() {
    int opcao;
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
    cin >> opcao;
    return opcao;
}

// Menu de execucao de um algoritmo especifico (tipo -> tamanho -> executa).
void menuAlgoritmo(int indiceAlgoritmo) {
    const string &nome = NOMES_ALGORITMOS[indiceAlgoritmo];
    FuncaoOrdenacao funcao = FUNCOES_ALGORITMOS[indiceAlgoritmo];

    int tipo = menuTipo();
    if (tipo == 0) return;
    if (tipo < 1 || tipo > 4) { cout << "Opcao invalida." << endl; return; }

    if (tipo == 4) { // todos os tipos
        int tamOpc = menuTamanho();
        if (tamOpc == 0) return;
        if (tamOpc == 7) {
            executarTodosOsTipos(nome, funcao);
        } else if (tamOpc >= 1 && tamOpc <= 6) {
            for (int t = 1; t <= 3; t++) operacoes(nome, funcao, t, TAMANHOS[tamOpc - 1]);
        } else {
            cout << "Opcao invalida." << endl; return;
        }
    } else {
        int tamOpc = menuTamanho();
        if (tamOpc == 0) return;
        if (tamOpc == 7) {
            executarTodosOsTamanhos(nome, funcao, tipo);
        } else if (tamOpc >= 1 && tamOpc <= 6) {
            operacoes(nome, funcao, tipo, TAMANHOS[tamOpc - 1]);
        } else {
            cout << "Opcao invalida." << endl; return;
        }
    }

    cout << "Execucao concluida. Pastas e arquivos de " << nome << " atualizados." << endl;
}

// Executa todos os algoritmos e tipos para um unico tamanho (utilitario
// interno de automacao/testes; o menu interativo continua sendo a forma
// principal de uso do programa, conforme pedido no enunciado).
void executarTudoParaTamanho(int tamanho) {
    for (int a = 0; a < 4; a++) {
        for (int tipo = 1; tipo <= 3; tipo++) {
            operacoes(NOMES_ALGORITMOS[a], FUNCOES_ALGORITMOS[a], tipo, tamanho);
        }
    }
}

int main(int argc, char *argv[]) {
    srand((unsigned int) time(NULL));

    // Modo de automacao (opcional, alem do menu interativo abaixo):
    //   ./programa --tudo            -> roda os 4 algoritmos, 3 tipos, 6 tamanhos
    //   ./programa --tamanho 1000000 -> roda os 4 algoritmos e 3 tipos so para esse tamanho
    if (argc > 1 && string(argv[1]) == "--tudo") {
        executarTudo();
        return 0;
    }
    if (argc > 4 && string(argv[1]) == "--tamanho" && string(argv[3]) == "--alg") {
        int tamanho = atoi(argv[2]);
        int idx = atoi(argv[4]); // 0=Insertion 1=Selection 2=Shell 3=Bubble
        for (int tipo = 1; tipo <= 3; tipo++) {
            operacoes(NOMES_ALGORITMOS[idx], FUNCOES_ALGORITMOS[idx], tipo, tamanho);
        }
        return 0;
    }
    if (argc > 2 && string(argv[1]) == "--tamanho") {
        executarTudoParaTamanho(atoi(argv[2]));
        return 0;
    }

    int opcao;
    bool sair = false;

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
        cin >> opcao;

        switch (opcao) {
            case 1: case 2: case 3: case 4:
                menuAlgoritmo(opcao - 1);
                break;
            case 5:
                executarTudo();
                cout << endl << "Execucao completa concluida para os 4 algoritmos." << endl;
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
