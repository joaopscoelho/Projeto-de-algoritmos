#ifndef ALGORITMOS_H
#define ALGORITMOS_H

// ============================================================================
// algoritmos.h
// Contem: os sete algoritmos de ordenacao (Insertion, Selection, Bubble,
// Shell, Merge Sort e as duas versoes do Quick Sort), as funcoes de manipulacao de arquivos (entrada, saida
// e tempo) e a criacao automatica da estrutura de pastas exigida pelo
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

// Merge Sort (versao tradicional, recursiva, "dividir para conquistar").
// Divide o vetor ao meio, ordena cada metade recursivamente e, por fim,
// intercala (merge) as duas metades ja ordenadas usando um vetor auxiliar.
// Diferente dos demais, o numero de comparacoes quase nao depende da ordem
// da entrada: o custo e Theta(n log n) no melhor, no medio e no pior caso,
// ao preco de O(n) de memoria extra (o vetor auxiliar).

// Intercala as duas metades ja ordenadas vetor[inicio..meio] e
// vetor[meio+1..fim]. "aux" e um vetor auxiliar com o mesmo tamanho de
// "vetor"; so as posicoes [inicio, fim] sao usadas.
void Intercalar(int *vetor, int *aux, int inicio, int meio, int fim) {
    int i = inicio;     // percorre a metade esquerda
    int j = meio + 1;   // percorre a metade direita
    int k = inicio;     // proxima posicao livre do vetor auxiliar

    // repetidamente copia para o auxiliar o menor entre os dois "da frente"
    while (i <= meio && j <= fim) {
        if (vetor[i] <= vetor[j]) { // "<=": em caso de empate, a esquerda sai primeiro (estavel)
            aux[k++] = vetor[i++];
        } else {
            aux[k++] = vetor[j++];
        }
    }

    // uma das metades acabou; copia o que sobrou da outra
    while (i <= meio) aux[k++] = vetor[i++];
    while (j <= fim)  aux[k++] = vetor[j++];

    // devolve o trecho ja intercalado para o vetor original
    for (k = inicio; k <= fim; k++) {
        vetor[k] = aux[k];
    }
}

// Ordena o trecho vetor[inicio..fim] (extremos incluidos).
void MergeSortRecursivo(int *vetor, int *aux, int inicio, int fim) {
    if (inicio >= fim) return; // 0 ou 1 elemento: ja esta ordenado

    int meio = inicio + (fim - inicio) / 2;

    MergeSortRecursivo(vetor, aux, inicio, meio);    // ordena a metade esquerda
    MergeSortRecursivo(vetor, aux, meio + 1, fim);   // ordena a metade direita
    Intercalar(vetor, aux, inicio, meio, fim);       // junta as duas metades
}

// Ponto de entrada do Merge Sort, com a mesma assinatura dos demais
// algoritmos (vetor, tamanho). Aloca o vetor auxiliar uma unica vez; esse
// custo de alocacao faz parte do algoritmo e entra no tempo medido.
void MergeSort(int *vetor, int tamanho) {
    if (tamanho < 2) return;

    int *aux = new int[tamanho];
    MergeSortRecursivo(vetor, aux, 0, tamanho - 1);
    delete[] aux;
}

// Quick Sort (ordenacao por troca de particoes, "dividir para conquistar").
// Escolhe um elemento do trecho como pivo, reorganiza o trecho de modo que os
// menores fiquem a esquerda do pivo e os maiores a direita (particao) e
// repete o processo, recursivamente, nos dois lados. Ao contrario do Merge
// Sort, NAO existe fase de combinacao: a particao ja deixa o pivo na sua
// posicao definitiva, e quando os dois lados estao ordenados o vetor inteiro
// esta ordenado.
//
// Sao implementadas duas versoes, como pede o trabalho:
//   Versao 1 - pivo = primeiro elemento do trecho (versao apresentada em aula);
//   Versao 2 - pivo = media aritmetica dos elementos do trecho, para melhorar
//              a escolha do pivo em entradas ja ordenadas.

// Troca o conteudo de duas posicoes do vetor.
void trocar(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

// --- Versao 1: pivo = primeiro elemento -----------------------------------

// Particiona o trecho vetor[inicio..fim] usando como pivo o PRIMEIRO
// elemento (vetor[inicio]). Percorre o trecho uma unica vez mantendo em "i"
// a ultima posicao ocupada por um elemento menor ou igual ao pivo; ao final,
// o pivo e levado para a posicao i, que e a sua posicao DEFINITIVA no vetor
// ordenado. Retorna essa posicao.
// Custo: exatamente m - 1 comparacoes para m = fim - inicio + 1 elementos,
// ou seja, O(m) -- cada elemento e visitado uma unica vez.
int ParticionaVersao1(int *vetor, int inicio, int fim) {
    int pivo = vetor[inicio];   // pivo: primeiro elemento do trecho
    int i = inicio;             // ultima posicao dos elementos <= pivo

    for (int j = inicio + 1; j <= fim; j++) {
        if (vetor[j] <= pivo) {
            i++;
            trocar(&vetor[i], &vetor[j]);
        }
    }

    trocar(&vetor[inicio], &vetor[i]); // pivo vai para a sua posicao final
    return i;
}

// Ordena o trecho vetor[inicio..fim] (extremos incluidos).
//
// Observacao importante sobre a recursao: no pior caso (entrada ja ordenada,
// em que o pivo e sempre o menor ou o maior elemento) uma das particoes fica
// vazia e a outra com m - 1 elementos, de modo que a recursao atingiria
// profundidade n -- para n = 1.000.000 isso estoura a pilha do programa
// (falha de segmentacao) antes de terminar. Por isso aplica-se aqui a
// eliminacao da recursao de cauda: a chamada recursiva e feita sempre na
// MENOR particao e a maior continua no proprio laco. As particoes, as
// comparacoes e as trocas sao EXATAMENTE as mesmas do algoritmo classico
// (a analise de complexidade nao muda), mas a pilha passa a ter no maximo
// O(log n) niveis, o que permite executar tambem o pior caso com 1.000.000
// de elementos.
void QuickSortVersao1Recursivo(int *vetor, int inicio, int fim) {
    while (inicio < fim) {
        int p = ParticionaVersao1(vetor, inicio, fim); // pivo na posicao p

        if (p - inicio < fim - p) {                    // lado esquerdo menor
            QuickSortVersao1Recursivo(vetor, inicio, p - 1);
            inicio = p + 1;                            // continua no direito
        } else {                                       // lado direito menor
            QuickSortVersao1Recursivo(vetor, p + 1, fim);
            fim = p - 1;                               // continua no esquerdo
        }
    }
}

// Ponto de entrada da versao 1, com a mesma assinatura dos demais
// algoritmos do trabalho (vetor e tamanho).
void QuickSortVersao1(int *vetor, int tamanho) {
    if (tamanho < 2) return;
    QuickSortVersao1Recursivo(vetor, 0, tamanho - 1);
}

// --- Versao 2: pivo = media aritmetica do trecho --------------------------

// Particiona o trecho vetor[inicio..fim] usando como pivo a MEDIA ARITMETICA
// dos seus elementos. A media e um valor (nao necessariamente um elemento do
// vetor) que fica sempre entre o menor e o maior elemento do trecho, o que
// evita o pior caso da versao 1: em uma entrada ja ordenada (crescente ou
// decrescente) a media cai no meio do trecho e as duas particoes ficam
// equilibradas.
//
// A varredura e a de Hoare (dois indices que caminham um para o outro):
// "i" anda para a direita enquanto encontra elementos menores que a media,
// "j" anda para a esquerda enquanto encontra elementos maiores que a media, e
// os dois elementos que travam a varredura sao trocados. Ao final, os indices
// se cruzam e os trechos a ordenar sao vetor[inicio..j] e vetor[i..fim] --
// por isso os dois indices sao devolvidos ao chamador.
//
// A soma e acumulada em "double" porque a soma de 1.000.000 de valores de
// ate 10^9 passa do limite de um int (ate 2^53 o double representa inteiros
// de forma exata, logo nao ha perda de precisao).
//
// Custo: a soma percorre os m elementos uma vez e a varredura os percorre
// mais uma vez, ou seja, O(m) -- apenas o dobro do trabalho da versao 1,
// o que nao altera a ordem de complexidade.
void ParticionaVersao2(int *vetor, int inicio, int fim, int *i, int *j) {
    double soma = 0.0;
    for (int k = inicio; k <= fim; k++) {
        soma += vetor[k];
    }
    int media = (int) (soma / (double) (fim - inicio + 1)); // pivo

    *i = inicio;
    *j = fim;

    do {
        // Os lacos nao precisam de teste de limite: como a media fica entre
        // o menor e o maior elemento do trecho, existe sempre um elemento
        // >= media para parar "i" e um elemento <= media para parar "j".
        while (vetor[*i] < media) (*i)++;
        while (vetor[*j] > media) (*j)--;

        if (*i <= *j) {
            trocar(&vetor[*i], &vetor[*j]);
            (*i)++;
            (*j)--;
        }
    } while (*i <= *j);
}

// Ordena o trecho vetor[inicio..fim]. Mesma eliminacao de recursao de cauda
// da versao 1 (recursao na menor particao, laco na maior).
void QuickSortVersao2Recursivo(int *vetor, int inicio, int fim) {
    while (inicio < fim) {
        int i, j;
        ParticionaVersao2(vetor, inicio, fim, &i, &j);

        // trechos que faltam ordenar: [inicio..j] e [i..fim]
        if (j - inicio < fim - i) {                    // lado esquerdo menor
            QuickSortVersao2Recursivo(vetor, inicio, j);
            inicio = i;                                // continua no direito
        } else {                                       // lado direito menor
            QuickSortVersao2Recursivo(vetor, i, fim);
            fim = j;                                   // continua no esquerdo
        }
    }
}

// Ponto de entrada da versao 2.
void QuickSortVersao2(int *vetor, int tamanho) {
    if (tamanho < 2) return;
    QuickSortVersao2Recursivo(vetor, 0, tamanho - 1);
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

// ---------------------------------------------------------------------------
// 5. Leitura da instancia a partir do arquivo (modo "em arquivo")
// ---------------------------------------------------------------------------

// Le a instancia gravada no arquivo de entrada correspondente, usada pelo
// modo "em arquivo" do menu: em vez de gerar os numeros na memoria, o
// programa carrega para o vetor os numeros que estao no arquivo de entrada
// ja gravado em disco, ordena esse vetor e grava os arquivos de saida e de
// tempo.
//
// Formato lido (o mesmo que o programa grava): primeira linha com o tamanho
// da instancia e, em seguida, um valor por linha.
//
// Retorna um vetor alocado com new[] (o chamador libera com delete[]) e
// escreve em *tamanhoLido quantos valores foram lidos. Retorna NULL se o
// arquivo nao existir ou estiver incompleto.
int* lerEntrada(const string &algoritmo, int tipo, int tamanho, int *tamanhoLido) {
    string categoria = nomeCategoria(tipo);
    string nomeArquivo = algoritmo + "/Arquivos de Entrada/" + categoria + "/Entrada" +
                          categoria + inteiroParaTexto(tamanho) + ".txt";

    FILE *arq = fopen(nomeArquivo.c_str(), "r");
    if (arq == NULL) {
        printf("ERRO: nao foi possivel abrir o arquivo de entrada \"%s\" (%s).\n",
               nomeArquivo.c_str(), strerror(errno));
        return NULL;
    }

    int quantidade = 0;
    if (fscanf(arq, "%d", &quantidade) != 1 || quantidade <= 0) {
        printf("ERRO: o arquivo \"%s\" nao comeca com o tamanho da instancia.\n", nomeArquivo.c_str());
        fclose(arq);
        return NULL;
    }

    int *vetor = new int[quantidade];
    for (int i = 0; i < quantidade; i++) {
        if (fscanf(arq, "%d", &vetor[i]) != 1) {
            printf("ERRO: o arquivo \"%s\" tem menos valores (%d) do que o tamanho informado (%d).\n",
                   nomeArquivo.c_str(), i, quantidade);
            delete[] vetor;
            fclose(arq);
            return NULL;
        }
    }
    fclose(arq);

    *tamanhoLido = quantidade;
    return vetor;
}

// Informa se o arquivo de entrada daquela instancia ja existe em disco.
bool existeEntrada(const string &algoritmo, int tipo, int tamanho) {
    string categoria = nomeCategoria(tipo);
    string nomeArquivo = algoritmo + "/Arquivos de Entrada/" + categoria + "/Entrada" +
                          categoria + inteiroParaTexto(tamanho) + ".txt";

    FILE *arq = fopen(nomeArquivo.c_str(), "r");
    if (arq == NULL) return false;
    fclose(arq);
    return true;
}

#endif
