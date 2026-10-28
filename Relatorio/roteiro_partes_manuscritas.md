# Roteiro das partes manuscritas do Merge Sort (Capítulo 5)

Os capítulos 1 a 4 (Insertion, Selection, Bubble e Shell Sort) já têm teste de mesa e análise feitos à mão, vindos do relatório da Entrega 2. **Faltam só duas imagens, as do Merge Sort:**

| Imagem | Arquivo a substituir (mesmo nome) |
|---|---|
| Teste de mesa manual | `Relatorio/Imagens/Merge Sort/teste_de_mesa.png` |
| Análise de complexidade (melhor, médio e pior caso) | `Relatorio/Imagens/Merge Sort/analise_complexidade.png` |

Hoje as duas são *placeholders* cinzas ("SUBSTITUA ESTA IMAGEM"). Escreva à mão (caneta escura, letra legível), tire foto ou escaneie, salve como PNG com **exatamente** esses nomes, substitua e recompile (`pdflatex Relatório_Corpo.tex`, duas ou três vezes). Nenhum `.tex` precisa mudar: o relatório já apresenta as figuras e já traz, digitado, o resumo da análise.

Tudo abaixo foi conferido executando o código real do Merge Sort passo a passo.

Sobre o algoritmo: é o **top-down recursivo**, com vetor auxiliar e `meio = inicio + (fim - inicio)/2` (divisão inteira). Quando o trecho tem tamanho ímpar, a metade esquerda fica com um elemento a mais. A intercalação usa `<=`, o que o torna **estável**.

---

## 1. Teste de mesa manual

Vetor de **7 elementos** (tamanho ímpar, para mostrar divisões desiguais), diferente dos exemplos de sala:

```
Vetor inicial: [26, 9, 41, 3, 18, 33, 12]      (índices 0 a 6)
```

### 1.1 Divisões (fase "dividir"; recursão em profundidade, esquerda antes da direita)

| Ordem | Chamada [início..fim] | meio | Metade esquerda | Metade direita |
|---|---|---|---|---|
| 1 | [0..6] | 3 | [26, 9, 41, 3] | [18, 33, 12] |
| 2 | [0..3] | 1 | [26, 9] | [41, 3] |
| 3 | [0..1] | 0 | [26] | [9] |
| 4 | [2..3] | 2 | [41] | [3] |
| 5 | [4..6] | 5 | [18, 33] | [12] |
| 6 | [4..5] | 4 | [18] | [33] |

Chamadas com `inicio >= fim` (um único elemento) retornam na hora: são as 7 folhas, [0], [1], …, [6]. Desenhe a **árvore de recursão** (raiz [0..6], folhas de um elemento): são 4 níveis, pois ⌈log₂ 7⌉ + 1 = 4.

### 1.2 Intercalações (fase "combinar"; na ordem em que acontecem ao voltar da recursão)

| Ordem | Trecho [início..fim] | Esquerda + Direita | Resultado | Comparações | Vetor completo depois |
|---|---|---|---|---|---|
| 1 | [0..1] | [26] + [9] | [9, 26] | 1 | [9, 26, 41, 3, 18, 33, 12] |
| 2 | [2..3] | [41] + [3] | [3, 41] | 1 | [9, 26, 3, 41, 18, 33, 12] |
| 3 | [0..3] | [9, 26] + [3, 41] | [3, 9, 26, 41] | 3 | [3, 9, 26, 41, 18, 33, 12] |
| 4 | [4..5] | [18] + [33] | [18, 33] | 1 | [3, 9, 26, 41, 18, 33, 12] |
| 5 | [4..6] | [18, 33] + [12] | [12, 18, 33] | 1 | [3, 9, 26, 41, 12, 18, 33] |
| 6 | [0..6] | [3, 9, 26, 41] + [12, 18, 33] | [3, 9, 12, 18, 26, 33, 41] | 6 | [3, 9, 12, 18, 26, 33, 41] |

Total: **6 intercalações e 13 comparações**; 40 cópias (cada intercalação de m elementos copia m para o auxiliar e m de volta: 2·(2+2+4+2+3+7) = 40).

### 1.3 Uma intercalação em detalhe (a última, ordem 6): [3, 9, 26, 41] + [12, 18, 33]

`i` percorre a esquerda, `j` a direita (índices relativos aos subvetores) e `k` é a posição no auxiliar.

| i | j | k | Comparação `vetor[i] <= vetor[j]` | Copia para `aux[k]` |
|---|---|---|---|---|
| 0 | 0 | 0 | 3 ≤ 12? sim | 3 (da esquerda) |
| 1 | 0 | 1 | 9 ≤ 12? sim | 9 (da esquerda) |
| 2 | 0 | 2 | 26 ≤ 12? não | 12 (da direita) |
| 2 | 1 | 3 | 26 ≤ 18? não | 18 (da direita) |
| 2 | 2 | 4 | 26 ≤ 33? sim | 26 (da esquerda) |
| 3 | 2 | 5 | 41 ≤ 33? não | 33 (da direita) |
| 3 | 3 | 6 | a direita acabou: copia o resto da esquerda | 41 |

Depois, `aux[0..6]` é copiado de volta para o vetor. Vetor final: `[3, 9, 12, 18, 26, 33, 41]`.

**Ao transcrever:** faça a árvore de divisões, a tabela de intercalações e, ao lado, **uma** intercalação em detalhe (a 6 ou a 3). Mostre que o `while` principal acaba quando uma das metades esgota e que o resto da outra é copiado direto.

---

## 2. Análise de complexidade (melhor, médio e pior caso)

Siga o mesmo formato dos capítulos anteriores: contagem de execuções de cada linha com uma constante de custo cᵢ, e depois a conclusão por caso. Como o Merge Sort é recursivo, a contagem de linhas vale para a intercalação, e a função recursiva fica com uma **recorrência**.

### 2.1 Custo da intercalação (`Intercalar`, trecho de m = fim − inicio + 1 elementos)

Seja p o número de iterações do primeiro laço (= número de comparações). Quando uma metade se esgota, restam s elementos na esquerda e t na direita. Cada elemento é copiado uma vez para o auxiliar, logo **p + s + t = m**.

| Linha | Código | Custo | Execuções |
|---|---|---|---|
| 1 | `int i = inicio, j = meio + 1, k = inicio;` | c₁ | 1 |
| 2 | `while (i <= meio && j <= fim)` | c₂ | p + 1 |
| 3 | `if (vetor[i] <= vetor[j])` | c₃ | p |
| 4 | `aux[k++] = vetor[i++];` ou `aux[k++] = vetor[j++];` | c₄ | p |
| 5 | `while (i <= meio)` | c₅ | s + 1 |
| 6 | `aux[k++] = vetor[i++];` | c₆ | s |
| 7 | `while (j <= fim)` | c₇ | t + 1 |
| 8 | `aux[k++] = vetor[j++];` | c₈ | t |
| 9 | `for (k = inicio; k <= fim; k++)` | c₉ | m + 1 |
| 10 | `vetor[k] = aux[k];` | c₁₀ | m |

Somando:

**T_I(m) = (c₂+c₃+c₄)·p + (c₅+c₆)·s + (c₇+c₈)·t + (c₉+c₁₀)·m + K**, com K = c₁ + c₂ + c₅ + c₇ + c₉ (constante).

Como p + s + t = m e todos os coeficientes são positivos, **T_I(m) = Θ(m) em qualquer entrada**: o que muda de um caso para outro é só como os m elementos se dividem entre p, s e t.

### 2.2 Recorrência e solução

`MergeSortRecursivo` faz um teste, o cálculo do meio, duas chamadas sobre metades e uma intercalação:

**T(n) = 2·T(n/2) + T_I(n) + c = 2·T(n/2) + Θ(n)**, com T(1) = Θ(1).

- **Árvore de recursão:** o nível k tem 2ᵏ subproblemas de tamanho n/2ᵏ, e o custo de cada nível é 2ᵏ · Θ(n/2ᵏ) = Θ(n). Há log₂ n + 1 níveis. Total: Θ(n) · (log₂ n + 1) = **Θ(n log n)**.
- **Teorema Mestre (confere):** a = 2, b = 2, f(n) = Θ(n) = Θ(n^{log_b a}) = Θ(n¹). É o **caso 2**, logo T(n) = Θ(n log n) (CORMEN et al., 2009).

### 2.3 Os três casos

- **Melhor caso (entrada crescente ou decrescente):** em cada intercalação, todos os elementos de uma metade são menores que os da outra. O primeiro laço termina após cerca de m/2 comparações (p ≈ m/2) e o resto é copiado direto. Comparações no total: ≈ (n/2)·log₂ n. T(n) = Θ(n log n).
- **Pior caso:** as duas metades se alternam em toda intercalação, e p = m − 1. Comparações no total: n·log₂ n − n + 1 (para n potência de 2). T(n) = Θ(n log n), só com constante maior. Nenhuma das três entradas testadas é exatamente esse caso.
- **Caso médio (entrada randômica):** as metades se entrelaçam aleatoriamente e, quando uma se esgota, restam em média poucos elementos da outra. Logo p fica perto de m − 1 e o número de comparações fica perto do limite do pior caso. T(n) = Θ(n log n).

**Conclusão a escrever:** melhor, médio e pior caso são todos **Θ(n log n)**. O número de comparações fica sempre entre ½·n·log₂ n e n·log₂ n (SEDGEWICK; WAYNE, 2011); as cópias são sempre ≈ 2·n·log₂ n.

**Exemplo numérico para n = 8 (confira à mão):**

| Entrada | Comparações | Cópias |
|---|---|---|
| [1, 2, 3, 4, 5, 6, 7, 8] (crescente) | 12 = (8/2)·log₂ 8 | 48 = 2·8·log₂ 8 |
| [8, 7, 6, 5, 4, 3, 2, 1] (decrescente) | 12 | 48 |
| [1, 5, 3, 7, 2, 6, 4, 8] (pior caso: metades alternadas em todos os níveis) | 17 = 8·log₂ 8 − 8 + 1 | 48 |

**Memória e estabilidade:** vetor auxiliar de tamanho n, ou seja, **O(n) de memória adicional** (≈ 4 MB para 1.000.000 de inteiros), mais a pilha de recursão com profundidade ≈ log₂ n (≈ 20 níveis para 1.000.000), desprezível. Os outros quatro algoritmos ordenam no próprio vetor. Como a intercalação usa `<=` e copia primeiro o elemento da esquerda em caso de empate, o algoritmo é **estável**.

### 2.4 Resultados medidos (Entrega 3), para fechar a análise

| Caso | Comparações (n = 1.000.000, versão instrumentada) | Ordem | n = 100.000 | n = 1.000.000 |
|---|---|---|---|---|
| Melhor (crescente) | 10.066.432 | Θ(n log n) | 0,002070 s | 0,021293 s |
| Melhor (decrescente) | 9.884.992 | Θ(n log n) | 0,001809 s | 0,020753 s |
| Médio (randômica) | 18.673.990 | Θ(n log n) | 0,006542 s | 0,077718 s |

Limites teóricos para n = 1.000.000: (n/2)·log₂ n ≈ 9.965.784 e n·log₂ n − n + 1 ≈ 18.931.570. As pequenas diferenças nos dois primeiros valores aparecem porque 1.000.000 não é potência de 2. Cópias: 39.902.848 nos três casos.

**Pontos para citar:**

1. De 100.000 para 1.000.000 de elementos (10 vezes) o tempo foi multiplicado por **10,3 / 11,5 / 11,9** (crescente / decrescente / randômica), perto de 12,0, o fator esperado para n log n. Um algoritmo quadrático daria 100.
2. A randômica foi cerca de 3,7 vezes mais lenta que as ordenadas, apesar da mesma complexidade. A contagem de operações não explica tudo; uma explicação **plausível, não verificada**, é a previsão de desvios do processador. Escreva como hipótese.
3. Em 1.000.000 de elementos randômicos o Merge Sort (0,078 s) foi o mais rápido dos cinco; nas entradas ordenadas, o Shell Sort (0,008 s e 0,012 s) e o Insertion e o Bubble (lineares na crescente) foram mais rápidos que ele (0,021 s).

---

## Checklist final

- [ ] Teste de mesa do Merge Sort feito à mão e salvo como `Imagens/Merge Sort/teste_de_mesa.png`.
- [ ] Análise de complexidade do Merge Sort feita à mão e salva como `Imagens/Merge Sort/analise_complexidade.png`.
- [ ] PDF recompilado e conferido página a página.
- [ ] Relatório em PDF no Moodle; pasta de código (`Entrega_3/`) no Drive. Entrega em 27/10/2026, com entrevista em sala.
