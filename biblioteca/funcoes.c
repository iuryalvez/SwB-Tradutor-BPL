#include "../include/funcoes.h"
 
void salvar_parametros(int nro_param, int *tampilha, Registrador r[MAX_REG]) {
    int i; // variável de iteração
    int ind_param = 5; // índice de parâmetro

    alinhar_tampilha(tampilha, 8);

    printf("\n    # %d parametro(s) a ser(em) salvo(s)\n", nro_param-1);
    printf("    subq $%d, %%rsp\n", (nro_param-1) * 8);
    for (i = 0; i < nro_param-1; i++) {
        *tampilha += 8;
        printf("    # salvando %dº parametro na pilha\n", i+1);
        printf("    movq    %%%s, -%d(%%rbp)\n", r[ind_param].nome64, *tampilha);
        ind_param--;
    }
}

void recuperar_parametros(int nro_param, int *tampilha, Registrador r[MAX_REG]) {
    
    int i; // variável de iteração
    int ind_param = 7-nro_param; // índice de parâmetro

    printf("\n    # %d parametro(s) a ser(em) recuperado(s)\n", nro_param-1);
    for (i = nro_param-1; i > 0; i--) {
        printf("    # recuperando %dº parametro na pilha\n", i);
        printf("    movq    -%d(%%rbp), %%%s\n", *tampilha, r[ind_param].nome64);
        *tampilha -= 8;
        ind_param++;
    }
    
    printf("    addq    $%d, %%rsp\n", (nro_param-1) * 8);
    
}

void iniciar_vlp(vl vlp[8]) {
    for (int i = 0; i < 8; i++) {
        vlp[i].indice = -1;
    }
}

void armazenar_vlp(vl vlp[8], int qtd_vlp, int tampilha) {
    int i;
    printf("    subq    %d, %%rsp\n", tampilha);
    for (i = 0; i < qtd_vlp; i++, tampilha -= vlp[i].pos_pilha) { 
        if ((vlp[i].pos_pilha-1) % 8 == 7) printf("    movq    $0, -%d(%%rbp)\n", vlp[i].pos_pilha);
        else printf("    movl    $0, -%d(%%rbp)\n", vlp[i].pos_pilha);
    }
}

void armazenar_vr(int ind_reg[4], int qtd_reg, Registrador r[MAX_REG]) {
    int i;
    int ind;
    printf("\n");
    for (i = 0, ind = 8; i < qtd_reg; i++, ind++) {
        printf("    # vr%d = 0\n", ind_reg[i]); // comentário de linha
        printf("    movq    $0, %%%s\n\n", r[ind].nome64); // rcx
    }

}

void alinhar_tampilha(int *tampilha, int alinhamento) {
    while (*tampilha % alinhamento != 0) {
        *tampilha += 4;
    }
}
