#include "./include/tradutor.h"

int main() {
    // cabeçalho de um arquivo assembly
    printf(".section rodata\n\n");
    printf(".data\n\n");
    printf(".text\n\n");
    
    int rsp = 0;     // tamanho da pilha da função
    // int padding = 0; // armazenar o padding da pilha

    char linha[TAM_LINHA]; // armazena a linha lida do arquivo 

    // variáveis de declaração de função
    int indice_funcao = 0; // índice da função
    char nomefuncao[5]; // f9999 (5 letras)
    
    Param param; // variável de parâmetro (declaração da função)
    inicializar_param(&param);

    VL vl; // variáveis locais (def e enddef)
    inicializar_vl(&vl);

    // flags de controle
    bool bloco_declaracao = true; // flag do bloco de declaração da função
    bool bloco_def = false;       // flag do bloco de definição de variáveis locais da função  
    bool bloco_funcao = false;    // flag do bloco de execução da função

    while(ler_linha(linha) != false) { // continua o loop linha a linha enquanto não chega numa linha nula ou no EOF 

        if (linha == '\0') continue; // se a linha for vazia, vai para a próxima

        if (bloco_declaracao) { // se a flag estiver ativa, procuraremos a declaração
            
            if (strncmp(linha, "function", 8) == 0){ // verifica se a linha inicia com a declaração de uma função

                rsp = 0;
                // padding = 0;
                inicializar_param(&param);

                param.qtd = sscanf(linha, "function f%d p%c1 p%c2 p%c3", &indice_funcao, &param.tipo[0], &param.tipo[1], &param.tipo[2]);
                
                --param.qtd;

                if (param.qtd == -1 || param.qtd > 3) {
                    printf("Tem algo de errado na declaracao da funcao.\n");
                    return 0;
                }

                printf(".globl f%d\n", indice_funcao);
                printf("f%d:\n", indice_funcao);
                printf("    pushq   %%rbp\n");
                printf("    movq    %%rsp, %%rbp\n");

                continue;
            }
            
            if (strncmp(linha, "def", 3) == 0){ // verifica se a linha é o início da definição de variáveis
                
                bloco_declaracao = false; // finaliza a declaração
                bloco_def = true; // permite o início da definição de variáveis

                // iniciar os valores das variáveis para a próxima função
                inicializar_vl(&vl);
                
                continue;
            }
        }

        if (bloco_def) {
            if (strncmp(linha, "enddef", 6) == 0) { // verifica se a linha é o fim da definição de variáveis
                bloco_def = false;   // finaliza a definição de variáveis
                bloco_funcao = true; // permite o início do corpo da função
                
                // padding = rsp; 
                alinhar_rsp(&rsp, 16); // se houver padding, rsp será atualizado
                // padding = rsp - padding; 
                
                printf("    subq    $%d, %%rsp\n", rsp);

                armazenar_vl(&vl);
                
                continue;
            }

            // lendo as variáveis locais e registrando suas posições na pilha
            if (sscanf(linha, "reg vr%d", &vl.reg[vl.reg_qtd].ind) == 1 && indice_funcao != 1) {
                alinhar_rsp(&rsp, 8);
                rsp += 8;
                vl.reg[vl.reg_qtd].pos = rsp;
                vl.reg_qtd++;
                continue;
            }
            if (sscanf(linha, "var vi%d", &vl.var[vl.var_qtd].ind) == 1) {
                rsp += 4;
                vl.var[vl.var_qtd].pos = rsp;
                vl.var_qtd++;
                continue;
            }
            if (sscanf(linha, "vet va%d size ci%d", &vl.vet[vl.vet_qtd].ind, &vl.vet[vl.vet_qtd].size) == 2) {
                alinhar_rsp(&rsp, 8);
                rsp += 8 * vl.vet[vl.vet_qtd].size;
                vl.vet[vl.vet_qtd].pos = rsp;
                vl.vet_qtd++;
                continue;
            }        
        }

        if (bloco_funcao) {

            // código do corpo da função aqui (cálculos, chamadas de outras funções, etc)
            if (strncmp(linha, "end", 3) == 0) { // verifica se a linha é uma finalização de função
                
                recuperar_vl(vl);

                printf("    addq    $%d, %%rsp\n", rsp);

                // código da finalização de função aqui (recuperação de v)
                printf("\n    leave\n");
                printf("    ret\n\n");

                bloco_funcao = false; // finaliza o corpo da função
                bloco_declaracao = true; // permite o início de uma nova declaração de função
                
                continue;
            }

            if (strncmp(linha, "call", 3) == 0) { // verifica se a linha é uma chamada de função
                sscanf(linha, "call %s", nomefuncao);

                // rsp -= padding;
                
                salvar_parametros(&param, &rsp); // salvar e mostrar que salvou os parâmetros da declaração da função
                alinhar_rsp(&rsp, 16);
                
                printf("    call %s\n", nomefuncao);
                
                recuperar_parametros(&param, &rsp); // recuperar e mostrar que recuperou os parâmetros da declaração da função
                
                // rsp += padding;
            }
        }
    }

    return 0;
}
