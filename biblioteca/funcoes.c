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
    printf("    subq    $%d, %%rsp\n", tampilha);
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

void reinicializa_parametros(int *indice_funcao,int *nro_param, char * tipo_param, int *tampilha, vl * vlp, int * ind_reg, int * cont_reg, int * cont_vlp){
    int i;
    // variáveis de declaração de função
    *indice_funcao = 0;     // índice da função
    *nro_param = 0;         // número de parâmetros da função
    tipo_param[0] = 0;
    tipo_param[1] = 0;
    tipo_param[2] = 0;

    // variáveis de controle e contagem da pilha
    *tampilha = 0; // tamanho da pilha da função

    // variáveis de controle da pilha
    for(i=0;i<8;i++){
        vlp[i].indice = 0;
        vlp[i].pos_pilha = 0;
        vlp[i].size = 0;
    }
    ind_reg[0] = 0;   // índice dos registradores
    ind_reg[1] = 0;
    ind_reg[2] = 0;
    ind_reg[3] = 0;
    *cont_reg = 0; // contador de registradores
    *cont_vlp = 0; // contador de variaveis locais de pilha
}

void printacontrole(int indice_funcao, int nro_param, char * tipo_param, int tampilha, vl * vlp, int * ind_reg, int cont_reg, int cont_vlp){
    int i;

    printf("indice funcao = %d\n", indice_funcao);
    printf("nro param = %d\n", nro_param);
    printf("tipo_param[0] = %d\n", tipo_param[0]);
    printf("tipo_param[1] = %d\n", tipo_param[1]);
    printf("tipo_param[2] = %d\n", tipo_param[2]);
    printf("tam pilha = %d\n", tampilha);

    for(i=0;i<8;i++){
        printf("vlp indice na posicao %d: %d\n", i, vlp[i].indice);
        printf("vlp pos_pilha na posicao %d: %d\n", i, vlp[i].pos_pilha);
        printf("vlp size na posicao %d: %d\n", i, vlp[i].size);
    }

    printf("ind_reg[0] = %d\n", ind_reg[0]);
    printf("ind_reg[1] = %d\n", ind_reg[1]);
    printf("ind_reg[2] = %d\n", ind_reg[2]);
    printf("ind_reg[3] = %d\n", ind_reg[3]);

    printf("cont_reg = %d\n", cont_reg);
    printf("cont_vlp = %d\n", cont_vlp);
}

// do 8 ao 11
void recuperar_reg(Registrador *r, int * tampilha, int * salvos){
    int i = salvos[4]-1;
    for(; i>0 ; i--){
        printf("    # recuperando r%d parametro na pilha\n", salvos[i]);
        printf("    movq    -%d(%%rbp), r%d\n",*tampilha, salvos[i]);
        *tampilha -= 8;
    }
    inicializasalvos(salvos);
}

void salvar_reg(Registrador *r, int * tampilha, int * salvos){
    int i;
    int cont = 0;
    

    for(i = 8; i <= 11 ; i++){
        
        if((r[i]. livre) == false) cont++;
        
    }
    
    printf("    subq $%d, %%rsp", multiplode16(cont*8));
    
    for(i = 8; i <= 11 ; i++){
        
        if(!(r[i]. livre)){
            *tampilha += 8;
            printf("    # salvando r%d parametro na pilha\n", i);
            printf("    movq    r%d, -%d(%%rbp)\n", i, *tampilha);
            salvos[salvos[4]] = i;
            salvos[4]++;
        }
        
    }
}

// Se receber um int multiplo de 8, retorna o multiplo de 16 mais proximo, acima dele. Se receber multiplo de 16, apenas retorna ele msm.
int multiplode16(int x){
    while((x%16) != 0) x += 16;
    return x;
}

void inicializasalvos(int * salvos){
    int i;
    for(i=0;i<4; i++){
        salvos[i] = -1;
    }
    salvos[4] = 0; // a posição 5 é a que armazena o indice da menor posição -1 do vetor;
}