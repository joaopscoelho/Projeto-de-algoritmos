# Roteiro das partes manuscritas do Quick Sort (Capítulo 6)

Os capítulos 1 a 5 já têm teste de mesa e análise feitos à mão. **Faltam só duas imagens, as do Quick Sort:**

| Imagem | Arquivo a substituir (mesmo nome) |
|---|---|
| Teste de mesa manual | `Relatorio/Imagens/Quick Sort/teste_de_mesa.png` |
| Análise de complexidade (melhor, médio e pior caso) | `Relatorio/Imagens/Quick Sort/analise_complexidade.png` |

Hoje as duas são *placeholders* cinzas ("SUBSTITUA ESTA IMAGEM"). Escreva à mão (caneta escura, letra legível), tire foto ou escaneie, salve como PNG com **exatamente** esses nomes, substitua e me peça para recompilar o PDF (o LaTeX do seu computador não tem o pacote de português do `babel`). Nenhum `.tex` precisa mudar: o relatório já apresenta as duas figuras e já traz, digitado, o resumo da análise.

Tudo abaixo foi conferido executando o código real das duas versões do Quick Sort passo a passo.

---

## 1. Teste de mesa manual (folha 1)

**Vetor escolhido (6 elementos, diferente dos exemplos de sala):**

```
Vetor inicial: [29, 45, 14, 52, 8, 36]      (índices 0 a 5)
Vetor final:   [8, 14, 29, 36, 45, 52]
```

O teste é da **versão 1** (pivô = primeiro elemento), que é a versão pedida como "algoritmo normal apresentado em sala". No pé da folha entra a primeira partição da **versão 2**, para mostrar a diferença da escolha do pivô.

São só **4 partições e 9 comparações** — e, de brinde, esse vetor passa pelos três tipos de divisão que existem no algoritmo: uma equilibrada (partição 1), uma em que o pivô é o **maior** do trecho e a direita fica vazia (partição 3), e uma em que ele é o **menor** e a esquerda fica vazia (partição 4). As duas últimas são exatamente o que acontece o tempo todo no pior caso, então dá para apontar para elas na hora da análise.

Lembrete do algoritmo: `pivo = vetor[inicio]`; `i` guarda a última posição ocupada por um elemento **menor ou igual** ao pivô; `j` varre de `inicio+1` até `fim`; no fim, troca-se `vetor[inicio]` com `vetor[i]` e o pivô fica na posição definitiva `i`.

### 1.1 Partição 1 — trecho [0..5], pivô = 29

| j | vetor[j] | vetor[j] ≤ 29? | Ação | Vetor depois |
|---|---|---|---|---|
| 1 | 45 | não | — | [29, 45, 14, 52, 8, 36] |
| 2 | 14 | sim | i = 1; troca v[1] ↔ v[2] | [29, **14**, **45**, 52, 8, 36] |
| 3 | 52 | não | — | [29, 14, 45, 52, 8, 36] |
| 4 | 8 | sim | i = 2; troca v[2] ↔ v[4] | [29, 14, **8**, 52, **45**, 36] |
| 5 | 36 | não | — | [29, 14, 8, 52, 45, 36] |

**Troca final:** v[0] ↔ v[2] → **[8, 14, `29`, 52, 45, 36]** — o pivô **29 fica fixo na posição 2**.
Sobram: esquerda **[0..1] = {8, 14}** e direita **[3..5] = {52, 45, 36}**. *5 comparações.*

### 1.2 Partição 2 — trecho [0..1] = [8, 14], pivô = 8

| j | vetor[j] | ≤ 8? | Ação |
|---|---|---|---|
| 1 | 14 | não | — |

**Troca final:** v[0] ↔ v[0] (não muda nada) → pivô **8 na posição 0**. *1 comparação.*
Esquerda **vazia**; direita = um só elemento (já ordenado). Vetor: **[8, 14, 29, 52, 45, 36]**.

### 1.3 Partição 3 — trecho [3..5] = [52, 45, 36], pivô = 52

| j | vetor[j] | ≤ 52? | Ação | Vetor depois |
|---|---|---|---|---|
| 4 | 45 | sim | i = 4; troca v[4] ↔ v[4] (não muda nada) | [8, 14, 29, 52, 45, 36] |
| 5 | 36 | sim | i = 5; troca v[5] ↔ v[5] (não muda nada) | [8, 14, 29, 52, 45, 36] |

**Troca final:** v[3] ↔ v[5] → **[8, 14, 29, 36, 45, `52`]** — pivô **52 na posição 5**. *2 comparações.*
Aqui o pivô era o **maior** do trecho: a partição **direita saiu vazia** e a esquerda ficou com os 2 elementos restantes. Sobra **[3..4] = {36, 45}**.

### 1.4 Partição 4 — trecho [3..4] = [36, 45], pivô = 36

| j | vetor[j] | ≤ 36? | Ação |
|---|---|---|---|
| 4 | 45 | não | — |

**Troca final:** v[3] ↔ v[3] (não muda nada) → pivô **36 na posição 3**. *1 comparação.*
Agora o pivô era o **menor** do trecho: a **esquerda saiu vazia**. Sobra um só elemento à direita, já ordenado.

**Vetor final: [8, 14, 29, 36, 45, 52]** ✔

### 1.5 Árvore de recursão (desenhe ao lado das tabelas)

```
                  [0..5]  pivô 29
                 /                \
        [0..1] pivô 8          [3..5] pivô 52
         /       \              /          \
    (vazio)   [1..1]     [3..4] pivô 36   (vazio)
                           /        \
                      (vazio)     [4..4]
```

**Totais: 4 partições, 9 comparações (5 + 1 + 2 + 1) e 8 chamadas de troca — das quais só 4 mudam o vetor de fato.**

Vale escrever uma linha no pé da tabela: *para n = 6, o melhor caso faria 8 comparações e o pior caso faria 6·5/2 = 15; este exemplo, com 9, ficou perto do melhor caso porque o primeiro pivô (29) caiu quase no meio do vetor.*

### 1.6 Comparação com a versão 2 (pivô = média) — só a primeira partição

Mesmo vetor `[29, 45, 14, 52, 8, 36]`.

**Pivô = média:** soma = 29+45+14+52+8+36 = **184**; m = 6; 184 / 6 = 30,666… → **média = 30** (divisão inteira).

A varredura é de Hoare: `i` anda para a direita enquanto `vetor[i] < 30`, `j` anda para a esquerda enquanto `vetor[j] > 30`, e os dois que travam são trocados.

| Passo | i | j | O que acontece | Vetor depois |
|---|---|---|---|---|
| 1 | 0 → 1 | 5 → 4 | v[0]=29 < 30 → i=1 (trava em 45); v[5]=36 > 30 → j=4 (trava em 8) | [29, 45, 14, 52, 8, 36] |
| 2 | — | — | i ≤ j: troca v[1] ↔ v[4]; depois i = 2, j = 3 | [29, **8**, 14, 52, **45**, 36] |
| 3 | 2 → 3 | 3 → 2 | v[2]=14 < 30 → i=3 (trava em 52); v[3]=52 > 30 → j=2 (trava em 14) | [29, 8, 14, 52, 45, 36] |
| 4 | 3 | 2 | i > j → fim da partição | — |

Resultado: **[29, 8, 14 | 52, 45, 36]** — trechos **[0..2]** (todos ≤ 30) e **[3..5]** (todos > 30), ou seja, **3 + 3**, perfeitamente equilibrado, contra **2 + 3** da versão 1.

### 1.7 Onde as duas versões realmente se separam (mini-exemplo, 2 linhas)

Vale fechar a folha com este exemplo curto, que é o ponto principal do capítulo. Use o vetor **já ordenado [1, 2, 3, 4]**:

- **Versão 1:** pivô = 1 (o primeiro, que é o menor) → nenhum elemento ≤ pivô → **esquerda vazia**, direita = {2, 3, 4}. Na seguinte, pivô = 2 → esquerda vazia de novo, e assim por diante: **n níveis de recursão → pior caso Θ(n²)**.
- **Versão 2:** soma = 10, m = 4, média = **2** → a partição divide em **[1] e [3, 4]** (o 2 já fica no lugar): partições equilibradas → **Θ(n log n)**.

---

## 2. Análise de complexidade (folha 2)

**Formato (igual ao das outras folhas):** uma página, com o custo `c_i` por linha no cabeçalho, a recorrência, e depois um bloco por caso ("Melhor caso", "Pior caso", "Caso médio"), cada um com o título sublinhado, contas curtas e a conclusão em forma de T(n) = …. O texto digitado do relatório (`Analise/quick_sort.tex`) segue exatamente esta ordem.

### 2.1 Cabeçalho: custo da partição (`ParticionaVersao1`)

Seja **m = fim − inicio + 1** o número de elementos do trecho e **k** o número de elementos (fora o pivô) menores ou iguais ao pivô, com **0 ≤ k ≤ m − 1**.

| Linha | Código | Custo | Execuções |
|---|---|---|---|
| 1 | `int pivo = vetor[inicio]; int i = inicio;` | c₁ | 1 |
| 2 | `for (j = inicio+1; j <= fim; j++)` | c₂ | m |
| 3 | `if (vetor[j] <= pivo)` | c₃ | m − 1 |
| 4 | `i++; trocar(&vetor[i], &vetor[j]);` | c₄ | k |
| 5 | `trocar(&vetor[inicio], &vetor[i]);` | c₅ | 1 |
| 6 | `return i;` | c₆ | 1 |

**T_P(m) = c₂·m + c₃·(m−1) + c₄·k + (c₁ + c₅ + c₆)**

Como **0 ≤ k ≤ m−1**, tanto a cota inferior como a superior são lineares em m:

**T_P(m) = Θ(m)** → *a partição tem tempo linear, qualquer que seja a ordem dos dados.*
São exatamente **m − 1 comparações** e no máximo **m trocas**. (Isto responde também o **segundo resultado** do enunciado: cada elemento é visitado uma única vez, pois `j` só avança e nunca retrocede.)

### 2.2 Recorrência (vale para os três casos)

Se o pivô termina na posição p, sobra um trecho de **k = p − inicio** elementos à esquerda e **n − 1 − k** à direita (o pivô não entra em nenhuma chamada seguinte):

**T(n) = T(k) + T(n − 1 − k) + Θ(n)**,  com T(0) = T(1) = Θ(1)

O que separa os três casos é **o valor de k**, que depende dos dados — diferente do Merge Sort, em que a divisão é sempre exatamente ao meio.

### 2.3 Os três blocos da folha

**Melhor caso — k = ⌊(n−1)/2⌋ (pivô = mediana):**
as duas partições ficam iguais → **T(n) = 2·T(n/2) + Θ(n)** → mesma recorrência do Merge Sort.
Árvore com log₂ n níveis, cada nível custando Θ(n):
**T(n) = Θ(n log n)**, com ≈ n·log₂ n comparações.
Confere com o **Teorema Mestre, caso 2** (a = 2, b = 2, f(n) = Θ(n)); (CORMEN et al., 2009).

**Pior caso — k = 0 ou k = n − 1 (pivô é o menor ou o maior):**
uma partição fica vazia e a outra com n − 1 elementos → **T(n) = T(n−1) + Θ(n)**.
Somando: **T(n) = Σ (m = 2 até n) Θ(m) = Θ(n²)**, com exatamente
**Σ (m = 2 até n) (m − 1) = n(n−1)/2 comparações**.
Na **versão 1** isso acontece na **entrada crescente** (o primeiro elemento é sempre o menor → k = 0) e na **decrescente** (é sempre o maior → k = n−1): *para a versão 1, dados já ordenados são o pior caso, e não o melhor.*

**Caso médio — k uniforme entre 0 e n−1 (entrada randômica):**
**T(n) = (1/n)·Σ (k = 0 até n−1) [T(k) + T(n−1−k)] + Θ(n)**
Solução: **T(n) ≈ 2(n+1)·H_n − 4n ≈ 1,386·n·log₂ n = Θ(n log n)**
(H_n = n-ésimo número harmônico), ou seja, só ~39 % acima do melhor caso.

### 2.4 Fecho da folha (duas ou três linhas)

- **Versão 2 (pivô = média):** a média está sempre entre o menor e o maior elemento → **nunca produz partição vazia**; em entrada ordenada cai no meio do trecho → as entradas crescente e decrescente passam a ser o **melhor caso**, Θ(n log n). Custo extra: uma segunda passada pelo trecho para somar (dobra a constante, não muda a ordem).
- **Memória:** as duas versões ordenam **no próprio vetor** (O(1) de memória extra, fora a pilha de recursão), ao contrário do Merge Sort, que precisa de Θ(n).
- **Não há fase de combinação** (quarto resultado do enunciado): a partição já deixa o pivô na posição definitiva e todo elemento da esquerda ≤ todo elemento da direita; quando os dois lados estão ordenados, o vetor está ordenado.
- **Não é estável:** as trocas da partição podem inverter a ordem de elementos iguais.

---

## 3. Conferências numéricas (não precisam ir para a folha — servem para a entrevista)

**Contagem de operações (versão instrumentada, fora do código entregue):**

| Caso | n | Comparações | Confere com |
|---|---|---|---|
| v1 crescente e decrescente | 1.000 | 499.500 | n(n−1)/2 = 499.500 — **exato** |
| v1 crescente e decrescente | 10.000 | 49.995.000 | n(n−1)/2 = 49.995.000 — **exato** |
| v1 randômica | 1.000.000 | 24.149.675 | 2(n+1)H_n − 4n ≈ 24.785.500 (2,6 % de diferença) |
| v2 crescente | 1.000.000 | 19.000.019 | n·log₂ n ≈ 19.931.569 |
| v2 decrescente | 1.000.000 | 19.000.036 | idem |
| v2 randômica | 1.000.000 | 21.334.769 | idem |

Para n = 1.000.000, o pior caso da v1 faria **n(n−1)/2 = 499.999.500.000** comparações (meio trilhão), ou seja, **vinte mil vezes** mais que as 24 milhões da entrada randômica.

**Profundidade da recursão** (versão puramente recursiva, sem a eliminação de recursão de cauda do código entregue): **50.000 níveis para n = 50.000** nas entradas ordenadas, contra **40 níveis** na randômica — é por isso que o código entregue faz a chamada recursiva sempre na **menor** partição e trata a maior no próprio laço: sem isso, o pior caso com 1.000.000 de elementos estoura a pilha e o programa quebra antes de terminar. Com a eliminação, a profundidade medida foi de 2 níveis nas ordenadas e 14 na randômica, e as partições/comparações/trocas são exatamente as mesmas.

**Tempos medidos (segundos):**

| n | v1 Crescente | v1 Decrescente | v1 Random | v2 Crescente | v2 Decrescente | v2 Random |
|---|---|---|---|---|---|---|
| 10 | 0,000001 | 0,000000 | 0,000000 | 0,000004 | 0,000000 | 0,000000 |
| 100 | 0,000007 | 0,000003 | 0,000003 | 0,000007 | 0,000001 | 0,000007 |
| 1.000 | 0,000162 | 0,000236 | 0,000036 | 0,000017 | 0,000011 | 0,000075 |
| 10.000 | 0,013770 | 0,017875 | 0,000452 | 0,000137 | 0,000136 | 0,000837 |
| 100.000 | 1,346279 | 1,809272 | 0,005350 | 0,001684 | 0,001716 | 0,009652 |
| 1.000.000 | 154,233806 | 229,408053 | 0,063731 | 0,021151 | 0,022560 | 0,114290 |

**Pontos para a entrevista:**

1. De 10.000 para 100.000 elementos (10 vezes), o tempo da v1 nas entradas ordenadas foi multiplicado por **97,8 e 101,2** — o fator 100 do crescimento quadrático. De 100.000 para 1.000.000, por 114,6 e 126,8 (um pouco acima de 100 porque o vetor de 4 MB já não cabe na cache).
2. Na entrada randômica, de 100.000 para 1.000.000 o fator foi **11,9**, próximo de 12,0, o esperado para n log n. A v2 deu 12,6 / 13,1 / 11,8 nas três entradas — n log n em todas.
3. A v2 foi **7.300 vezes** mais rápida que a v1 na crescente e **10.200 vezes** na decrescente; em compensação, foi **1,8 vez mais lenta** na randômica, porque percorre o trecho duas vezes por partição (uma para somar a média, outra para trocar).
4. Se perguntarem por que o pior caso aparece em dados ordenados: porque o pivô é o primeiro elemento, que num vetor ordenado é sempre o extremo — a partição não divide nada.
5. **Ambiente:** os tempos do Quick Sort foram medidos em ambiente de nuvem, e não no seu computador, porque o pior caso de 1.000.000 leva vários minutos seguidos. Para calibrar, o Shell e o Merge Sort foram reexecutados lá: ficaram 1,29 e 1,74 vez mais lentos que no seu computador. Isso está dito no relatório, na seção *Tabela de Comparação*.

---

## Checklist final

- [ ] Teste de mesa do Quick Sort feito à mão e salvo como `Relatorio/Imagens/Quick Sort/teste_de_mesa.png`.
- [ ] Análise de complexidade do Quick Sort feita à mão e salva como `Relatorio/Imagens/Quick Sort/analise_complexidade.png`.
- [ ] PDF recompilado (me peça) e conferido página a página.
- [x] ~~Apagar os arquivos .tgz de transferência~~ (já apagados).
- [ ] Apagar ou recompilar `Entrega_4/main`: hoje é um executável **Linux**, não roda no Windows nem no macOS.
- [ ] Relatório em PDF no Moodle; pasta de código (`Entrega_4/`) no Drive. Entrega em 10/11/2026, com entrevista em sala sobre o Merge e o Quick Sort.
