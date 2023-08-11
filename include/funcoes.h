#ifndef FUNCOES_H
#define FUNCOES_H

#include "padrao.h"
#include "registradores.h"
#include <stdbool.h>

typedef struct Var{
    int ind; // índice da variável
    int pos; // posição na pilha da variável
} Var;

typedef struct vet {
    int ind; // índice da variável
    int pos; // posição na pilha da variável
    int size; // size do vet
} Vet;

typedef struct reg {
    int ind; // índice da variável
    int pos; // posição na pilha da variável
} Reg;

typedef struct VL {
    Var var[4]; // vetor que guarda as vars
    Vet vet[4]; // vetor que guarda os arrays
    Reg reg[4]; // vetor que guarda os registradores
    int var_qtd; // quantidade de var
    int vet_qtd; // quantidade de vet
    int reg_qtd; // quantidade de reg
} VL;

typedef struct param {
    int qtd;
    char tipo[3];
    int pos[3];
    int subq;
} Param;

void inicializar_param(Param *param);

void salvar_parametros(Param *param, int *rsp);

void recuperar_parametros(Param *param, int *rsp);

/**
 * @brief inicializa os valores da struct de variáveis locais com 0
 * 
 * @param vl ponteiro para a struct de variáveis locais
 */
void inicializar_vl(VL *vl);

void armazenar_vl(VL *vl);

void recuperar_vl(VL vl);

void alinhar_rsp(int *rsp, int alinhamento);

#endif
