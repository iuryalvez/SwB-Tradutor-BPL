#include "../include/funcoes.h"

void inicializar_pilha(Pilha *pilha) {
    int i;
    
    pilha->var_qtd = 0;
    pilha->reg_qtd = 0;
    pilha->vet_qtd = 0;

    for (i = 0; i < 4; i++) {
        pilha->var[i].ind = 0;
        pilha->var[i].pos = 0;
        
        pilha->vet[i].ind = 0;
        pilha->vet[i].pos = 0;

        pilha->reg[i].ind = 0;
        pilha->reg[i].pos = 0;
    }

    pilha->param_qtd = 0;
    pilha->param[0].tipo = '\0';
    pilha->param[1].tipo = '\0';
    pilha->param[2].tipo = '\0';

    pilha->rsp = 0;
} 

void armazenar_pilha(Pilha *pilha) {
    int i;
    int ind = 5;

    Registrador r[MAX_REG];
    iniciar_registradores(r);
    
    printf("\n    # pilha:\n");
    
    // armazenar variáveis locais do tipo var
    if (pilha->var_qtd) {
        for (i = 0; i < pilha->var_qtd; i++) {
            pilha->rsp += 4;
            pilha->var[i].pos = pilha->rsp;
            printf("    # vi%d: -%d\n", pilha->var[i].ind, pilha->var[i].pos);
        }
    }
    
    // armazenar variáveis locais do tipo vet
    if (pilha->vet_qtd) {
        for (i = 0; i < pilha->vet_qtd; i++) {
            // alinhar(&(pilha->rsp), 8);
            pilha->rsp += 4 * pilha->vet[i].size;
            pilha->vet[i].pos = pilha->rsp;
            printf("    # va%d: -%d\n", pilha->vet[i].ind, pilha->vet[i].pos);
        }
    }

    // armazenar variáveis locais do tipo reg 
    // só irá armazenar a partir da segunda função
    // só irá armazenar os reg que a função utilizar (definidos no def enddef)
    if (pilha->reg_qtd) {
        for (i = 0; i < pilha->reg_qtd; i++) {
            alinhar(&(pilha->rsp), 8);
            pilha->rsp += 8;
            pilha->reg[i].pos = pilha->rsp;
            printf("    # vr%d: -%d\n", pilha->reg[i].ind, pilha->reg[i].pos);
        }
    }

    // armazenando a posição na pilha das variáveis de declaração de função 
    if (pilha->param_qtd) {
        for (int i = 0; i < pilha->param_qtd; i++) {
            if (pilha->param[i].tipo == 'i') { // se for do tipo inteiro
                pilha->rsp += 4; // não precisa alinhar porque 4 é múltiplo de 4, 8, 16
                pilha->param[i].pos = pilha->rsp; // guarda a posição na pilha
                printf("    # pi%d: -%d\n", i+1, pilha->param[i].pos);
            } else {
                alinhar(&(pilha->rsp), 8); // precisa alinhar para 8 (ponteiro) pq não sabemos se está alinhado
                pilha->rsp += 8; // desce o topo da pilha para colocar mais uma variável
                pilha->param[i].pos = pilha->rsp; // guarda a posição na pilha
                printf("    # pa%d: -%d\n", i+1, pilha->param[i].pos);
            }
        }
    }

    alinhar(&(pilha->rsp), 16);
}

void print_armazenamento(Pilha pilha) {
    int i;

    Registrador r[MAX_REG];
    iniciar_registradores(r);
    
    printf("\n    # armazenar todas as variáveis\n");
    printf("    subq    $%d, %%rsp\n", pilha.rsp);

    // armazenar variáveis locais do tipo var
    if (pilha.var_qtd) {
        for (i = 0; i < pilha.var_qtd; i++) {
            printf("\n    # vi%d = 0\n", pilha.var[i].ind);
            printf("    movl    $0, -%d(%%rbp)\n", pilha.var[i].pos);
        }
    }
    
    // armazenar variáveis locais do tipo vet
    if (pilha.vet_qtd) {
        for (i = 0; i < pilha.vet_qtd; i++) {
            printf("\n    # salvando endereco inicial do va%d\n", pilha.vet[i].ind);
            printf("    movq    $0, -%d(%%rbp)\n", pilha.vet[i].pos);
        }
    }

    // armazenar variáveis locais do tipo reg 
    // só irá armazenar a partir da segunda função
    // só irá armazenar os reg que a função utilizar (definidos no def enddef)
    if (pilha.reg_qtd) {
        for (i = 0; i < pilha.reg_qtd; i++) {
            printf("\n    # vr%d -> %%%s\n", pilha.reg[i].ind, r[i+12].nome64);
            printf("    movq    %%%s, -%d(%%rbp)\n", r[i+12].nome64, pilha.reg[i].pos);
        }
    }
}

void print_recuperacao(Pilha pilha) {
    // recuperar variáveis locais do tipo reg 
    // só irá recuperar a partir da segunda função
    // só irá recuperar os reg que a função utilizar (definidos no def enddef)
    Registrador r[MAX_REG];
    iniciar_registradores(r);
    int i;
    if (pilha.reg_qtd) {
        for (i = pilha.reg_qtd-1; i >= 0; i--) {
            printf("\n    # devolvendo o valor do %%%s anterior\n", r[i+12].nome64);
            printf("    movq    -%d(%%rbp), %%%s\n", pilha.reg[i].pos, r[i+12].nome64);
        }
    }
}

void salvar_parametros(Pilha pilha) {
    int i;

    Registrador r[MAX_REG];
    iniciar_registradores(r);
    
    // printf do assembly de armazenamento, vai utilizar as informações que acabamos de obter
    printf("\n    # %d parametro(s) a ser(em) salvo(s)\n", pilha.param_qtd); 
    for (i = 0; i < pilha.param_qtd; i++) {
        printf("\n    # salvando %dº parametro na pilha\n", i+1);
        if (pilha.param[i].tipo == 'i') printf("    movl    %%%s, -%d(%%rbp)\n", r[5-i].nome32, pilha.param[i].pos);
        else printf("    movq    %%%s, -%d(%%rbp)\n", r[5-i].nome64, pilha.param[i].pos);
    }
}

void recuperar_parametros(Pilha pilha) {
    int i;
    
    Registrador r[MAX_REG];
    iniciar_registradores(r);

    printf("\n    # %d parametro(s) a ser(em) recuperado(s)\n", pilha.param_qtd);
    for (i = pilha.param_qtd-1; i >= 0; i--) {
        printf("\n    # recuperando %dº parametro na pilha\n", i+1);
        if (pilha.param[i].tipo == 'i') printf("    movl    -%d(%%rbp), %%%s\n", pilha.param[i].pos, r[5-i].nome32);
        else printf("    movq    -%d(%%rbp), %%%s\n", pilha.param[i].pos, r[5-i].nome64);
    }
}

void alinhar(int *tamanho, int alinhamento) {
    while (*tamanho % alinhamento != 0) {
        *tamanho += 4;
    }
}

void inicializar_parameters(Typecharint * p){
    int i;

    //printf("\n\n==================================================================================================\n\n");
    for(i=0; i<4 ; i++){
        p[i].index = 0;
        p[i].type = 0;
        p[i].x = 0;

        if(i==0) p[i].x = 'v';
       // printf("Index = %d, Type = %c, X = %c.\n", p[i].index, p[i].type, p[i].x);
    }
    //printf("\n\n==================================================================================================\n\n");

}

void callfuncao(char * s){
    printf("\n    call %s\n", s);
}