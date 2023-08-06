#include "./include/tradutor.h"

/*  Última modificação 06/08 - 15h13

    Modificações IURY (06/08 - 2h51): 
    
    -> formatação: organizei algumas formatações. obs: utilizar o tab do teclado e não o \t porque o \t é muito grande;
    
    -> organização: passei o código principal que estava na "call_funcao()" para a main porque faz mais sentido, percebi
                    que o código não precisa mais ter a ideia de recursão também;

    -> bloco_def: implementei algumas regras de variáveis locais (bloco_def), o conceito de tamanho da
       pilha (junto com a ideia de múltiplo), criei novas variáveis (vlp, ind_reg); 
    
    -> problemas: 
        - 1. criei as funções de salvar e recuperar os registradores de parâmetros ao ler um call, mas tem
          algo errado na implementação, verificar depois e corrigir; 
        - 2. ao compilar o código surge alguns warnings em relação ao uso da struct registrador, verificar 
          depois e corrigir.

    Modificações IURY (06/08 - 14h24):
    
    -> arquivo shell: acrescentei o clear antes e exclui o arquivo test depois de executá-lo para que caso um programa
       que não deve compilar compile. 

    -> refactor: refatorei os códigos de armazenar as variáveis e os registradores, mas não sei até que ponto está dando certo;
    
    -> fix problema 1: implementei as funções de salvar e recuperar parâmetros antes da chamada de função;
    
    -> fix problema 2: o problema não estava deixando o código compilar, faltava as declarações ifndef nos arquivos .h
       tentei criar uma variável global (entre os arquivos) para representar os registradores mas não consegui, tive
       que continuar com a implementação da variável dentro do bloco da main, com o ifndef agora está funcionando.

    -> o que precisa ser feito: reavaliar a função de alinhar pilha para ter certeza que vai funcionar em todos os casos 
*/

int main() {

    // cabeçalho de um arquivo assembly
    printf(".section rodata\n\n");
    printf(".data\n\n");
    printf(".text\n\n");
    
    Registrador r[MAX_REG];

    char linha[TAM_LINHA]; // armazena a linha lida do arquivo 

    // variáveis de declaração de função
    int indice_funcao;     // índice da função
    int nro_param;         // número de parâmetros da função
    char tipo_param[3];    // tipo de parâmetro

    // variáveis de controle e contagem da pilha
    int tampilha = 0; // tamanho da pilha da função

    // variáveis de controle da pilha
    vl vlp[8]; // variáveis locais de pilha
    int ind_reg[4];   // índice dos registradores
    int cont_reg = 0; // contador de registradores
    int cont_vlp = 0; // contador de variaveis locais de pilha

    // flags de controle
    bool bloco_declaracao = true; // flag do bloco de declaração da função
    bool bloco_def = false;       // flag do bloco de definição de variáveis locais da função  
    bool bloco_funcao = false;    // flag do bloco de execução da função

    while(ler_linha(linha) != false) { // continua o loop linha a linha enquanto não chega numa linha nula ou no EOF 

        if (linha == '\0') continue; // se a linha for vazia, vai para a próxima

        if (bloco_declaracao) { // se a flag estiver ativa, procuraremos a declaração
            
            if (strncmp(linha, "function", 8) == 0){ // verifica se a linha inicia com a declaração de uma função

                nro_param = sscanf(linha, "function f%d p%c1 p%c2 p%c3", &indice_funcao, &tipo_param[0], &tipo_param[1], &tipo_param[2]);

                if (nro_param == 0 || nro_param > 4) {
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

                iniciar_registradores(r);
                
                continue;
            }

        }

        if (bloco_def) {

            if (strncmp(linha, "enddef", 6) == 0) { // verifica se a linha é o fim da definição de variáveis
                bloco_def = false;   // finaliza a definição de variáveis
                bloco_funcao = true; // permite o início do corpo da função
                
                // se houver variáveis locais de registradores, armazenamos
                if (cont_reg) armazenar_vr(ind_reg, cont_reg, r); 
                
                // se houver variáveis locais de pilha, armazenamos
                if (cont_vlp) armazenar_vlp(vlp, cont_vlp, tampilha);

                continue;
            }

            // código da definição de variáveis aqui
            if (sscanf(linha, "reg vr%d", &ind_reg[cont_reg]) == 1) {

                cont_reg++;
                
            }
            if (sscanf(linha, "var vi%d", &vlp[cont_vlp].indice) == 1) {
                
                vlp[cont_vlp].size = 1;
                alinhar_tampilha(&tampilha, 4); // alinhando a pilha para um endereço mútiplo de 4 (int)
                tampilha += 4; // atualizando o valor da pilha
                vlp[cont_vlp].pos_pilha = tampilha; // a posição da pilha na variável é o valor do tamanho da pilha
                cont_vlp++; // atualizando o contador de variáveis locais
                
                continue;
            }
            if (sscanf(linha, "vet va%d size ci%d", &vlp[cont_vlp].indice, &vlp[cont_vlp].size) == 2) {
                
                alinhar_tampilha(&tampilha, 8); // alinhando a pilha para um endereço múltiplo de 8 (ponteiro)
                tampilha += 8*vlp[cont_vlp].size; // atualizando o valor da pilha
                vlp[cont_vlp].pos_pilha = tampilha; // a posição da pilha da variável é o valor do tamanho da pilha
                cont_vlp++; // atualizando o contador de variáveis locais
                
                continue;
            }        
        }

        if (bloco_funcao) {

            // código do corpo da função aqui (cálculos, chamadas de outras funções, etc)

            if (strncmp(linha, "end", 3) == 0) { // verifica se a linha é uma finalização de função
                
                // código da finalização de função aqui (recuperação de v)
                printf("\n    leave\n");
                printf("    ret\n\n");

                bloco_funcao = false; // finaliza o corpo da função
                bloco_declaracao = true; // permite o início de uma nova declaração de função
                
                // agora é necessário reiniciar os valores das variáveis para a próxima função

                continue;
            }

            if (strncmp(linha, "call", 3) == 0) { // verifica se a linha é uma chamada de função
                salvar_parametros(nro_param, &tampilha, r);
                
                // código de chamada de função aqui

                recuperar_parametros(nro_param, &tampilha, r);
            }
        }

    }

    return 0;
}
