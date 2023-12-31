#include "tradutor.h"

void remover_newline(char *ptr) {
	while (*ptr) {
		if (*ptr == '\n') *ptr = 0;
		else ptr++;
	}
}

int ler_linha(char * linha) {
	if (fgets(linha, TAM_LINHA, stdin) != NULL) {
        remover_newline(linha);
        return TRUE;
    }
    return FALSE;
}

void iniciar_r(Reg_param r[3]) {
    strcpy(r[0].nome32, "edi");
    strcpy(r[0].nome64, "rdi");
    
    strcpy(r[1].nome32, "esi");
    strcpy(r[1].nome64, "rsi");

    strcpy(r[2].nome32, "edx");
    strcpy(r[2].nome64, "rdx");

}

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

    Reg_param r[3];
    iniciar_r(r);
    
    if (pilha->param_qtd || pilha->var_qtd || pilha->vet_qtd || pilha->reg_qtd) printf("\n    # pilha:\n");
    
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
            alinhar(pilha->rsp, 8);
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

    Reg_param r[3];
    iniciar_r(r);
    
    if (pilha.param_qtd || pilha.var_qtd || pilha.vet_qtd || pilha.reg_qtd) {
        printf("\n    # espaco para armazenamento (topo deve ser multiplo de 16)\n");
        printf("    subq    $%d, %%rsp\n", pilha.rsp);
    } else printf("\n    # nao ha o que guardar na pilha");

    // armazenar variáveis locais do tipo reg 
    // só irá armazenar a partir da segunda função
    // só irá armazenar os reg que a função utilizar (definidos no def enddef)
    if (pilha.reg_qtd) {
        for (i = 0; i < pilha.reg_qtd; i++) {
            printf("\n    # vr%d -> %%r%dd\n", pilha.reg[i].ind, i+12);
            printf("    movl    %%r%d, -%d(%%rbp)\n", i+12, pilha.reg[i].pos);
        }
    }
}

void print_recuperacao(Pilha pilha) {
    
    // recuperar variáveis locais do tipo reg 
    // só irá recuperar a partir da segunda função
    // só irá recuperar os reg que a função utilizar (definidos no def enddef)
    Reg_param r[3];
    iniciar_r(r);
    int i;
    if (pilha.reg_qtd) {
        for (i = pilha.reg_qtd-1; i >= 0; i--) {
            printf("\n    # devolvendo o valor do %%r%d anterior\n", i+12);
            printf("    movl    -%d(%%rbp), %%r%dd\n", pilha.reg[i].pos, i+12);
        }
    }
}

void salvar_parametros(Pilha pilha) {
    int i;

    Reg_param r[3];
    iniciar_r(r);
    
    // printf do assembly de armazenamento, vai utilizar as informações que acabamos de obter
    printf("\n    # %d parametro(s) a ser(em) salvo(s)\n", pilha.param_qtd); 
    for (i = 0; i < pilha.param_qtd; i++) {
        printf("\n    # salvando %dº parametro na pilha\n", i+1);
        if (pilha.param[i].tipo == 'i') printf("    movl    %%%s, -%d(%%rbp)\n", r[i].nome32, pilha.param[i].pos);
        else printf("    movq    %%%s, -%d(%%rbp)\n", r[i].nome64, pilha.param[i].pos);
    }
}

void recuperar_parametros(Pilha pilha) {
    int i;
    
    Reg_param r[3];
    iniciar_r(r);

    printf("\n    # %d parametro(s) a ser(em) recuperado(s)\n", pilha.param_qtd);
    for (i = pilha.param_qtd-1; i >= 0; i--) {
        printf("\n    # recuperando %dº parametro na pilha\n", i+1);
        if (pilha.param[i].tipo == 'i') printf("    movl    -%d(%%rbp), %%%s\n", pilha.param[i].pos, r[i].nome32);
        else printf("    movq    -%d(%%rbp), %%%s\n", pilha.param[i].pos, r[i].nome64);
    }
}

void alinhar(int *tamanho, int alinhamento) {
    while (*tamanho % alinhamento != 0) {
        *tamanho += 4;
    }
}

void inicializar_parameters(Typecharint * p){
    int i;

    for(i=0; i<4 ; i++){
        p[i].index = 0;
        p[i].type = 0;
        p[i].x = 0;
        if(i==0) p[i].x = 'v';
    }
}

void callfuncao(char * s){
    printf("\n    call %s\n", s);
}

void atribui_call(Pilha pilha, Typecharint * parameters, int numero_args, char ** registers_param){
    int i;
    int k;
    int pos;

    for(i=1; i<=numero_args ; i++){
        switch(parameters[i].x){
            case 'p':
                pos = parameters[i].index-1;
                if(parameters[i].type == 'a'){
                    printf("    movq    -%d(%%rbp), %%r%s # r%s = %c%c%d\n", pilha.param[pos].pos, registers_param[i-1], registers_param[i-1], parameters[i].x, parameters[i].type, parameters[i].index);
                }
                else{
                    printf("    movl    -%d(%%rbp), %%e%s # e%s = %c%c%d\n", pilha.param[pos].pos, registers_param[i-1], registers_param[i-1], parameters[i].x, parameters[i].type, parameters[i].index);
                }
                break;
            case 'v':
                if( parameters[i].type == 'a' ){
                    // printf("\na\n");
                    for(k=0; k<pilha.vet_qtd;k++){
                        if(pilha.vet[k].ind == parameters[i].index)
                            printf("    leaq    -%d(%%rbp), %%r%s # r%s = %c%c%d\n", pilha.vet[k].pos, registers_param[i-1], registers_param[i-1], parameters[i].x, parameters[i].type, parameters[i].index);
                    }
                }
                else if( parameters[i].type == 'r' ){
                    for(k=0; k<pilha.reg_qtd;k++){
                        if(pilha.reg[k].ind == parameters[i].index)
                            printf("    movl    %%r%dd, %%e%s # e%s = %c%c%d\n", 12+k, registers_param[i-1], registers_param[i-1], parameters[i].x, parameters[i].type, parameters[i].index);
                    }
                }
                else {
                    for(k=0; k<pilha.var_qtd;k++){
                        if(pilha.var[k].ind == parameters[i].index)
                            printf("    movl    -%d(%%rbp), %%e%s # e%s = %c%c%d\n", pilha.var[k].pos, registers_param[i-1], registers_param[i-1], parameters[i].x, parameters[i].type, parameters[i].index);
                    }
                }
                break;
            case 'c':
                printf("    movl    $%d, %%e%s # e%s = %d\n", parameters[i].index, registers_param[i-1], registers_param[i-1], parameters[i].index);
                break;
            default:
                break;
        }
    }
}


void expressions(Pilha pilha, char operator, Typecharint * parameters, int qtd, char ** registers_param){
    int pos;
    int i;
    int pos1, pos2, pos3;
    int isAtribute3, isAtribute2, isAtribute1;

    if(qtd == 1){
        
        switch(parameters[0].type){
            case 'i':
                for(i=0; i<pilha.var_qtd;i++)
                    if(parameters[0].index == pilha.var[i].ind) 
                        pos = pilha.var[i].pos;

                switch(parameters[1].x){
                    case 'v':
                        if( parameters[1].type == 'i' ){
                            for(i=0; i<pilha.var_qtd;i++)
                                if(parameters[1].index == pilha.var[i].ind) 
                                    printf("    movl    -%d(%%rbp), %%r8d # r8d = %c%c%d\n", pilha.var[i].pos, parameters[1].x, parameters[1].type, parameters[1].index);
                            printf("    movl    %%r8d, -%d(%%rbp) # %c%c%d = r8d\n", pos, parameters[0].x, parameters[0].type, parameters[0].index);
                        }
                        else{
                            for(i=0; i<pilha.reg_qtd;i++)
                                if(parameters[1].index == pilha.reg[i].ind) 
                                    printf("    movl    %%r%dd, %%r8d # r8d = %c%c%d\n", 12+i, parameters[1].x, parameters[1].type, parameters[1].index);
                            printf("    movl    %%r8d, -%d(%%rbp) # %c%c%d = r8d\n", pos, parameters[0].x, parameters[0].type, parameters[0].index);
                        }
                        break;
                    case 'p':
                        for(i=0; i<pilha.param_qtd;i++)
                            if(parameters[1].index == i+1) 
                                printf("    movl    %%e%s, %%r8d # r8d = %c%c%d\n", registers_param[i], parameters[1].x, parameters[1].type, parameters[1].index);
                        printf("    movl    %%r8d, -%d(%%rbp) # %c%c%d = r8d\n", pos, parameters[0].x, parameters[0].type, parameters[0].index);
                        break;
                    case 'c':
                        printf("    movl    $%d, %%r8d # r8d = %d\n", parameters[1].index, parameters[1].index);
                        printf("    movl    %%r8d, -%d(%%rbp) # %c%c%d = r8d\n", pos, parameters[0].x, parameters[0].type, parameters[0].index);
                        break;
                }
                break;
            case 'r':
                for(i=0; i<pilha.reg_qtd;i++)
                    if(parameters[0].index == pilha.reg[i].ind) 
                        pos = 12 + i;

                switch(parameters[1].x){
                    case 'v':
                        if( parameters[1].type == 'i' ){
                            for(i=0; i<pilha.var_qtd;i++)
                                if(parameters[1].index == pilha.var[i].ind) 
                                    printf("    movl    -%d(%%rbp), %%r8d # r8d = %c%c%d\n", pilha.var[i].pos, parameters[1].x, parameters[1].type, parameters[1].index);
                            printf("    movl    %%r8d, %%r%dd # %c%c%d = r8d\n", pos, parameters[0].x, parameters[0].type, parameters[0].index);
                        }
                        else{
                            for(i=0; i<pilha.reg_qtd;i++)
                                if(parameters[1].index == pilha.reg[i].ind) 
                                    printf("    movl    %%r%dd, %%r8d # r8d = %c%c%d\n", 12+i, parameters[1].x, parameters[1].type, parameters[1].index);
                            printf("    movl    %%r8d, %%r%dd # %c%c%d = r8d\n", pos, parameters[0].x, parameters[0].type, parameters[0].index);
                        }
                        break;
                    case 'p':
                        for(i=0; i<pilha.param_qtd;i++)
                            if(parameters[1].index == i+1) 
                                printf("    movl    %%e%s, %%r8d # r8d = %c%c%d\n", registers_param[i], parameters[1].x, parameters[1].type, parameters[1].index);
                        printf("    movl    %%r8d, %%r%dd # %c%c%d = r8d\n", pos, parameters[0].x, parameters[0].type, parameters[0].index);
                        break;
                    case 'c':
                        printf("    movl    $%d, %%r8d # r8d = %d\n", parameters[1].index, parameters[1].index);
                        printf("    movl    %%r8d, %%r%dd # %c%c%d = r8d\n", pos, parameters[0].x, parameters[0].type, parameters[0].index);
                        break;
                }
                break;
            default:
                break;
        }
    }
    else{
        switch(operator){
            case '+':
                pos1 = pegapos(parameters[0], pilha, &isAtribute1);
                pos2 = pegapos(parameters[1], pilha, &isAtribute2);
                pos3 = pegapos(parameters[2], pilha, &isAtribute3);

                if(pos2 == 722){
                    printf("    movl    $%d, %%r8d # r8d = %d\n", parameters[1].index, parameters[1].index);
                }
                else if( pos2 > 0 ){
                    printf("    movl    %%r%dd, %%r8d # r8d = %c%c%d\n", pos2, parameters[1].x, parameters[1].type, parameters[1].index);
                }
                else{
                    if(isAtribute2 == 0) printf("    movl    %d(%%rbp), %%r8d # r8d = %c%c%d\n", pos2, parameters[1].x, parameters[1].type, parameters[1].index);
                    else printf("    movl    %%e%s, %%r8d # r8d = %c%c%d\n", registers_param[parameters[1].index-1], parameters[1].x, parameters[1].type, parameters[1].index);
                }

                if(pos3 == 722){    
                    printf("    addl    $%d, %%r8d # r8d = r8d + %d\n", parameters[2].index, parameters[2].index);
                }
                else if( pos3 > 0 ){
                    printf("    addl    %%r%dd, %%r8d # r8d = r8d + %c%c%d\n", pos3, parameters[2].x, parameters[2].type, parameters[2].index);
                    
                }
                else{
                    if(isAtribute3 == 0) printf("    addl    %d(%%rbp), %%r8d # r8d = r8d + %c%c%d\n", pos3, parameters[2].x, parameters[2].type, parameters[2].index);
                    else printf("    addl    %%e%s, %%r8d # r8d = r8d + %c%c%d\n", registers_param[parameters[2].index-1], parameters[2].x, parameters[2].type, parameters[2].index);
                }

                if( pos1 > 0 ){
                    printf("    movl    %%r8d, %%r%dd # %c%c%d = r8d\n", pos1, parameters[0].x, parameters[0].type, parameters[0].index);
                }
                else{
                    printf("    movl    %%r8d, %d(%%rbp) # %c%c%d = r8d\n", pos1, parameters[0].x, parameters[0].type, parameters[0].index);
                }
                
                break;
            case '-':
                pos1 = pegapos(parameters[0], pilha, &isAtribute1);
                pos2 = pegapos(parameters[1], pilha, &isAtribute2);
                pos3 = pegapos(parameters[2], pilha, &isAtribute3);

                if(pos2 == 722){
                    printf("    movl    $%d, %%r8d # r8d = %d\n", parameters[1].index, parameters[1].index);
                }
                else if( pos2 > 0 ){
                    printf("    movl    %%r%dd, %%r8d # r8d = %c%c%d\n", pos2, parameters[1].x, parameters[1].type, parameters[1].index);
                }
                else{
                    if(isAtribute2 == 0) printf("    movl    %d(%%rbp), %%r8d # r8d = %c%c%d\n", pos2, parameters[1].x, parameters[1].type, parameters[1].index);
                    else printf("    movl    %%e%s, %%r8d # r8d = %c%c%d\n", registers_param[parameters[1].index-1], parameters[1].x, parameters[1].type, parameters[1].index);
                }

                if(pos3 == 722){    
                    printf("    subl    $%d, %%r8d # r8d = r8d - %d\n", parameters[2].index, parameters[2].index);
                }
                else if( pos3 > 0 ){
                    printf("    subl    %%r%dd, %%r8d # r8d = r8d - %c%c%d\n", pos3, parameters[2].x, parameters[2].type, parameters[2].index);
                }
                else{
                    if(isAtribute3 == 0) printf("    subl    %d(%%rbp), %%r8d # r8d = r8d - %c%c%d\n", pos3, parameters[2].x, parameters[2].type, parameters[2].index);
                    else printf("    subl    %%e%s, %%r8d # r8d = r8d - %c%c%d\n", registers_param[parameters[2].index-1], parameters[2].x, parameters[2].type, parameters[2].index);
                }

                if( pos1 > 0 ){
                    printf("    movl    %%r8d, %%r%dd # %c%c%d = r8d\n", pos1, parameters[0].x, parameters[0].type, parameters[0].index);
                }
                else{
                    printf("    movl    %%r8d, %d(%%rbp) # %c%c%d = r8d\n", pos1, parameters[0].x, parameters[0].type, parameters[0].index);
                }

                break;
            case '*':
                pos1 = pegapos(parameters[0], pilha, &isAtribute1);
                pos2 = pegapos(parameters[1], pilha, &isAtribute2);
                pos3 = pegapos(parameters[2], pilha, &isAtribute3);

                if(pos2 == 722){
                    printf("    movl    $%d, %%r8d # r8d = %d\n", parameters[1].index, parameters[1].index);
                }
                else if( pos2 > 0 ){
                    printf("    movl    %%r%dd, %%r8d # r8d = %c%c%d\n", pos2, parameters[1].x, parameters[1].type, parameters[1].index);
                }
                else{
                    if(isAtribute2 == 0) printf("    movl    %d(%%rbp), %%r8d # r8d = %c%c%d\n", pos2, parameters[1].x, parameters[1].type, parameters[1].index);
                    else printf("    movl    %%e%s, %%r8d # r8d = %c%c%d\n", registers_param[parameters[1].index-1], parameters[1].x, parameters[1].type, parameters[1].index);
                }

                if(pos3 == 722){    
                    printf("    imull   $%d, %%r8d # r8d = r8d * %d\n", parameters[2].index, parameters[2].index);
                }
                else if( pos3 > 0 ){
                    printf("    imull   %%r%dd, %%r8d # r8d = r8d * %c%c%d\n", pos3, parameters[2].x, parameters[2].type, parameters[2].index);
                }
                else{
                    if(isAtribute3 == 0) printf("    imull   %d(%%rbp), %%r8d # r8d = r8d * %c%c%d\n", pos3, parameters[2].x, parameters[2].type, parameters[2].index);
                    else printf("    imull   %%e%s, %%r8d # r8d = r8d * %c%c%d\n", registers_param[parameters[2].index-1], parameters[2].x, parameters[2].type, parameters[2].index);
                }

                if( pos1 > 0 ){
                    printf("    movl    %%r8d, %%r%dd # %c%c%d = r8d\n", pos1, parameters[0].x, parameters[0].type, parameters[0].index);
                }
                else{
                    printf("    movl    %%r8d, %d(%%rbp) # %c%c%d = r8d\n", pos1, parameters[0].x, parameters[0].type, parameters[0].index);
                }

                break;
            case '/':
                pos1 = pegapos(parameters[0], pilha, &isAtribute1);
                pos2 = pegapos(parameters[1], pilha, &isAtribute2);
                pos3 = pegapos(parameters[2], pilha, &isAtribute3);

                if(pilha.param_qtd > 0){
                    if(pilha.param[0].pos%8 == 0){
                        printf("    movq    %%rdx, -%d(%%rbp) # salvando rdx\n", pilha.param[0].pos);
                    }
                    else{
                        printf("    movl    %%edx, -%d(%%rbp) # salvando edx\n", pilha.param[0].pos);
                    }
                }

                if(pos2 == 722){
                    printf("    movl    $%d, %%eax # eax = %d\n", parameters[1].index, parameters[1].index);
                }
                else if( pos2 > 0 ){
                    printf("    movl    %%r%dd, %%eax # eax = %c%c%d\n", pos2, parameters[1].x, parameters[1].type, parameters[1].index);
                }
                else{
                    if(isAtribute2 == 0) printf("    movl    %d(%%rbp), %%eax # eax = %c%c%d\n", pos2, parameters[1].x, parameters[1].type, parameters[1].index);
                    else printf("    movl    %%e%s, %%eax # eax = %c%c%d\n", registers_param[parameters[1].index-1], parameters[1].x, parameters[1].type, parameters[1].index);
                }

                printf("    cltd\n");

                if(pos3 == 722){    
                    printf("    movl    $%d, %%r8d # r8d = %d\n", parameters[2].index, parameters[2].index);
                    printf("    idivl   %%r8d\n");
                }
                else if( pos3 > 0 ){
                    printf("    idivl   %%r%dd\n", pos3);
                }
                else{
                    if(isAtribute3 == 0) printf("    idivl   %d(%%rbp)\n", pos3);
                    else{
                        if(parameters[2].index != 3) printf("    idivl    %%e%s\n", registers_param[parameters[2].index-1]);
                        else printf("    idivl   %d(%%rbp)\n", pos3);
                    } 
                }

                if( pos1 > 0 ){
                    printf("    movl    %%eax, %%r%dd # %c%c%d = eax\n", pos1, parameters[0].x, parameters[0].type, parameters[0].index);
                }
                else{
                    printf("    movl    %%eax, %d(%%rbp) # %c%c%d = eax\n", pos1, parameters[0].x, parameters[0].type, parameters[0].index);
                }

                if(pilha.param_qtd > 0){
                    if(pilha.param[0].pos%8 == 0){
                        printf("    movq    -%d(%%rbp), %%rdx # recuperando rdx\n", pilha.param[0].pos);
                    }
                    else{
                        printf("    movl    -%d(%%rbp), %%edx # recuperando edx\n", pilha.param[0].pos);
                    }
                }
                break;
            default:
                break;
        }
    }
}

int pegapos(Typecharint parameters, Pilha pilha, int * x){
    int i;
    *x = 0;

    if(parameters.x == 'v'){
        if(parameters.type == 'i'){
            i = encontrar_indvar(pilha, parameters.index);
            return -(pilha.var[i].pos);
        }
        else {
            i = encontrar_indreg(pilha, parameters.index);
            return 12+i;
        }
    }
    else if(parameters.x == 'p'){
        for(i=0 ; i<pilha.param_qtd ; i++){
            if( (i+1) == parameters.index){
                *x = 1;
                return -(pilha.param[i].pos);
            }   
        }
    }
    else return 722;
}

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
