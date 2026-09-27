# Roteiro para as partes manuscritas do relatório

## 1. Teste de mesa manual

Vetor sugerido (diferente dos exemplos de aula - confira antes de usar):
[12, 45, 7, 23, 3, 39, 18]

| i | chave | Deslocamentos | Vetor ao final da iteracao |
|---|-------|---------------|------------------------------|
| 1 | 45 | nenhum | [12, 45, 7, 23, 3, 39, 18] |
| 2 | 7  | 45 e 12 deslocados | [7, 12, 45, 23, 3, 39, 18] |
| 3 | 23 | 45 deslocado | [7, 12, 23, 45, 3, 39, 18] |
| 4 | 3  | 45,23,12,7 deslocados | [3, 7, 12, 23, 45, 39, 18] |
| 5 | 39 | 45 deslocado | [3, 7, 12, 23, 39, 45, 18] |
| 6 | 18 | 45,39,23 deslocados | [3, 7, 12, 18, 23, 39, 45] |

Vetor final (ordenado): [3, 7, 12, 18, 23, 39, 45] - verificado computacionalmente.

Ao passar para o papel, mostre: a chave retirada, a comparacao vetor[j] > chave
a cada passo, e cada deslocamento, ate a insercao final.

## 2. Analise de complexidade - calculos completos

Base (n = tamanho do vetor, laco externo com n-1 iteracoes, i = 1..n-1):
c(i) = numero de comparacoes "vetor[j] > chave" na iteracao i
d(i) = numero de deslocamentos "vetor[j+1] = vetor[j]" na iteracao i

### Melhor caso (crescente)
vetor[i-1] <= chave sempre -> a 1a comparacao ja falha.
c(i) = 1, d(i) = 0 para toda iteracao.
T(n) ~ soma_{i=1}^{n-1} (1+0) = n-1  =>  O(n)   [linear]
Medido: ~0,001s para n=1.000.000.

### Pior caso (decrescente)
Todos os i elementos a esquerda sao maiores -> todos deslocados.
c(i) = d(i) = i.
T(n) ~ soma_{i=1}^{n-1} (i+i) = 2 * soma_{i=1}^{n-1} i

Soma de Gauss (deducao):
  S = 1+2+...+(n-1)
  S = (n-1)+(n-2)+...+1
 2S = n+n+...+n  (n-1 vezes) = n(n-1)
  S = n(n-1)/2

T(n) ~ 2 * n(n-1)/2 = n(n-1) = n^2 - n  =>  O(n^2)   [quadratico]
Medido: ~139,3s para n=1.000.000.

### Caso medio (randomica)
Na iteracao i ha i elementos ja ordenados; a chave tem igual chance de ficar
em qualquer uma das i+1 posicoes relativas. Numero esperado de deslocamentos:
  E[d(i)] = (1/(i+1)) * (0+1+2+...+i) = (1/(i+1)) * i(i+1)/2 = i/2
(metade dos i elementos, em media, precisa ser deslocada)

c(i) = d(i) = i/2
T(n) ~ soma_{i=1}^{n-1} (i/2+i/2) = soma_{i=1}^{n-1} i = n(n-1)/2  =>  O(n^2)
(metade do numero de operacoes do pior caso)
Medido: ~69,7s para n=1.000.000 (139,3/69,7 ~= 2, exatamente a razao teorica).

Justificativa alternativa: numero esperado de inversoes numa permutacao
aleatoria de n elementos = C(n,2)/2 = n(n-1)/4, e cada deslocamento do
Insertion Sort remove exatamente uma inversao -> mesmo resultado.

### Resumo

| Caso       | Operacoes         | Ordem | Tempo medido (n=1.000.000) |
|------------|-------------------|-------|------------------------------|
| Melhor     | n-1               | O(n)  | 0,001 s |
| Medio      | n(n-1)/2          | O(n^2)| 69,7 s |
| Pior       | n(n-1)            | O(n^2)| 139,3 s |

Feche relacionando com o grafico em escala log do capitulo: decrescente e
random formam retas praticamente paralelas (crescimento quadratico, com
random sempre um fator constante abaixo de decrescente), enquanto crescente
fica bem abaixo das outras duas (crescimento linear).
