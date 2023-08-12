#include "./include/tradutor.h"

int main() {
    // cabeçalho de um arquivo assembly
    printf(".section rodata\n\n");
    printf(".data\n\n");
    printf(".text\n\n");

    char linha[TAM_LINHA]; // armazena a linha lida do arquivo 

    Pilha pilha; 

    Registrador r[MAX_REG];
    iniciar_registradores(r);

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

                if (pilha.param_qtd == -1 || pilha.param_qtd > 3) {
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
            if (sscanf(linha, "reg vr%d", &pilha.reg[pilha.reg_qtd].ind) == 1 && indice_funcao != 1) {
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
                if (strncmp(linha, "get pa", 6) == 0) {
                    if (sscanf(linha, "get pa%d index ci%d to pi%d", &arr, &ind, &dest) == 3) {
                        
                        printf("\n    # pi%d = pa%d[%d]\n", dest, arr, ind);
                        printf("    movq    $%d, %%r8\n", ind);
                        printf("    imulq   $4, %%r8\n");
                        printf("    leaq    -%d(%%rbp), %%r9\n", pilha.param[arr-1].pos);
                        printf("    addq    %%r9, %%r8\n");
                        printf("    movl    (%%r8), %%r10\n");
                        printf("    movl    %%r10d, -%d(%%rbp)\n", pilha.param[dest].pos);

                    } 
                    else if (sscanf(linha, "get pa%d index ci%d to vi%d", &arr, &ind, &dest) == 3) {
                        


                    } 
                    else if (sscanf(linha, "get pa%d index ci%d to vr%d", &arr, &ind, &dest) == 3) {
                        


                    }
                }
                else {
                    if (sscanf(linha, "get va%d index ci%d to pi%d", &arr, &ind, &dest) == 3) {

                    } 
                    else if (sscanf(linha, "get va%d index ci%d to vi%d", &arr, &ind, &dest) == 3) {

                    } 
                    else if (sscanf(linha, "get va%d index ci%d, to vr%d", &arr, &ind, &dest) == 3) {

                    }
                }

                continue;
            }
            
            if (strncmp(linha, "set", 3) == 0) {

                continue;
            }

            if (strncmp(linha, "call", 3) == 0) { // verifica se a linha é uma chamada de função
                sscanf(linha, "call %s", nomefuncao);

                printf("\n    call %s\n", nomefuncao);

                continue;
            }
        }
    }

    return 0;
}
