# Tradutor BPL (Bruno's Programming Language) para Assembly AMD64
Repositório criado para implementação do trabalho que compõe a nota final da disciplina de Software Básico do curso de Ciência da Computação da UFG.

## Tasks de desenvolvimento (temporário)
- [ ] Implementar regra para diferenciar tipos de variáveis locais (var, vet e reg)
- [ ] Implementar o assembly do armazenamento de variáveis inteiras na pilha
- [ ] Implementar o assembly do armazenamento de arrays na pilha 
- [ ] Implementar o assembly da atribuição de registradores
- [ ] Implementar o assembly final da chamada de funções (recuperação de regist. da pilha e retorno)

### Rafael
- [ ] Implementar o assembly de início da chamada das funções de acordo com parâmetros (armazenamento de regist. na pilha)
### Iury

### Vinicius

## Introdução
O trabalho propõe a criação de um tradutor de BPL para Assembly AMD64. O enunciado do projeto está na pasta "especificacoes" que contém 2 pdf's das versões do trabalho.

## Desenvolvimento
O projeto foi modularizado em "biblioteca", "include" e "tradutor.c".<br>
1. A pasta **include** é responsável pela declaração das funções utilizadas na implementação do projeto;
2. A pasta **biblioteca** é responsável pela implementação das funções da pasta "include";
3. O arquivo **tradutor.c** é o arquivo principal de execução do programa.

## Compilação
O tradutor deve ler um arquivo em BPL da entrada padrão (e.g., usando o scanf) e imprimir a tradução desse programa em Assembly na saída padrão (e.g., usando o printf). Pode-se criar um arquivo com a linguagem e utilizar o redirecionamento para testar:

` ./tradutor < prog.bpl `

## Desenvolvedores
- [Iury Alexandre Alves Bo](https://github.com/iuryalvez)
- [Rafael Nunes Moreira Costa](https://github.com/rafaelnmcosta)
- [Vinicius de Freitas Castro](https://github.com/vinifcastro)
