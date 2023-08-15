#include "tradutor.h"

int main() {
    // cabeçalho de um arquivo assembly
    printf("\n.section rodata\n\n");
    printf(".data\n\n");
    printf(".text\n");

    char *registers_param[] = {"di", "si", "dx"};

    int i;
    char op; // operator

    char linha[TAM_LINHA]; // armazena a linha lida do arquivo 

    Pilha pilha; 

    int nro_param; // Conta os parametros do sscanf da função call; // antigo callss

    Reg_param r[3];
    iniciar_r(r);

    Typecharint parameters[4];
    inicializar_parameters(parameters); // parameters[0].type = 'v' e o resto zera tudo.

    // variáveis de declaração de função
    int indice_funcao = 0; // índice da função
    char nomefuncao[5]; // f9999 (5 letras)

    // variável de expressão
    Exp exp;

    // variável de acesso à vetores
    int size = 0;

    // variável de condicional
    int if_cont = 0;
    char oprel[2];

    // flaexp de controle
    int bloco_declaracao = TRUE; // flag do bloco de declaração da função
    int bloco_def = FALSE;       // flag do bloco de definição de variáveis locais da função  
    int bloco_funcao = FALSE;    // flag do bloco de execução da função
    int bloco_if = TRUE;         // flag do bloco de if

    while(ler_linha(linha) != FALSE) { // continua o loop linha a linha enquanto não chega numa linha nula ou no EOF 

        if (linha == '\0') continue; // se a linha for vazia, vai para a próxima

        if (bloco_declaracao) { // se a flag estiver ativa, procuraremos a declaração
            
            if (strncmp(linha, "function", 8) == 0){ // verifica se a linha inicia com a declaração de uma função

                inicializar_pilha(&pilha);                

                pilha.param_qtd = sscanf(linha, "function f%d p%c1 p%c2 p%c3", &indice_funcao, &pilha.param[0].tipo, &pilha.param[1].tipo, &pilha.param[2].tipo);
                
                --pilha.param_qtd; // atualizando a quantidade de parâmetros que o sscanf retornou para armazenar de fato a qtd de parâmetros

                printf("\n .globl f%d\n", indice_funcao);
                printf(" f%d:\n", indice_funcao);
                printf("    pushq   %%rbp\n");
                printf("    movq    %%rsp, %%rbp\n");

                continue;
            }
            
            if (strncmp(linha, "def", 3) == 0){ // verifica se a linha é o início da definição de variáveis
                
                bloco_declaracao = FALSE; // finaliza a declaração
                bloco_def = TRUE; // permite o início da definição de variáveis
                
                continue;
            }
        }

        if (bloco_def) {
            if (strncmp(linha, "enddef", 6) == 0) { // verifica se a linha é o fim da definição de variáveis
                bloco_def = FALSE;   // finaliza a definição de variáveis
                bloco_funcao = TRUE; // permite o início do corpo da função

                armazenar_pilha(&pilha);

                print_armazenamento(pilha);
                
                continue;
            }

            // lendo as variáveis locais e registrando suas posições na pilha
            if (sscanf(linha, "var vi%d", &pilha.var[pilha.var_qtd].ind) == 1) {
                pilha.var_qtd++;
                continue;
            }
            if (sscanf(linha, "vet va%d size ci%d", &pilha.vet[pilha.vet_qtd].ind, &pilha.vet[pilha.vet_qtd].size) == 2) {
                pilha.vet_qtd++;
                continue;
            }        
            if (sscanf(linha, "reg vr%d", &pilha.reg[pilha.reg_qtd].ind) == 1) {
                pilha.reg_qtd++;
                continue;
            }
        }

        if (bloco_funcao) {

            // código do corpo da função aqui (cálculos, chamadas de outras funções, etc)
            if (strncmp(linha, "end", 3) == 0 && strncmp(linha, "endif", 5) != 0) { // verifica se a linha é uma finalização de função

                bloco_funcao = FALSE; // finaliza o corpo da função
                bloco_declaracao = TRUE; // permite o início de uma nova declaração de função
                
                continue;
            }
            
            if (strncmp(linha, "endif", 5) == 0) {
                printf("\n    end_if0%d:\n", if_cont);
                bloco_if = TRUE;
                continue;
            }

            if (strncmp(linha, "return", 6) == 0) {
                sscanf(linha, "return %c%c%d", &exp.tipo1[0], &exp.tipo1[1], &exp.ind1);
                if (exp.tipo1[0] == 'c') {
                    printf("\n    # retorno (%%eax) = %d\n", exp.ind1);
                    printf("    movl    $%d, %%eax\n", exp.ind1);
                }
                else if (exp.tipo1[0] == 'p') {
                    printf("\n    # retorno (%%eax) = %%%s\n", r[exp.ind1-1].nome32);
                    printf("    movl    %%%s, %%eax\n", r[exp.ind1-1].nome32);
                }
                else if (exp.tipo1[1] == 'r') { // vr
                    printf("\n    # retorno (%%eax) = vr%d\n", exp.ind1);
                    exp.ind1 = encontrar_indreg(pilha, exp.ind1);
                    printf("    movl    %%r%dd, %%eax\n", 11+exp.ind1);
                }
                else {
                    printf("\n    # retorno (%%eax) = vi%d\n", exp.ind1);
                    exp.ind1 = encontrar_indvar(pilha, exp.ind1);
                    printf("    movl    -%d(%%rbp), %%eax\n", pilha.var[exp.ind1].pos);
                }
                // código da finalização de função aqui (recuperação de valores da pilha)
                print_recuperacao(pilha);
                printf("\n    leave\n");
                printf("    ret\n");
            }

            // código de get ou set
            if (sscanf(linha, "%cet %ca%d index ci%d %*s %c%c%d", &op, &exp.tipo1[0], &exp.ind1, &size, &exp.tipo2[0], &exp.tipo2[1], &exp.ind2) == 7) {
                
                if (op == 'g') {
                    
                    // comentário do que iremos fazer
                    printf("\n    # %c%c%d = %ca%d[%d]\n", exp.tipo2[0], exp.tipo2[1], exp.ind2, exp.tipo1[0], exp.ind1, size);
                    
                    // calculando o índice
                    printf("    movq    $%d, %%r8\n", size);
                    printf("    imulq   $4, %%r8\n");
                    
                    // acessando o endereço do índice
                    if (exp.tipo1[0] == 'p') printf("    addq    %%%s, %%r8\n", r[exp.ind1-1].nome64);
                    else {
                        exp.ind1 = encontrar_indvet(pilha, exp.ind1);
                        printf("    leaq    -%d(%%rbp), %%r9\n", pilha.vet[exp.ind1].pos);
                        printf("    addq    %%r9, %%r8\n");
                    }

                    printf("    movl    (%%r8), %%r10d\n");

                    // colocando o valor do endereço no lugar de destino
                    if (exp.tipo2[0] == 'p') {
                        printf("    movl    %%r10d, %%%s\n", r[exp.ind2-1].nome32);
                    } 
                    else if (exp.tipo2[1] == 'i') {
                        exp.ind2 = encontrar_indvar(pilha, exp.ind2);
                        printf("    movl    %%r10d, -%d(%%rbp)\n", pilha.var[exp.ind2].pos);
                    } 
                    else {
                        exp.ind2 = encontrar_indreg(pilha, exp.ind2);
                        printf("    movl    %%r10d, %%r%dd\n", 11+exp.ind2);
                    }

                    continue;
                }
                else {

                    // comentário do que iremos fazer
                    printf("\n    # %ca%d[%d] = %c%c%d\n", exp.tipo1[0], exp.ind1, size, exp.tipo2[0], exp.tipo2[1], exp.ind2);

                    // colocar o valor em um registrador
                    if (exp.tipo2[0] == 'c') {
                        printf("    movl    $%d, %%r10d\n", exp.ind2);
                    }
                    else if (exp.tipo2[0] == 'p') {
                        printf("    movl    %%%s, %%r10d\n", r[exp.ind2-1].nome32);
                    }
                    else if (exp.tipo2[1] == 'i') {
                        exp.ind2 = encontrar_indvar(pilha, exp.ind2);
                        printf("    movl    -%d(%%rbp), %%r10d\n", pilha.var[exp.ind2].pos);
                    }
                    else {
                        exp.ind2 = encontrar_indreg(pilha, exp.ind2);
                        printf("    movl    %%r%dd, %%r10d\n", 11+exp.ind2);
                    }
                    
                    // calculando o índice
                    printf("    movq    $%d, %%r8\n", size);
                    printf("    imulq   $4, %%r8\n");

                    // acessar o vetor
                    if (exp.tipo1[0] == 'p') printf("    addq    %%%s, %%r8\n", r[exp.ind1-1].nome64);
                    else {
                        exp.ind1 = encontrar_indvet(pilha, exp.ind1);
                        printf("    leaq    -%d(%%rbp), %%r9\n", pilha.vet[exp.ind1].pos);
                        printf("    addq    %%r9, %%r8\n");
                    }

                    // colocar o resultado no endereço acessado
                    printf("    movl    %%r10d, (%%r8)\n");

                    continue;
                }
            }
            
            if (strncmp(linha, "call", 4) == 0) {
                nro_param = sscanf(linha, "call %s %c%c%d %c%c%d %c%c%d", nomefuncao, &parameters[1].x, &parameters[1].type, &parameters[1].index, &parameters[2].x, &parameters[2].type, &parameters[2].index, &parameters[3].x, &parameters[3].type, &parameters[3].index);

                salvar_parametros(pilha);
                printf("\n");
                switch(nro_param){
                    case 1:
                        atribui_call(pilha, parameters, 0, registers_param);
                        callfuncao(nomefuncao);

                        break;
                    case 4:
                        atribui_call(pilha, parameters, 1, registers_param);
                        callfuncao(nomefuncao);

                        break;
                    case 7:
                        atribui_call(pilha, parameters, 2, registers_param);
                        callfuncao(nomefuncao);

                        break;
                    case 10:
                        atribui_call(pilha, parameters, 3, registers_param);
                        callfuncao(nomefuncao);

                        break;
                    default:
                        break;
                }   
            }

            // código de atribuições
            if ( (( linha[4] == '=' ) && ( linha[7] != 'a')) || (( linha[5] == '=' ) && ( linha[8] != 'a')) ) {
                nro_param = sscanf(linha, "v%c%d = %c%c%d %c %c%c%d", &parameters[0].type, &parameters[0].index, &parameters[1].x, &parameters[1].type, &parameters[1].index, &op, &parameters[2].x, &parameters[2].type, &parameters[2].index);
                printf("\n");
                switch(nro_param){
                    case 5:
                        expressions(pilha, op, parameters, 1, registers_param);
                        break;
                    case 9:
                        expressions(pilha, op, parameters, 2, registers_param);
                        break;
                    default:
                        break;
                }

                inicializar_parameters(parameters);
                continue;
            }

            // código de atribuição de uma call
            if ((linha[0] == 'v') & (linha[4] == '=')  & (linha[6] == 'c')  & (linha[7] == 'a')  & (linha[8] == 'l')  & (linha[9] == 'l') || (linha[0] == 'v') & (linha[5] == '=')  & (linha[7] == 'c')  & (linha[8] == 'a')  & (linha[9] == 'l')  & (linha[10] == 'l')) { // verifica se a linha é uma chamada de função
                nro_param = sscanf(linha, "v%c%d = call %s %c%c%d %c%c%d %c%c%d", &parameters[0].type, &parameters[0].index, nomefuncao, &parameters[1].x, &parameters[1].type, &parameters[1].index, &parameters[2].x, &parameters[2].type, &parameters[2].index, &parameters[3].x, &parameters[3].type, &parameters[3].index);

                salvar_parametros(pilha);
                printf("\n");
                switch(nro_param){
                    case 3:
                        atribui_call(pilha, parameters, 0, registers_param);
                        callfuncao(nomefuncao);

                        break;
                    case 6:
                        atribui_call(pilha, parameters, 1, registers_param);
                        callfuncao(nomefuncao);

                        break;
                    case 9:
                        atribui_call(pilha, parameters, 2, registers_param);
                        callfuncao(nomefuncao);

                        break;
                    case 12:
                        atribui_call(pilha, parameters, 3, registers_param);
                        callfuncao(nomefuncao);

                        break;
                    default:
                        break;
               }       

               if( parameters[0].type == 'a' ){
                    for(i=0 ; i<pilha.vet_qtd ; i++){
                        if(pilha.vet[i].ind == parameters[0].index) printf("\n    movq      %%rax, -%d(%%rbp) # v%c%d = Retorno da função\n",pilha.vet[i].pos,parameters[0].type,parameters[0].index); // vax = Retorno
                    }
               }
               else if( parameters[0].type == 'r' ){
                    for(i=0 ; i<pilha.reg_qtd ; i++){
                        if(pilha.reg[i].ind == parameters[0].index) printf("\n    movl      %%eax, %%r%dd # v%c%d = Retorno da função\n",12+i,parameters[0].type, parameters[0].index); // vrx = Retorno
                    }
               }
               else {
                    for(i=0 ; i<pilha.var_qtd ; i++){
                        if(pilha.var[i].ind == parameters[0].index) printf("\n    movl      %%eax, -%d(%%rbp) # v%c%d = Retorno da função\n",pilha.var[i].pos,parameters[0].type,parameters[0].index); // vix = Retorno
                    }
               }

                recuperar_parametros(pilha);
                inicializar_parameters(parameters);

                continue;
            }
            
            // código de condicionais
            if (strncmp(linha, "if", 2) == 0 && bloco_if) {
                bloco_if = FALSE;
                if_cont++;
                sscanf(linha, "if %c%c%d %c%c %c%c%d", &exp.tipo1[0], &exp.tipo1[1], &exp.ind1, &oprel[0], &oprel[1], &exp.tipo2[0], &exp.tipo2[1], &exp.ind2);    
            
                printf("\n    if0%d:\n", if_cont);
                
                // o tipo 1 armazena o que vai ser printado em segundo lugar
                if (exp.tipo1[0] == 'c') printf("    movl    $%d, %%r8d\n", exp.ind1);
                else if (exp.tipo1[0] == 'p') printf("    movl    %%%s, %%r8d\n", r[exp.ind1-1].nome32);
                else if (exp.tipo1[1] == 'i') {
                    exp.ind1 = encontrar_indvar(pilha, exp.ind1);
                    printf("    movl    -%d(%%rbp), %%r8d\n", pilha.var[exp.ind1].pos);
                }
                else {
                    exp.ind1 = encontrar_indreg(pilha, exp.ind1);
                    printf("    movl    %%r%dd, %%r9d\n", exp.ind1+11); 
                }
                
                // o tipo 2 armazena o que vai ser printado em primeiro lugar
                if (exp.tipo2[0] == 'c') printf("    movl    $%d, %%r9d\n", exp.ind2);
                else if (exp.tipo2[0] == 'p') printf("    movl    %%%s, %%r9d\n", r[exp.ind2-1].nome32);
                else if (exp.tipo2[1] == 'i') { 
                    exp.ind2 = encontrar_indvar(pilha, exp.ind2);
                    printf("    movl    -%d(%%rbp), %%r9d\n", pilha.var[exp.ind2].pos);
                }
                else { 
                    exp.ind2 = encontrar_indreg(pilha, exp.ind2);
                    printf("    movl    %%r%dd, %%r9d\n", exp.ind2+11);
                }

                printf("    cmpl    %%r9d, %%r8d\n");

                if(oprel[0] == 'e') printf("    jne end_if0%d", if_cont);
                else if (oprel[0] == 'n') printf("    je end_if0%d", if_cont);
                else if (oprel[0] == 'l' && oprel[1] == 't') printf("    jge end_if0%d", if_cont);
                else if (oprel[0] == 'l' && oprel[1] == 'e') printf("    jg end_if0%d", if_cont);
                else if (oprel[0] == 'g' && oprel[1] == 't') printf("    jle end_if0%d", if_cont);
                else printf("    jl end_if0%d", if_cont);
                
                // printf(" # if !(%c%c%d %c%c %c%c%d) goto end_if0%d\n", exp.tipo1[0], exp.tipo1[1], exp.ind1, oprel[0], oprel[1], exp.tipo2[0], exp.tipo2[1], exp.ind2, if_cont);
                
                continue;
            }            
        }
    }

    return 0;
}