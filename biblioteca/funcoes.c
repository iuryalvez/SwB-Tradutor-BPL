#include "../include/funcoes.h"

void inicializar_param(Param *param) {
    param->qtd = 0;
    param->tipo[0] = '\0';
    param->tipo[1] = '\0';
    param->tipo[2] = '\0';
    param->subq = 0;
}

void salvar_parametros(Param *param, int *rsp) {
    
    int i;
    int rsp0 = *rsp;  // salvando meu rsp original para poder descobrir o subq 
    int ind_rdi = 5; // índice do rdi

    Registrador r[MAX_REG];
    iniciar_registradores(r);
    
    for (i = 0; i < param->qtd; i++) {
        if (param->tipo[i] == 'i') { // se for do tipo inteiro
            *rsp += 4; // não precisa alinhar porque 4 é múltiplo de 4, 8, 16
            param->pos[i] = *rsp; // guarda a posição na pilha
        } else {
            alinhar_rsp(rsp, 8); // precisa alinhar para 8 (ponteiro) pq não sabemos se está alinhado
            *rsp += 8; // desce o topo da pilha para colocar mais uma variável
            param->pos[i] = *rsp; // guarda a posição na pilha
        }
    }

    param->subq = *rsp - rsp0; // rsp é negativo, rsp0 é negativo, rsp0 - rsp vai resultar num número positivo (pq rsp é maior que rsp0)
    alinhar_rsp(&(param->subq), 16);

    printf("\n    subq    $%d, %%rsp\n", param->subq); 

    // printf do assembly de armazenamento, vai utilizar as informações que acabamos de obter
    printf("\n    # %d parametro(s) a ser(em) salvo(s)\n", param->qtd); 
    for (i = 0; i < param->qtd; i++) {
        printf("\n    # salvando %dº parametro na pilha\n", i+1);
        if (param->tipo[i] == 'i') printf("    movl    %%%s, -%d(%%rbp)\n", r[ind_rdi-i].nome32, param->pos[i]);
        else printf("    movq    %%%s, -%d(%%rbp)\n", r[ind_rdi-i].nome64, param->pos[i]);
    }
}

void recuperar_parametros(Param *param, int *rsp) {
    int i;
    int ind_ultimo = 6 - param->qtd; // índice do último registrador 
    
    Registrador r[MAX_REG];
    iniciar_registradores(r);

    printf("\n    # %d parametro(s) a ser(em) recuperado(s)\n", param->qtd);
    for (i = param->qtd-1; i >= 0; i--) {
        printf("\n    # recuperando %dº parametro na pilha\n", i+1);
        if (param->tipo[i] == 'i') printf("    movl    -%d(%%rbp), %%%s\n", param->pos[i], r[ind_ultimo].nome32);
        else printf("    movq    -%d(%%rbp), %%%s\n", param->pos[i], r[ind_ultimo].nome64);
        ind_ultimo++;
    }
    
    printf("\n    addq    $%d, %%rsp\n", param->subq);
    *rsp -= param->subq;
    
}

void inicializar_vl(VL *vl) {
    int i;
    
    vl->var_qtd = 0;
    vl->reg_qtd = 0;
    vl->vet_qtd = 0;

    for (i = 0; i < 4; i++) {
        vl->var[i].ind = 0;
        vl->var[i].pos = 0;
        
        vl->vet[i].ind = 0;
        vl->vet[i].pos = 0;

        vl->reg[i].ind = 0;
        vl->reg[i].pos = 0;
    }
} 

void armazenar_vl(VL *vl) {
    int i; 
    Registrador r[MAX_REG];
    iniciar_registradores(r);
    // armazenar variáveis locais do tipo var
    if (vl->var_qtd) {
        for (i = 0; i < vl->var_qtd; i++) {
            printf("\n    # vi%d = 0\n", vl->var[i].ind);
            printf("    movl    $0, %d(%%rbp)\n", vl->var[i].pos);
        }
    }

    // armazenar variáveis locais do tipo vet
    if (vl->vet_qtd) {
        for (i = 0; i < vl->vet_qtd; i++) {
            printf("\n    # salvando endereco inicial do va%d\n", vl->vet[i].ind);
            printf("    movq    $0, -%d(%%rbp)\n", vl->vet[i].pos);
        }
    }

    // armazenar variáveis locais do tipo reg 
    // só irá armazenar a partir da segunda função
    // só irá armazenar os reg que a função utilizar (definidos no def enddef)
    if (vl->reg_qtd) {
        for (i = 0; i < vl->reg_qtd; i++) {
            printf("\n    # vr%d -> %%%s\n", vl->reg[i].ind, r[i+8].nome64);
            printf("    movq    %%%s, -%d(%%rbp)\n", r[i+8].nome64, vl->reg[i].pos);
        }
    }
}

void recuperar_vl(VL vl) {
    // recuperar variáveis locais do tipo reg 
    // só irá recuperar a partir da segunda função
    // só irá recuperar os reg que a função utilizar (definidos no def enddef)
    Registrador r[MAX_REG];
    iniciar_registradores(r);
    int i;
    if (vl.reg_qtd) {
        for (i = vl.reg_qtd-1; i >= 0; i--) {
            printf("    # devolvendo o valor do %%%s anterior\n", r[i+8].nome64);
            printf("    movq    -%d(%%rbp), %%%s\n", vl.reg[i].pos, r[i+8].nome64);
        }
    }
}

void alinhar_rsp(int *rsp, int alinhamento) {
    while (*rsp % alinhamento != 0) {
        *rsp += 4;
    }
}
