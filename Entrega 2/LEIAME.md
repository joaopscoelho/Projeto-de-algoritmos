# Entrega 2 — Algoritmos de Ordenação

Insertion Sort, Selection Sort, Shell Sort e Bubble Sort em um único programa com menu
(algoritmo → tipo de entrada → tamanho). O próprio programa cria as pastas e os arquivos de
entrada, saída e tempo.

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

As pastas `Insertion Sort/`, `Selection Sort/`, `Shell Sort/` e `Bubble Sort/` (cada uma com
`Arquivos de Entrada`, `Arquivos de Saida` e `Arquivos de Tempo`, e dentro delas `Crescente`,
`Decrescente` e `Random`) são criadas **na pasta em que o programa é executado**. Ao abrir, o
programa mostra esse caminho.

Se aparecer um erro de permissão, execute o programa a partir de uma pasta onde seja possível gravar
(por exemplo, Documentos ou Área de Trabalho) — não de dentro de um `.zip`, de um pendrive protegido
ou de `Arquivos de Programas`.

## Tempo de execução

Com 1.000.000 de elementos, Insertion, Selection e Bubble Sort levam de alguns minutos até cerca de
uma hora (pior caso, O(n²)). Para uma demonstração rápida, use tamanhos até 100.000 no menu, ou o Shell Sort.
Evite a opção "Executar TODOS os algoritmos" se não houver tempo.

Modo direto, sem menu (útil para testar):

```
main --tamanho 1000              # 4 algoritmos × 3 tipos, só para n = 1000
main --tamanho 1000000 --alg 2   # só um algoritmo (0=Insertion 1=Selection 2=Shell 3=Bubble)
main --tudo                      # tudo (demora)
```

## Observação sobre os tempos

O tempo é medido com `clock()`. No Windows ele tem resolução de cerca de 1 ms, então para
instâncias muito pequenas o tempo pode aparecer como `0.000000`. Isso é esperado.
