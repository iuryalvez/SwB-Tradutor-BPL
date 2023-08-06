#include "../include/funcoes.h"
 
void salvar_parametros(int nro_param, int *tampilha, Registrador r[16]) {
    int i; // variável de iteração
    int ind_param = 5; // índice de parâmetro
    int alinhamento = 8; // alinhamento padrão de registradores

    alinhar_tampilha(tampilha, 8);

    printf("    # %d parametros a serem salvos\n", nro_param-1);
    printf("    subq $%d, %%rsp", (nro_param-1) * 8);
    for (i = 0; i < nro_param; i++) {
        printf("    # salvando %dº parametro na pilha\n", i+1);
        printf("    movq    %%%s, -%d(%%rbp)\n", r[ind_param].nome64, (alinhamento*i)+(*tampilha));
        *tampilha += 8;
        ind_param--;
    }
    
}

void recuperar_parametros(int nro_param, int *tampilha, Registrador r[16]) {
    
    int i; // variável de iteração
    int ind_param = 5; // índice de parâmetro

    printf("    # %d parametros a serem recuperados\n", nro_param-1);
    for (i = nro_param-1; i > 0; i--) {
        printf("    # recuperando %dº parametro na pilha\n", i+1);
        printf("    movq    -%d(%%rbp), %%%s\n", (i*8)+(*tampilha), r[ind_param].nome64);
        *tampilha -= 8;
        ind_param--;
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
    for (i = 0; i < qtd_vlp; i++) { 
        if ((vlp[i].pos_pilha-1) % 8 == 7) { // então é múltiplo de 8 
            printf("    movq    $0, -%d(%%rbp)\n", vlp[i].pos_pilha);
            tampilha -= vlp[i].pos_pilha;
        }
        else {
            printf("    movl  $0, -%d(%%rbp)\n", vlp[i].pos_pilha);
            tampilha -= vlp[i].pos_pilha;
        }
    }
}

void armazenar_reg(int ind_reg[4], int qtd_reg) {

    // código de armazenar registradores aqui

}

void alinhar_tampilha(int *tampilha, int alinhamento) {
    while (*tampilha % alinhamento != 0) {
        *tampilha += 4;
    }
}
