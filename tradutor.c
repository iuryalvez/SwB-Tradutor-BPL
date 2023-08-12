#include "./include/tradutor.h"

int main() {
    // cabeçalho de um arquivo assembly
    printf(".section rodata\n\n");
    printf(".data\n\n");
    printf(".text\n\n");

    int i;

    char linha[TAM_LINHA]; // armazena a linha lida do arquivo 

    Pilha pilha; 

    int callss; // Conta os parametros do sscanf da função call;

    Registrador r[MAX_REG];
    iniciar_registradores(r);

    Typecharint parameters[4];
    inicializar_parameters(parameters); // parameters[0].type = 'v' e o resto zera tudo.

    // variáveis de declaração de função
    int indice_funcao = 0; // índice da função
    char nomefuncao[5]; // f9999 (5 letras)

    // variáveis de acesso à vetores
    int arr;  // array
    int ind;  // índice
    int dest; // destino

    // flags de controle
    bool bloco_declaracao = true; // flag do bloco de declaração da função
    bool bloco_def = false;       // flag do bloco de definição de variáveis locais da função  
    bool bloco_funcao = false;    // flag do bloco de execução da função

    while(ler_linha(linha) != false) { // continua o loop linha a linha enquanto não chega numa linha nula ou no EOF 

        if (linha == '\0') continue; // se a linha for vazia, vai para a próxima

        if (bloco_declaracao) { // se a flag estiver ativa, procuraremos a declaração
            
            if (strncmp(linha, "function", 8) == 0){ // verifica se a linha inicia com a declaração de uma função

                inicializar_pilha(&pilha);                

                pilha.param_qtd = sscanf(linha, "function f%d p%c1 p%c2 p%c3", &indice_funcao, &pilha.param[0].tipo, &pilha.param[1].tipo, &pilha.param[2].tipo);
                
                --pilha.param_qtd; // atualizando a quantidade de parâmetros que o sscanf retornou para armazenar de fato a qtd de parâmetros

                printf(".globl f%d\n", indice_funcao);
                printf("f%d:\n", indice_funcao);
                printf("    pushq   %%rbp\n");
                printf("    movq    %%rsp, %%rbp\n");

                continue;
            }
            
            if (strncmp(linha, "def", 3) == 0){ // verifica se a linha é o início da definição de variáveis
                
                bloco_declaracao = false; // finaliza a declaração
                bloco_def = true; // permite o início da definição de variáveis
                
                continue;
            }
        }

        if (bloco_def) {
            if (strncmp(linha, "enddef", 6) == 0) { // verifica se a linha é o fim da definição de variáveis
                bloco_def = false;   // finaliza a definição de variáveis
                bloco_funcao = true; // permite o início do corpo da função

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
            if (strncmp(linha, "end", 3) == 0) { // verifica se a linha é uma finalização de função
                
                print_recuperacao(pilha);

                // código da finalização de função aqui (recuperação de v)
                printf("\n    leave\n");
                printf("    ret\n\n");

                bloco_funcao = false; // finaliza o corpo da função
                bloco_declaracao = true; // permite o início de uma nova declaração de função
                
                continue;
            }

            if (strncmp(linha, "get", 3) == 0) {
                // sscanf(line, "get %ca%d index ci%d %*s %ci%d", 
                // &_1_operand_type,
                // &_1_operand_index,
                // &vet_size,
                // &_2_operand_type,
                // &_2_operand_index);
            

                // printf("\n    movq    $%d, %%r10\n", vet_size);
                // printf("    imulq   $4, %%r10\n");

                // if(_1_operand_type == 'v'){
                    // printf("    leaq    -%d(%%rbp), %%r9\n", var_stack[_1_operand_index-1]);
                // }
                // if(_1_operand_type == 'p'){
                    // printf("    movq    -%d(%%rbp), %%r9\n", para_stack[_1_operand_index-1]);
                // }
                
                // printf("    addq    %%r9, %%r10\n");
                // printf("    movl    (%%r10), %%r8d\n");

                // if(_2_operand_type == 'v'){
                    // printf("    movl    %%r8d, -%d(%%rbp)\n", var_stack[_2_operand_index-1]);
                // }
                // if(_2_operand_type == 'p'){
                    // printf("    movl    %%r8d, -%d(%%rbp)\n", para_stack[_2_operand_index-1]);
                // }

                continue;
            }
            
            if (strncmp(linha, "set", 3) == 0) {

                continue;
            }

            if ((linha[0] == 'v') & (linha[4] == '=')  & (linha[6] == 'c')  & (linha[7] == 'a')  & (linha[8] == 'l')  & (linha[9] == 'l') ) { // verifica se a linha é uma chamada de função
                callss = sscanf(linha, "v%c%d = call %s %c%c%d %c%c%d %c%c%d", &parameters[0].type, &parameters[0].index, nomefuncao, &parameters[1].x, &parameters[1].type, &parameters[1].index, &parameters[2].x, &parameters[2].type, &parameters[2].index, &parameters[3].x, &parameters[3].type, &parameters[3].index);
                // 12 parametros a serem lidos, 0 var = 3, 1 var = 6, 2 var = 9, 3 var = 12.

              /*
                    printf("\n\n==================================================================================================\n\n");
                    printf("\n\nCallss = %d\n\n", callss);
                    for(int i=0; i<4 ; i++){
                        printf("Index = %d, Type = %c, X = %c.\n", parameters[i].index, parameters[i].type, parameters[i].x);
                    }
                    printf("\n\n==================================================================================================\n\n");
              
              */

               salvar_parametros(pilha);

               switch(callss){
                    case 3:

                        callfuncao(nomefuncao);

                        break;
                    case 6:

                        callfuncao(nomefuncao);

                        break;
                    case 9:

                        callfuncao(nomefuncao);

                        break;
                    case 12:

                        callfuncao(nomefuncao);
                        break;
                    default:
                        break;
               }       

               printf("\n\n\nType = %c\n\n\n", parameters[0].type);

               if( parameters[0].type == 'a' ){
                    for(i=0 ; i<pilha.vet_qtd ; i++){
                        if(pilha.vet[i].ind == parameters[0].index) printf("\n    movq      %%rax, -%d(%%rbp) # v%c%d = Retorno da função\n",pilha.vet[i].pos,parameters[0].type,parameters[0].index);
                    }
               }
               else if( parameters[0].type == 'r' ){
                    for(i=0 ; i<pilha.reg_qtd ; i++){
                        if(pilha.reg[i].ind == parameters[0].index) printf("\n    movq      %%rax, -%d(%%rbp) # v%c%d = Retorno da função\n",pilha.reg[i].pos,parameters[0].type,parameters[0].index);
                    }
               }
               else {
                    for(i=0 ; i<pilha.var_qtd ; i++){
                        if(pilha.var[i].ind == parameters[0].index) printf("\n    movl      %%eax, -%d(%%rbp) # v%c%d = Retorno da função\n",pilha.var[i].pos,parameters[0].type,parameters[0].index);
                    }
               }

                recuperar_parametros(pilha);
                inicializar_parameters(parameters);

                continue;
            }
        }
    }
    return 0;
}

