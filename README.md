# Tradutor BPL (Bruno's Programming Language) para Assembly AMD64
Repositório criado para implementação do trabalho que compõe a nota final da disciplina de Software Básico do curso de Ciência da Computação da UFG.

<!-- ### Rafael
- [x] Implementar o assembly de início da chamada das funções.

### Iury
- [x] Implementar regra para diferenciar tipos de variáveis locais (var, vet e reg);
- [x] Implementar o assembly do armazenamento dos registradores de parâmetros na pilha (no caso de call);
- [x] Implementar o assembly do armazenamento de variáveis inteiras na pilha;
- [x] Implementar o assembly do armazenamento de arrays na pilha;
- [x] Implementar o assembly de acesso à arrays;
- [x] Implementar o assembly de condicional;
- [x] Implementar o assembly final (retorno) de execução das funções e condicionais.

### Vinicius
- [x] Implementar o assembly do armazenamento das variáveis de registradores na pilha (no caso de call);
- [x] Implementar a regra e assembly para chamada de funções (call);
- [x] Implementar o assembly de retorno das funções;
- [x] Implementar a atribuição de parametros em caso de call;
- [x] Implementar o assembly de atribuições e expressões. -->

## Introdução
O trabalho propõe a criação de um tradutor de BPL para Assembly AMD64. O enunciado do projeto está na pasta "especificacoes" que contém 2 pdf's das versões do trabalho.

## Desenvolvimento
O projeto foi modularizado em tradutor.c, main.c, tradutor.h e uma pasta de testes.

## Compilação
O tradutor deve ler um arquivo em BPL da entrada padrão (e.g., usando o scanf) e imprimir a tradução desse programa em Assembly na saída padrão (e.g., usando o printf). Pode-se criar um arquivo com a linguagem e utilizar o redirecionamento para testar:

` gcc -o tradutor main.c tradutor.c `
` ./tradutor < prog.bpl `

## Desenvolvedores
- [Iury Alexandre Alves Bo](https://github.com/iuryalvez)
- [Rafael Nunes Moreira Costa](https://github.com/rafaelnmcosta)
- [Vinicius de Freitas Castro](https://github.com/vinifcastro)
