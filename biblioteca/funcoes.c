#include "../include/funcoes.h"

void call_funcao() {
    
    int indice_funcao; // índice da função atual
    int nro_param; // número de parâmetros da função atual
    char tipo_param[3]; // tipo de parâmetro
    char linha[TAM_LINHA];

    // variáveis de controle e contagem da pilha
    int offset = 0;
    int alocacao = 0;

    // flags de controle
    bool bloco_declaracao = true;
    bool bloco_def = false;
    bool bloco_funcao = false;

    // contadores de variáveis locais
    int cont_var = 0;
    int cont_reg = 0;
    int cont_vet = 0;

    while(ler_linha(linha) != false) { // continua o loop linha a linha enqunto não chega numa linha nula ou no EOF 

        if (linha == '\0') continue; // se a linha for vazia, vai para a próxima

        if (bloco_declaracao) { // se a flag estiver ativa, procuraremos a declaração
            
            if (strncmp(linha, "function", 8) == 0){ // verifica se a linha inicia com a declaração de uma função

                nro_param = sscanf(linha, "function f%d p%c1 p%c2 p%c3", &indice_funcao, &tipo_param[0], &tipo_param[1], &tipo_param[2]);

                printf(".globl f%d\n", indice_funcao);
                printf("f%d:\n\tpushq %rbp\n\tmovq %rsp, %rbp\n", indice_funcao);
                
                continue;
            }
            
            if (strncmp(linha, "def", 3) == 0){ // verifica se a linha é o início da definição de variáveis
                
                bloco_declaracao = false; // finaliza a declaração
                bloco_def = true; // permite o início da definição de variáveis
                
                continue;
            }

        }

        if (bloco_def) {

            // código da definição de variáveis aqui
            
            if (strncmp(linha, "enddef", 6) == 0) { // verifica se a linha é o fim da definição de variáveis
                
                bloco_def = false; // finaliza a definição de variáveis
                bloco_funcao = true; // permite o início do corpo da função
                
                continue;
            }
        }

        if (bloco_funcao) {

            // código do corpo da função aqui (cálculos, chamadas de outras funções, etc)

            if (strncmp(linha, "end", 3) == 0) { // verifica se a linha é uma finalização de função
                
                // código da finalização de função aqui (recuperação de v)
                printf("\tleave\n\tret\n");

                bloco_funcao = false; // finaliza o corpo da função
                bloco_declaracao = true; // permite o início de uma nova declaração de função
                
                continue;
            }
        }

    }

}
