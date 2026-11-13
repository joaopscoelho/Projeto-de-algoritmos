# Entrega 4 — Algoritmos de Ordenação

Insertion Sort, Selection Sort, Shell Sort, Bubble Sort, Merge Sort e as duas versões do
**Quick Sort** (novas nesta entrega) em um único programa com menu (algoritmo → forma de
execução → tipo de entrada → tamanho). O próprio programa cria as pastas e os arquivos de
entrada, saída e tempo.

## O Quick Sort

As duas versões estão em `algoritmos.h` e se diferenciam **apenas na escolha do pivô**:

| Versão | Função | Pivô | Comportamento |
|---|---|---|---|
| 1 | `QuickSortVersao1` | primeiro elemento do trecho | O(n log n) na entrada randômica; **O(n²) nas entradas já ordenadas** (pior caso) |
| 2 | `QuickSortVersao2` | média aritmética dos elementos do trecho | O(n log n) nas três entradas |

As funções `ParticionaVersao1`, `ParticionaVersao2`, `QuickSortVersao1Recursivo` e
`QuickSortVersao2Recursivo` são auxiliares delas.

**Sobre a recursão:** no pior caso da versão 1 (entrada ordenada) uma das partições fica
vazia e a recursão do algoritmo clássico chegaria a `n` níveis, estourando a pilha com
1.000.000 de elementos. Por isso a chamada recursiva é feita sempre na **menor** partição,
e a maior continua no próprio laço `while`. As partições, as comparações e as trocas são
exatamente as mesmas do algoritmo clássico (a complexidade não muda), mas a pilha passa a
ter no máximo O(log n) níveis.

## Ordenação em vetor e em arquivo

Como pede o enunciado, cada algoritmo pode ser executado de duas formas, escolhidas no menu:

- **Em vetor:** a instância é gerada na memória, ordenada e gravada nos três arquivos.
- **Em arquivo:** a instância é **lida do arquivo de entrada** já gravado em disco (função
  `lerEntrada`), ordenada, e os arquivos de saída e de tempo são gravados. Se o arquivo de
  entrada ainda não existir, ele é gerado antes, para o programa funcionar numa pasta vazia.

Nas duas formas o tempo medido é apenas o da ordenação, sem a leitura e a gravação dos
arquivos, para que os valores sejam comparáveis entre si.

## Como compilar e executar

Basta um compilador C++ **qualquer** (o código não exige C++11/17 e funciona em Windows, Linux e macOS).
Compile **somente o `main.cpp`**; os arquivos `.h` desta pasta precisam estar junto dele.

| Ambiente | Comando / passos |
|---|---|
| Windows – MinGW / g++ (terminal) | `g++ -O2 main.cpp -o main.exe` e depois `main.exe` |
| Windows – Dev-C++ ou Code::Blocks | Abrir o `main.cpp`, compilar e executar (F11 no Dev-C++) |
| Windows – Visual Studio | Criar um **Projeto Vazio**, adicionar o `main.cpp` e compilar. (No modelo "Aplicativo de Console", desative os cabeçalhos pré-compilados.) Ou, no *Developer Command Prompt*: `cl /EHsc /O2 main.cpp` |
| Linux | `g++ -O2 main.cpp -o main && ./main` |
| macOS | `clang++ -O2 main.cpp -o main && ./main` |

Para gerar um `.exe` que roda em qualquer Windows, **sem precisar de DLLs do MinGW**, compile com
`g++ -O2 -static main.cpp -o main.exe`.

## Onde os arquivos são gerados

As pastas `Insertion Sort/`, `Selection Sort/`, `Shell Sort/`, `Bubble Sort/`, `Merge Sort/`,
`Quick Sort Versao 1/` e `Quick Sort Versao 2/` (cada uma com `Arquivos de Entrada`,
`Arquivos de Saida` e `Arquivos de Tempo`, e dentro delas `Crescente`, `Decrescente` e
`Random`) são criadas **na pasta em que o programa é executado**. Ao abrir, o programa
mostra esse caminho.

Se aparecer um erro de permissão, execute o programa a partir de uma pasta onde seja possível gravar
(por exemplo, Documentos ou Área de Trabalho) — não de dentro de um `.zip`, de um pendrive protegido
ou de `Arquivos de Programas`.

## Tempo de execução

Com 1.000.000 de elementos:

- **Rápidos (fração de segundo):** Shell Sort, Merge Sort, Quick Sort versão 2 (nas três
  entradas) e Quick Sort versão 1 **na entrada randômica**.
- **Lentos (minutos a cerca de uma hora):** Insertion, Selection e Bubble Sort, e o
  **Quick Sort versão 1 nas entradas crescente e decrescente** (pior caso, O(n²)):
  foram medidos 154 s e 229 s.

Para uma demonstração rápida, use tamanhos até 100.000 no menu. Para gerar só os dados do
Quick Sort versão 2 (poucos segundos): menu `7` → `1` (em vetor) → `4` (todos os tipos) →
`7` (todos os tamanhos). Evite a opção "Executar TODOS os algoritmos" se não houver tempo.

Modo direto, sem menu (útil para testar):

```
main --tamanho 1000                          # 7 algoritmos × 3 tipos, só para n = 1000
main --tamanho 1000000 --alg 5               # só um algoritmo
                                             # (0=Insertion 1=Selection 2=Shell 3=Bubble
                                             #  4=Merge 5=Quick v1 6=Quick v2)
main --tamanho 1000000 --alg 5 --tipo 3      # só um tipo (1=Crescente 2=Decrescente 3=Random)
main --tamanho 1000 --alg 6 --arquivo        # lê a instância do arquivo de entrada
main --tudo                                  # tudo (demora)
```

## Observação sobre os tempos

O tempo é medido com `clock()`. No Windows ele tem resolução de cerca de 1 ms, então para
instâncias muito pequenas o tempo pode aparecer como `0.000000`. Isso é esperado.

Os tempos das duas versões do Quick Sort gravados nesta pasta foram medidos em ambiente de
nuvem, porque o pior caso da versão 1 com 1.000.000 de elementos leva vários minutos de
processamento contínuo. Os dos outros cinco algoritmos foram medidos no computador pessoal.
O relatório explica essa diferença e traz a calibração entre os dois ambientes.
