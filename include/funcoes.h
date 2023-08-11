#ifndef FUNCOES_H
#define FUNCOES_H

#include "padrao.h"
#include "registradores.h"
#include <stdbool.h>

// representa uma variávei local do tipo var
typedef struct Var{
    int ind; // índice da variável
    int pos; // posição na pilha da variável
} Var;

// representa uma variávei local do tipo vet
typedef struct vet {
    int ind; // índice da variável
    int pos; // posição na pilha da variável
    int size; // size do vet
} Vet;

// representa uma variávei local do tipo reg
typedef struct reg {
    int ind; // índice da variável
    int pos; // posição na pilha da variável
} Reg;

// representa um parâmetro da declaração de função
typedef struct param {
    char tipo;
    int pos;
} Param;

// representa todas as variáveis de declaração e locais que podem entrar na pilha
typedef struct pilha {
    Param param[3]; // variável que guarda os parâmetros da função 
    Var var[4];     // vetor que guarda as vars
    Vet vet[4];     // vetor que guarda os arrays
    Reg reg[4];     // vetor que guarda os registradores
    int var_qtd;    // quantidade de var
    int vet_qtd;    // quantidade de vet
    int reg_qtd;    // quantidade de reg
    int param_qtd;  // quantidade de parâmetros
    int rsp;        // registra o ponto atual da pilha
} Pilha;

/**
 * @brief exibe o salvamento dos parâmetros da função na pilha antes de uma chamada de função
 * 
 * @param pilha carrega todas as variáveis importantes da função
 */
void salvar_parametros(Pilha pilha);

/**
 * @brief exibe a recuperação dos parâmetros da função na pilha após a chamada de função
 * 
 * @param pilha carrega todas as variáveis importantes da função
 */
void recuperar_parametros(Pilha pilha);

/**
 * @brief inicializa os valores da variável pilha
 * 
 * @param pilha carrega todas as variáveis importantes da função
 */
void inicializar_pilha(Pilha *pilha);

/**
 * @brief exibe o salvamento das variáveis locais de pilha
 * 
 * @param pilha carrega todas as variáveis importantes da função
 */
void armazenar_pilha(Pilha pilha);

/**
 * @brief exibe a recuperação das variáveis locais de pilha
 * 
 * @param pilha carrega todas as variáveis importantes da função
 */
void recuperar_pilha(Pilha pilha);

/**
 * @brief alinha o tamanho de acordo com o alinhamento
 * 
 * @param tamanho ponteiro para variável que será alinhada
 * @param alinhamento múltiplo de alinhamento
 */
void alinhar(int *tamanho, int alinhamento);

#endif
