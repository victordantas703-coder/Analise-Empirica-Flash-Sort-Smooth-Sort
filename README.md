# Analise-Empirica-Flash-Sort-Smooth-Sort

![Git Hub](https://img.shields.io/badge/github-repository-blue?logo=github)
![EDBII-UFRN](https://img.shields.io/badge/EDB_II-UFRN-blue.svg)
![C++](https://img.shields.io/badge/-C++-blue?logo=cplusplus)

## Sumário

- [Introdução](#introdução)
- [Compilação](#compilação)
- [Uso do Programa de Análise Empírica](#uso-do-programa-de-análise-empírica)
- [Integrantes](#integrantes)

## Introdução

Trabalho de análise empírica dos algorítmos Flash Sort e Smooth Sort referente à disciplina de Estrutura de Dados Básicas II do curso de BTI/UFRN.

## Compilação

__1.__ Execute

```shell
git clone https://github.com/victordantas703-coder/Analise-Empirica-Flash-Sort-Smooth-Sort.git
```

ou baixe o arquivo compactado do projeto [por aqui](https://github.com/victordantas703-coder/Analise-Empirica-Flash-Sort-Smooth-Sort/archive/refs/heads/main.zip) e extraia a pasta contida nele.

__2.__  Execute

Windows


```shell
cd .\Analise-Empirica-Flash-Sort-Smooth-Sort\
g++ .\src\main.cpp .\src\keyGenerator.cpp -o analise_empirica
```

Linux / macOS


```shell
cd /Analise-Empirica-Flash-Sort-Smooth-Sort
g++ /src/main.cpp /src/keyGenerator.cpp -o analise_empirica
```

ou caso tenha o `make` na máquina, basta executar

Windows

```shell
cd .\Analise-Empirica-Flash-Sort-Smooth-Sort\
make
```

Linux / macOS

```shell
cd /Analise-Empirica-Flash-Sort-Smooth-Sort
make
```

para abrir a pasta do projeto e compilá-lo.

## Uso do Programa de Análise Empírica

Execute o programa a partir do terminal usando o comando abaixo.

Windows

```shell
.\analise_empirica.exe
```

Linux / macOS

```shell
./analise_empirica
```

Será executado 320 simulações dos algorítmos Flash Sort e Smooth Sort.

Ao final do experimento, será mostrado o tempo de execução médio dos algorítmos na ordenação de vetores aleatórios de tamanho $n \in \{2^{12}, 2^{13}, 2^{14},..., 2^{19}, 2^{20}\}$.

## Integrantes

Gabriel Pinheiro Maia

Pedro Fidalgo

Victor Matheus de Abreu Dantas

Vitor Cavalcante Borges 
