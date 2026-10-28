#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Aplica, nos .tex JA EXISTENTES de Relatorio/, as edicoes da Entrega 3:
  - titulo da capa (inclui Merge Sort) e novas subsecoes "Merge Sort";
  - nomes das funcoes nas listagens (X_versao1 -> X, como no codigo entregue);
  - trechos de instrucao ("deve ser inserida...", "substitua a imagem...")
    viram texto final;
  - frases do Bubble e do Shell Sort que a nova rodada de tempos deixou
    desatualizadas;
  - copia os placeholders das imagens manuscritas para "Imagens/Merge Sort/".

Cada edicao so e aplicada se o trecho for encontrado EXATAMENTE o numero de
vezes esperado; caso contrario nada e alterado naquele ponto e o script avisa
("FALHA"). Pode ser rodado de novo sem duplicar nada ("ja").

Uso, de dentro da pasta Relatorio/:   python3 aplicar_entrega3.py
"""
import os
import re
import shutil

resultados = []


def ler(caminho):
    with open(caminho, encoding="utf-8") as f:
        return f.read()


def gravar(caminho, texto):
    with open(caminho, "w", encoding="utf-8", newline="\n") as f:
        f.write(texto)


def padrao_ws(texto):
    """Regex que casa o texto ignorando diferencas de espacos e quebras de linha."""
    return re.compile(r"\s+".join(re.escape(p) for p in texto.split()))


def registrar(status, arquivo, rotulo, n):
    resultados.append((status, arquivo, rotulo, n))


def trocar(arquivo, antigo, novo, esperado=1, rotulo=""):
    """Troca 'antigo' por 'novo' (ignorando quebras de linha) se houver 'esperado' ocorrencias."""
    t = ler(arquivo)
    rotulo = rotulo or antigo[:55]
    n = len(padrao_ws(antigo).findall(t))
    if n == esperado:
        gravar(arquivo, padrao_ws(antigo).sub(lambda m: novo, t))
        registrar("ok", arquivo, rotulo, n)
    elif n == 0 and padrao_ws(novo).search(t):
        registrar("ja", arquivo, rotulo, 0)
    else:
        registrar("FALHA", arquivo, rotulo, n)


def remover_versao1(arquivo):
    t = ler(arquivo)
    rx = re.compile(r"\\?_versao1")
    n = len(rx.findall(t))
    if n == 0:
        registrar("ja", arquivo, "sufixo _versao1 nas listagens", 0)
    elif n == 2:
        gravar(arquivo, rx.sub("", t))
        registrar("ok", arquivo, "sufixo _versao1 nas listagens", n)
    else:
        registrar("FALHA", arquivo, "sufixo _versao1 nas listagens", n)


def inserir_subsecoes_merge(arquivo):
    t = ler(arquivo)
    if "merge_sort" in t:
        registrar("ja", arquivo, "subsecoes Merge Sort", 0)
        return
    rx = re.compile(r"\\input\{(Algoritmos|Analise|Tabela&Grafico)/shell_sort\}")
    novo, n = rx.subn(
        lambda m: m.group(0) + "\n\t\t\\newpage\n\n\t\t\\subsection{Merge Sort}\n\t\t\\input{"
        + m.group(1) + "/merge_sort}", t)
    if n == 3:
        gravar(arquivo, novo)
        registrar("ok", arquivo, "subsecoes Merge Sort", n)
    else:
        registrar("FALHA", arquivo, "subsecoes Merge Sort", n)


# ------------------------------------------------------------------ arquivo principal
PRINCIPAL = "Relatório_Corpo.tex"
trocar(PRINCIPAL,
       "IMPLEMENTAÇÃO E ANÁLISE DOS ALGORITMOS INSERTION, SELECTION, BUBBLE E SHELL SORT",
       "IMPLEMENTAÇÃO E ANÁLISE DOS ALGORITMOS INSERTION, SELECTION, BUBBLE, SHELL E MERGE SORT",
       esperado=2, rotulo="titulo da capa (2 capas)")
inserir_subsecoes_merge(PRINCIPAL)

# ------------------------------------------------------------------ Algoritmos/*.tex
NOMES = {"insertion_sort": "Insertion Sort", "selection_sort": "Selection Sort",
         "bubble_sort": "Bubble Sort", "shell_sort": "Shell Sort"}
for slug, nome in NOMES.items():
    arq = "Algoritmos/%s.tex" % slug
    remover_versao1(arq)
    trocar(arq,
           "Nesta seção deve ser inserida a digitalização do teste de mesa manual do %s, "
           "realizado à mão com um exemplo de entrada diferente dos exemplos trabalhados "
           "em sala de aula, conforme exigido pelo enunciado do trabalho. Substitua a "
           "imagem de exemplo abaixo pela digitalização (foto ou escaneamento) do teste "
           "de mesa manuscrito." % nome,
           "O teste de mesa manual do %s, apresentado na figura abaixo, foi realizado à mão "
           "com um exemplo de entrada diferente dos exemplos trabalhados em sala de aula, "
           "conforme exigido pelo enunciado do trabalho." % nome,
           rotulo="texto de apresentacao do teste de mesa")

trocar("Algoritmos/bubble_sort.tex",
       "o que costuma torná-lo mais lento na prática, como discutido na análise de "
       "complexidade e evidenciado nos resultados experimentais deste capítulo.",
       "o que tende a torná-lo mais lento na prática, efeito que os resultados "
       "experimentais deste capítulo mostram com clareza no caso de entradas "
       "aleatórias, como discutido na análise de complexidade.",
       rotulo="frase sobre o Bubble ser mais lento")

# ------------------------------------------------------------------ Analise/*.tex
for slug in ("insertion_sort", "selection_sort", "bubble_sort"):
    trocar("Analise/%s.tex" % slug,
           "conforme exigido pelo enunciado do trabalho. A digitalização do material "
           "manuscrito deve ser inserida a seguir, substituindo a imagem de exemplo abaixo.",
           "conforme exigido pelo enunciado do trabalho, e é apresentada a seguir.",
           rotulo="texto de apresentacao da analise")

trocar("Analise/shell_sort.tex",
       "cujo conteúdo manuscrito deve ser inserido a seguir, substituindo a imagem de "
       "exemplo abaixo.",
       "cujo conteúdo manuscrito é apresentado a seguir.",
       rotulo="texto de apresentacao da pesquisa")

trocar("Analise/shell_sort.tex",
       "Ainda assim, mesmo utilizando a sequência mais simples possível, o Shell Sort "
       "superou folgadamente os demais algoritmos deste trabalho em todos os tipos de "
       "entrada testados, como mostram os resultados da próxima seção.",
       "Ainda assim, mesmo utilizando a sequência mais simples possível, o Shell Sort "
       "superou folgadamente o Insertion, o Selection e o Bubble Sort em todos os tipos "
       "de entrada testados, como mostram os resultados da próxima seção; a comparação "
       "com o Merge Sort é feita na Tabela de Comparação e na Conclusão.",
       rotulo="frase sobre o Shell superar os demais")

trocar("Analise/bubble_sort.tex",
       "Vale destacar que, apesar de Insertion Sort e Bubble Sort compartilharem a mesma "
       "ordem de grandeza no pior caso, o Bubble Sort tende a ser mais lento na prática, "
       "pois cada troca custa três atribuições, contra apenas uma atribuição por "
       "deslocamento no Insertion Sort — o que também é confirmado pelos tempos medidos.",
       "Vale destacar que, apesar de Insertion Sort e Bubble Sort compartilharem a mesma "
       "ordem de grandeza no pior caso, cada troca do Bubble Sort custa três atribuições, "
       "contra apenas uma atribuição por deslocamento no Insertion Sort, o que tende a "
       "torná-lo mais lento na prática. Os tempos medidos, porém, mostram que o custo de "
       "cada instrução não é o único fator: na entrada randômica o Bubble Sort foi cerca "
       "de 11,6 vezes mais lento que o Insertion Sort (938,7 s contra 80,6 s para "
       "1.000.000 de elementos), enquanto na entrada decrescente os dois tiveram "
       "desempenho equivalente (155,6 s contra 167,0 s). Na entrada randômica, parte da "
       "diferença vem do número de comparações (cerca de $n(n-1)/2$ no Bubble Sort contra "
       "$n(n-1)/4$ no Insertion Sort); o restante provavelmente decorre da previsão de "
       "desvios do processador: na entrada decrescente a condição "
       "\\texttt{vetor[j] > vetor[j+1]} é sempre verdadeira, um padrão que o processador "
       "prevê quase perfeitamente, ao passo que na entrada randômica ela é imprevisível e "
       "causa perdas de desempenho do \\textit{pipeline}. Essa é uma explicação plausível, "
       "mas não foi verificada neste trabalho.",
       rotulo="paragrafo final da analise do Bubble")

# ------------------------------------------------------------------ imagens manuscritas (placeholders)
destino = os.path.join("Imagens", "Merge Sort")
os.makedirs(destino, exist_ok=True)
for nome in ("teste_de_mesa.png", "analise_complexidade.png"):
    alvo = os.path.join(destino, nome)
    if os.path.exists(alvo):
        registrar("ja", alvo, "placeholder da imagem manuscrita", 0)
    else:
        shutil.copyfile(os.path.join("Imagens", "Insertion Sort", nome), alvo)
        registrar("ok", alvo, "placeholder copiado", 1)

# ------------------------------------------------------------------ resumo
print("%-6s %-34s %s" % ("status", "arquivo", "edicao (ocorrencias)"))
for status, arq, rot, n in resultados:
    print("%-6s %-34s %s (%d)" % (status, arq, rot, n))
falhas = [r for r in resultados if r[0] == "FALHA"]
print("\n%d edicoes ok/ja aplicadas, %d FALHA(S)." % (len(resultados) - len(falhas), len(falhas)))
raise SystemExit(1 if falhas else 0)
