#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Gera os dados do relatorio (CSVs, tabelas LaTeX e graficos) A PARTIR DOS
ARQUIVOS DE TEMPO da entrega, para que o relatorio sempre bata com os
arquivos que o professor vai abrir.

Uso (de dentro da pasta Relatorio/):
    python3 gerar_dados_relatorio.py                 # usa ../Entrega_3
    python3 gerar_dados_relatorio.py ../Entrega_2    # outra entrega

Requer: Python 3 e matplotlib.

Gera:
    tabela_tempos_<algoritmo>.csv         (e tabela_tempos.csv = Insertion Sort)
    Tabelas/tab_<algoritmo>.tex           (tabela de cada algoritmo)
    Tabelas/comparacao_<tipo>.tex         (tabela de comparacao dos algoritmos)
    Imagens/<Algoritmo>/grafico.png       (escala linear)
    Imagens/<Algoritmo>/grafico_log.png   (escala logaritmica)
    Imagens/Geral/grafico_geral_<tipo>[_log].png

Atencao: os textos do relatorio que citam numeros (ex.: "cerca de 167,0
segundos") sao escritos a mao; se refizer as medicoes, revise-os.
"""
import os
import sys

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

AQUI = os.path.dirname(os.path.abspath(__file__))
ENTREGA = os.path.abspath(sys.argv[1]) if len(sys.argv) > 1 else os.path.join(AQUI, "..", "Entrega_3")

TAMANHOS = [10, 100, 1000, 10000, 100000, 1000000]
TIPOS = ["Crescente", "Decrescente", "Random"]

# (pasta, slug, rotulo em "tab:tempos_<rotulo>", cor no grafico geral)
ALGORITMOS = [
    ("Insertion Sort", "insertion_sort", "insertion", "tab:blue"),
    ("Selection Sort", "selection_sort", "selection", "tab:green"),
    ("Bubble Sort", "bubble_sort", "bubble", "tab:red"),
    ("Shell Sort", "shell_sort", "shell", "tab:orange"),
    ("Merge Sort", "merge_sort", "merge", "tab:purple"),
]
COR_TIPO = {"Crescente": "tab:blue", "Decrescente": "tab:red", "Random": "tab:orange"}


def ler_tempos():
    """dados[pasta][tipo][tamanho] = texto exato gravado no arquivo de tempo."""
    dados = {}
    for pasta, _, _, _ in ALGORITMOS:
        dados[pasta] = {}
        for tipo in TIPOS:
            dados[pasta][tipo] = {}
            for n in TAMANHOS:
                caminho = os.path.join(ENTREGA, pasta, "Arquivos de Tempo", tipo, "Tempo%s%d.txt" % (tipo, n))
                with open(caminho) as f:
                    linhas = f.read().split()
                if len(linhas) != 2 or int(linhas[0]) != n:
                    raise SystemExit("Arquivo de tempo com formato inesperado: " + caminho)
                float(linhas[1])  # confere que e numero
                dados[pasta][tipo][n] = linhas[1]
    return dados


def virgula(texto):
    return texto.replace(".", ",")


def milhar(n):
    return "{:,}".format(n).replace(",", ".")


def escrever(caminho, conteudo):
    os.makedirs(os.path.dirname(caminho), exist_ok=True)
    with open(caminho, "w", encoding="utf-8", newline="\n") as f:
        f.write(conteudo)


def cabecalho_tabela():
    return ("        \\hline\n         & " + " & ".join(milhar(n) for n in TAMANHOS) + " \\\\\n        \\hline\n")


def gerar_csvs(dados):
    for pasta, slug, _, _ in ALGORITMOS:
        linhas = ["Tamanho," + ",".join(TIPOS)]
        for n in TAMANHOS:
            linhas.append(str(n) + "," + ",".join(dados[pasta][t][n] for t in TIPOS))
        texto = "\n".join(linhas) + "\n"
        escrever(os.path.join(AQUI, "tabela_tempos_%s.csv" % slug), texto)
        if slug == "insertion_sort":  # arquivo legado da Entrega 1
            escrever(os.path.join(AQUI, "tabela_tempos.csv"), texto)


def gerar_tabelas_individuais(dados):
    for pasta, slug, rotulo, _ in ALGORITMOS:
        t = "\\begin{table}[htbp]\n    \\centering\n    \\begin{tabular}{|l|r|r|r|r|r|r|}\n"
        t += cabecalho_tabela()
        for tipo in TIPOS:
            t += "        %s & %s \\\\\n        \\hline\n" % (tipo, " & ".join(virgula(dados[pasta][tipo][n]) for n in TAMANHOS))
        t += "    \\end{tabular}\n"
        t += "    \\caption{Tempo de execução (em segundos) do %s por tamanho e tipo de instância}\n" % pasta
        t += "    \\label{tab:tempos_%s}\n\\end{table}\n" % rotulo
        escrever(os.path.join(AQUI, "Tabelas", "tab_%s.tex" % slug), t)


def gerar_tabelas_comparacao(dados):
    for tipo in TIPOS:
        t = "\\begin{table}[htbp]\n    \\centering\n    \\begin{tabular}{|l|r|r|r|r|r|r|}\n"
        t += cabecalho_tabela()
        for pasta, _, _, _ in ALGORITMOS:
            t += "        %s & %s \\\\\n        \\hline\n" % (pasta, " & ".join(virgula(dados[pasta][tipo][n]) for n in TAMANHOS))
        t += "    \\end{tabular}\n"
        t += "    \\caption{Tempo de execução (em segundos) dos %d algoritmos -- entrada %s}\n" % (len(ALGORITMOS), tipo)
        t += "    \\label{tab:comparacao_%s}\n\\end{table}\n" % tipo.lower()
        escrever(os.path.join(AQUI, "Tabelas", "comparacao_%s.tex" % tipo.lower()), t)


def valores(serie_texto, log):
    """Converte para float; na escala log, zeros (abaixo da resolucao do relogio) viram NaN."""
    v = [float(serie_texto[n]) for n in TAMANHOS]
    if log:
        v = [x if x > 0 else float("nan") for x in v]
    return v


def estilo(ax, titulo, log):
    ax.set_xscale("log")
    if log:
        ax.set_yscale("log")
    ax.set_title(titulo)
    ax.set_xlabel("Tamanho da entrada")
    ax.set_ylabel("Tempo em segundos")
    ax.grid(True, which="both", linestyle="--", alpha=0.4)
    ax.legend()


def gerar_graficos_individuais(dados):
    for pasta, _, _, _ in ALGORITMOS:
        for log in (False, True):
            fig, ax = plt.subplots(figsize=(8, 5))
            for tipo in TIPOS:
                ax.plot(TAMANHOS, valores(dados[pasta][tipo], log), marker="o", color=COR_TIPO[tipo], label=tipo)
            estilo(ax, "Gráfico do %s" % pasta, log)
            fig.tight_layout()
            saida = os.path.join(AQUI, "Imagens", pasta, "grafico_log.png" if log else "grafico.png")
            os.makedirs(os.path.dirname(saida), exist_ok=True)
            fig.savefig(saida, dpi=150)
            plt.close(fig)


def gerar_graficos_gerais(dados):
    for tipo in ("Crescente", "Decrescente"):
        for log in (False, True):
            fig, ax = plt.subplots(figsize=(8, 5))
            for pasta, _, _, cor in ALGORITMOS:
                ax.plot(TAMANHOS, valores(dados[pasta][tipo], log), marker="o", color=cor, label=pasta)
            estilo(ax, "Gráfico Geral -- entrada %s" % tipo, log)
            fig.tight_layout()
            nome = "grafico_geral_%s%s.png" % (tipo.lower(), "_log" if log else "")
            saida = os.path.join(AQUI, "Imagens", "Geral", nome)
            os.makedirs(os.path.dirname(saida), exist_ok=True)
            fig.savefig(saida, dpi=150)
            plt.close(fig)


def resumo(dados):
    print("Fonte dos dados:", ENTREGA)
    zeros = []
    for pasta, _, _, _ in ALGORITMOS:
        for tipo in TIPOS:
            for n in TAMANHOS:
                if float(dados[pasta][tipo][n]) == 0:
                    zeros.append("%s / %s / n=%d" % (pasta, tipo, n))
    print("Valores iguais a zero (omitidos nos graficos em escala log): %d" % len(zeros))
    for z in zeros:
        print("   ", z)


if __name__ == "__main__":
    dados = ler_tempos()
    gerar_csvs(dados)
    gerar_tabelas_individuais(dados)
    gerar_tabelas_comparacao(dados)
    gerar_graficos_individuais(dados)
    gerar_graficos_gerais(dados)
    resumo(dados)
    print("Pronto: CSVs, Tabelas/, Imagens/<Algoritmo>/ e Imagens/Geral/ atualizados.")
