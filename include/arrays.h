#ifndef ARRAYS_H
#define ARRAYS_H

#include "funcoes.h"

// Get ou Set
typedef struct gs {
    char op;
    char tipo1;
    char tipo2[2];
    int ind1;
    int ind2;
    int size;
} GS;

int encontrar_indvet(Pilha pilha, int ind);

int encontrar_indvar(Pilha pilha, int ind);

int encontrar_indreg(Pilha pilha, int ind);

#endif