#include "../include/arrays.h"

int encontrar_indvet(Pilha pilha, int ind) {
    int i;
    for (i = 0; i < pilha.vet_qtd; i++) {
        if (pilha.vet[i].ind == ind) return i;
    }
}

int encontrar_indvar(Pilha pilha, int ind) {
    int i;
    for (i = 0; i < pilha.var_qtd; i++) {
        if (pilha.var[i].ind == ind) return i;
    }
}

int encontrar_indreg(Pilha pilha, int ind) {
    int i;
    for (i = 0; i < pilha.reg_qtd; i++) {
        if (pilha.reg[i].ind == ind) return i;
    }
}
