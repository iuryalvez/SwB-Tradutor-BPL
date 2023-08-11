#include "./include/tradutor.h"

int main() {
    // cabeçalho de um arquivo assembly
    printf(".section rodata\n\n");
    printf(".data\n\n");
    printf(".text\n\n");

    char linha[TAM_LINHA]; // armazena a linha lida do arquivo 

    Pilha pilha; 

    // variáveis de declaração de função
    int indice_funcao = 0; // índice da função
    char nomefuncao[5]; // f9999 (5 letras)
    

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

                // armazenando a posição na pilha das variáveis de declaração de função 
                for (int i = 0; i < pilha.param_qtd; i++) {
                    if (pilha.param[i].tipo == 'i') { // se for do tipo inteiro
                        pilha.rsp += 4; // não precisa alinhar porque 4 é múltiplo de 4, 8, 16
                        pilha.param[i].pos = pilha.rsp; // guarda a posição na pilha
                    } else {
                        alinhar(&(pilha.rsp), 8);  // precisa alinhar para 8 (ponteiro) pq não sabemos se está alinhado
                        pilha.rsp += 8;      // desce o topo da pilha para colocar mais uma variável
                        pilha.param[i].pos = pilha.rsp; // guarda a posição na pilha
                    }
                }

                alinhar(&(pilha.rsp), 16);
                
                printf("\n    # armazenar todas as variáveis (inclusive os registradores de parametros)\n");
                printf("    subq    $%d, %%rsp\n", pilha.rsp);

                armazenar_pilha(pilha);
                
                continue;
            }

            // lendo as variáveis locais e registrando suas posições na pilha
            if (sscanf(linha, "reg vr%d", &pilha.reg[pilha.reg_qtd].ind) == 1 && indice_funcao != 1) {
                alinhar(&(pilha.rsp), 8);
                pilha.rsp += 8;
                pilha.reg[pilha.reg_qtd].pos = pilha.rsp;
                pilha.reg_qtd++;
                continue;
            }
            if (sscanf(linha, "var vi%d", &pilha.var[pilha.var_qtd].ind) == 1) {
                pilha.rsp += 4;
                pilha.var[pilha.var_qtd].pos = pilha.rsp;
                pilha.var_qtd++;
                continue;
            }
            if (sscanf(linha, "vet va%d size ci%d", &pilha.vet[pilha.vet_qtd].ind, &pilha.vet[pilha.vet_qtd].size) == 2) {
                alinhar(&(pilha.rsp), 8);
                pilha.rsp += 8 * pilha.vet[pilha.vet_qtd].size;
                pilha.vet[pilha.vet_qtd].pos = pilha.rsp;
                pilha.vet_qtd++;
                continue;
            }        
        }

        if (bloco_funcao) {

            // código do corpo da função aqui (cálculos, chamadas de outras funções, etc)
            if (strncmp(linha, "end", 3) == 0) { // verifica se a linha é uma finalização de função
                
                recuperar_pilha(pilha);

                // código da finalização de função aqui (recuperação de v)
                printf("\n    leave\n");
                printf("    ret\n\n");

                bloco_funcao = false; // finaliza o corpo da função
                bloco_declaracao = true; // permite o início de uma nova declaração de função
                
                continue;
            }
            if (strncmp(linha, "call", 3) == 0) { // verifica se a linha é uma chamada de função
                sscanf(linha, "call %s", nomefuncao);

                salvar_parametros(pilha); 
                printf("\n    call %s\n", nomefuncao);
                recuperar_parametros(pilha); 

                continue;
            }
        }
    }

    return 0;
}
