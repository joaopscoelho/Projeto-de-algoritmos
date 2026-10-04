// O Visual Studio trata fopen() como "inseguro" e interrompe a compilacao;
// este define precisa vir ANTES de qualquer #include (nos demais compiladores
// nao tem efeito).
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Operacoes.h"

using namespace std;

// Exibe o submenu para escolha do tipo de instancia e retorna a opcao.
int menuTipo() {
    int tipo;
    cout << "1 - Crescente" << endl;
    cout << "2 - Decrescente" << endl;
    cout << "3 - Random" << endl;
    cout << "Escolha o tipo de instancia: ";
    cin >> tipo;
    return tipo;
}

int main() {
    srand((unsigned int) time(NULL));

    int tipoAtual = 3; // Random, por padrao
    int opcao;
    bool sair = false;

    while (!sair) {
        cout << endl << "===== Insertion Sort - Projeto de Algoritmos =====" << endl;
        cout << "Tipo de instancia atual: " << nomeCategoria(tipoAtual) << endl;
        cout << "1 - Executar o Insertion Sort (todos os tamanhos: 10 a 1.000.000)" << endl;
        cout << "2 - Selecionar o tipo de instancia (Crescente, Decrescente ou Random)" << endl;
        cout << "3 - Sair da aplicacao" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                cout << "Executando Insertion Sort para o tipo " << nomeCategoria(tipoAtual) << "..." << endl;
                if (executarTodosOsTamanhos(tipoAtual)) {
                    cout << "Execucao concluida. Arquivos de entrada, saida e tempo atualizados." << endl;
                } else {
                    cout << "Execucao interrompida por erro (veja a mensagem acima)." << endl;
                }
                break;
            case 2:
                tipoAtual = menuTipo();
                break;
            case 3:
                sair = true;
                break;
            default:
                cout << "Opcao invalida." << endl;
        }
    }

    return 0;
}
