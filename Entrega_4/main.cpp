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

// Os 7 algoritmos do trabalho, na ordem em que aparecem no menu.
// O indice do vetor e (opcao do menu - 1) e tambem o valor aceito por "--alg".
const int NUM_ALGORITMOS = 7;
const string NOMES_ALGORITMOS[NUM_ALGORITMOS] = {
    "Insertion Sort",
    "Selection Sort",
    "Shell Sort",
    "Bubble Sort",
    "Merge Sort",
    "Quick Sort Versao 1",
    "Quick Sort Versao 2"
};
const FuncaoOrdenacao FUNCOES_ALGORITMOS[NUM_ALGORITMOS] = {
    InsertionSort,
    SelectionSort,
    ShellSort,
    BubbleSort,
    MergeSort,
    QuickSortVersao1,
    QuickSortVersao2
};

const int TAMANHOS[6] = {10, 100, 1000, 10000, 100000, 1000000};

// Formas de execucao pedidas pelo enunciado ("usando vetor e arquivo"):
//   MODO_VETOR   - a instancia e gerada na memoria, ordenada e gravada;
//   MODO_ARQUIVO - a instancia e lida do arquivo de entrada ja gravado em
//                  disco, ordenada e os arquivos de saida/tempo sao gravados.
const int MODO_VETOR = 1;
const int MODO_ARQUIVO = 2;

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

// Executa o algoritmo de ordenacao indicado medindo o tempo gasto
// (clock()/CLOCKS_PER_SEC) e salva os arquivos de entrada, tempo e saida
// correspondentes dentro da pasta do algoritmo.
//
// No MODO_VETOR a instancia e gerada na memoria e gravada no arquivo de
// entrada. No MODO_ARQUIVO a instancia e lida do arquivo de entrada ja
// existente (se ele ainda nao existir, e gerado antes, para que o programa
// funcione mesmo numa pasta vazia). Nos dois modos o tempo medido e apenas
// o da ordenacao, sem a leitura e a gravacao dos arquivos, para que os
// valores possam ser comparados entre si.
//
// Retorna false se alguma pasta/arquivo nao puder ser criado ou lido.
bool operacoes(const string &algoritmo, FuncaoOrdenacao funcao, int tipo, int tamanho, int modo) {
    if (!criarEstruturaPastas(algoritmo)) return false; // pastas criadas pelo codigo

    clock_t start_t, end_t;
    double tempoGasto;
    int *vetor;

    if (modo == MODO_ARQUIVO) {
        // garante que exista um arquivo de entrada para ser lido
        if (!existeEntrada(algoritmo, tipo, tamanho)) {
            int *gerado = gerarSequencia(tipo, tamanho);
            bool gravouEntrada = salvarEntrada(algoritmo, tipo, tamanho, gerado);
            delete[] gerado;
            if (!gravouEntrada) return false;
        }

        int tamanhoLido = 0;
        vetor = lerEntrada(algoritmo, tipo, tamanho, &tamanhoLido);
        if (vetor == NULL) return false;
        tamanho = tamanhoLido; // usa o tamanho realmente lido do arquivo
    } else {
        vetor = gerarSequencia(tipo, tamanho);

        if (!salvarEntrada(algoritmo, tipo, tamanho, vetor)) {
            delete[] vetor;
            return false;
        }
    }

    start_t = clock();
    funcao(vetor, tamanho);
    end_t = clock();

    tempoGasto = (end_t - start_t) / (double)CLOCKS_PER_SEC;

    bool gravou = salvarTempo(algoritmo, tipo, tamanho, tempoGasto) &&
                  salvarSaida(algoritmo, tipo, tamanho, vetor);

    delete[] vetor;

    if (gravou) {
        printf("  [%s] modo=%s tamanho=%d tipo=%s tempo=%f s\n",
               algoritmo.c_str(),
               (modo == MODO_ARQUIVO ? "arquivo" : "vetor"),
               tamanho, nomeCategoria(tipo).c_str(), tempoGasto);
    }
    return gravou;
}

// Executa um algoritmo para um tipo de instancia, em todos os tamanhos
// exigidos pelo trabalho (10, 100, 1.000, 10.000, 100.000 e 1.000.000).
bool executarTodosOsTamanhos(const string &algoritmo, FuncaoOrdenacao funcao, int tipo, int modo) {
    for (int i = 0; i < 6; i++) {
        if (!operacoes(algoritmo, funcao, tipo, TAMANHOS[i], modo)) return false;
    }
    return true;
}

// Executa um algoritmo para todos os tipos de instancia (Crescente,
// Decrescente e Random), em todos os tamanhos.
bool executarTodosOsTipos(const string &algoritmo, FuncaoOrdenacao funcao, int modo) {
    for (int tipo = 1; tipo <= 3; tipo++) {
        if (!executarTodosOsTamanhos(algoritmo, funcao, tipo, modo)) return false;
    }
    return true;
}

// Executa os 7 algoritmos, para os 3 tipos e os 6 tamanhos: gera toda a
// estrutura de pastas e arquivos do trabalho em uma unica chamada.
bool executarTudo(int modo) {
    for (int a = 0; a < NUM_ALGORITMOS; a++) {
        printf("\n===== Executando %s =====\n", NOMES_ALGORITMOS[a].c_str());
        if (!executarTodosOsTipos(NOMES_ALGORITMOS[a], FUNCOES_ALGORITMOS[a], modo)) return false;
    }
    return true;
}

int menuModo() {
    cout << endl << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "=-=-=  Forma de Execucao  =-=-=" << endl;
    cout << "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
    cout << "1 => Em vetor   (gera a instancia na memoria)" << endl;
    cout << "2 => Em arquivo (le a instancia do arquivo de entrada)" << endl;
    cout << "0 => Voltar" << endl;
    cout << "Escolha uma opcao: ";
    return lerOpcao();
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

// Menu de execucao de um algoritmo especifico
// (vetor/arquivo -> tipo -> tamanho -> executa).
void menuAlgoritmo(int indiceAlgoritmo) {
    const string &nome = NOMES_ALGORITMOS[indiceAlgoritmo];
    FuncaoOrdenacao funcao = FUNCOES_ALGORITMOS[indiceAlgoritmo];
    bool ok = true;

    int modo = menuModo();
    if (modo == 0) return;
    if (modo != MODO_VETOR && modo != MODO_ARQUIVO) { cout << "Opcao invalida." << endl; return; }

    int tipo = menuTipo();
    if (tipo == 0) return;
    if (tipo < 1 || tipo > 4) { cout << "Opcao invalida." << endl; return; }

    int tamOpc = menuTamanho();
    if (tamOpc == 0) return;
    if (tamOpc < 1 || tamOpc > 7) { cout << "Opcao invalida." << endl; return; }

    if (tipo == 4) { // todos os tipos
        if (tamOpc == 7) {
            ok = executarTodosOsTipos(nome, funcao, modo);
        } else {
            for (int t = 1; t <= 3 && ok; t++) ok = operacoes(nome, funcao, t, TAMANHOS[tamOpc - 1], modo);
        }
    } else {
        if (tamOpc == 7) {
            ok = executarTodosOsTamanhos(nome, funcao, tipo, modo);
        } else {
            ok = operacoes(nome, funcao, tipo, TAMANHOS[tamOpc - 1], modo);
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
bool executarTudoParaTamanho(int tamanho, int modo) {
    for (int a = 0; a < NUM_ALGORITMOS; a++) {
        for (int tipo = 1; tipo <= 3; tipo++) {
            if (!operacoes(NOMES_ALGORITMOS[a], FUNCOES_ALGORITMOS[a], tipo, tamanho, modo)) return false;
        }
    }
    return true;
}

// Procura uma opcao na linha de comando (ex.: "--arquivo").
bool temArgumento(int argc, char *argv[], const string &opcao) {
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == opcao) return true;
    }
    return false;
}

// Le o valor numerico que vem depois de uma opcao da linha de comando
// (ex.: "--tipo 2"). Devolve "padrao" se a opcao nao estiver presente.
int valorArgumento(int argc, char *argv[], const string &opcao, int padrao) {
    for (int i = 1; i + 1 < argc; i++) {
        if (string(argv[i]) == opcao) return atoi(argv[i + 1]);
    }
    return padrao;
}

int main(int argc, char *argv[]) {
    srand((unsigned int) time(NULL));

    // Modo de automacao (opcional, alem do menu interativo abaixo):
    //   programa --tudo                      -> roda os 7 algoritmos, 3 tipos, 6 tamanhos
    //   programa --tamanho N                 -> roda os 7 algoritmos e 3 tipos so para o tamanho N
    //   programa --tamanho N --alg I         -> idem, so para o algoritmo I
    //                                           (0=Insertion 1=Selection 2=Shell 3=Bubble
    //                                            4=Merge 5=Quick v1 6=Quick v2)
    //   programa --tamanho N --alg I --tipo T-> idem, so para o tipo T
    //                                           (1=Crescente 2=Decrescente 3=Random)
    // Acrescentando --arquivo em qualquer um deles, a instancia e lida do
    // arquivo de entrada em vez de ser gerada na memoria.
    int modoLinhaComando = temArgumento(argc, argv, "--arquivo") ? MODO_ARQUIVO : MODO_VETOR;
    int tipoLinhaComando = valorArgumento(argc, argv, "--tipo", 0); // 0 = os tres tipos

    if (argc > 1 && string(argv[1]) == "--tudo") {
        return executarTudo(modoLinhaComando) ? 0 : 1;
    }
    if (argc > 4 && string(argv[1]) == "--tamanho" && string(argv[3]) == "--alg") {
        int tamanho = atoi(argv[2]);
        int idx = atoi(argv[4]);
        if (tamanho <= 0 || idx < 0 || idx >= NUM_ALGORITMOS || tipoLinhaComando < 0 || tipoLinhaComando > 3) {
            printf("Uso: %s --tamanho N --alg I [--tipo T] [--arquivo]\n", argv[0]);
            printf("     (N > 0; I: 0=Insertion 1=Selection 2=Shell 3=Bubble 4=Merge 5=Quick v1 6=Quick v2)\n");
            printf("     (T: 1=Crescente 2=Decrescente 3=Random; sem --tipo, roda os tres)\n");
            return 1;
        }
        if (tipoLinhaComando != 0) { // um unico tipo de instancia
            return operacoes(NOMES_ALGORITMOS[idx], FUNCOES_ALGORITMOS[idx],
                             tipoLinhaComando, tamanho, modoLinhaComando) ? 0 : 1;
        }
        for (int tipo = 1; tipo <= 3; tipo++) {
            if (!operacoes(NOMES_ALGORITMOS[idx], FUNCOES_ALGORITMOS[idx], tipo, tamanho, modoLinhaComando)) return 1;
        }
        return 0;
    }
    if (argc > 2 && string(argv[1]) == "--tamanho") {
        int tamanho = atoi(argv[2]);
        if (tamanho <= 0) {
            printf("Uso: %s --tamanho N [--arquivo]   (N > 0)\n", argv[0]);
            return 1;
        }
        return executarTudoParaTamanho(tamanho, modoLinhaComando) ? 0 : 1;
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
        cout << "5 => Merge Sort" << endl;
        cout << "6 => Quick Sort - Versao 1 (pivo = primeiro elemento)" << endl;
        cout << "7 => Quick Sort - Versao 2 (pivo = media dos elementos)" << endl;
        cout << "8 => Executar TODOS os algoritmos (gera toda a estrutura automaticamente)" << endl;
        cout << "0 => Sair do Programa" << endl;
        cout << "Escolha uma opcao: ";
        opcao = lerOpcao();

        switch (opcao) {
            case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                menuAlgoritmo(opcao - 1);
                break;
            case 8: {
                int modo = menuModo();
                if (modo == 0) break;
                if (modo != MODO_VETOR && modo != MODO_ARQUIVO) { cout << "Opcao invalida." << endl; break; }
                if (executarTudo(modo)) {
                    cout << endl << "Execucao completa concluida para os 7 algoritmos." << endl;
                } else {
                    cout << endl << "Execucao interrompida por erro (veja a mensagem acima)." << endl;
                }
                break;
            }
            case 0:
                sair = true;
                break;
            default:
                cout << "Opcao invalida." << endl;
        }
    }

    return 0;
}
