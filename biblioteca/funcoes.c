#include "../include/funcoes.h"

void call_funcao() {
    
    int indice_funcao; // índice da função atual
    int nro_param; // número de parâmetros da função atual
    char tipo_param[3]; // tipo de parâmetro
    char linha[TAM_LINHA];

    // flags de controle
    bool bloco_declaracao = true;
    bool bloco_def = false;
    bool bloco_funcao = false;

    // contadores de variáveis locais
    int cont_var = 0;
    int cont_reg = 0;
    int cont_vet = 0;

    while(ler_linha(linha) != false) {

        if (linha == '\0') continue; // se a linha for vazia, vai para a próxima

        if (bloco_declaracao) { // se a flag estiver ativa, procuraremos a declaração
            
            nro_param = sscanf(linha, "function f%d p%c1 p%c2 p%c3", &indice_funcao, &tipo_param[0], &tipo_param[1], &tipo_param[2]);

            switch (nro_param) {
                case 1:
                    // sem parâmetros
                    break;
                case 2:
                    // 1 parâmetro ... 
                    break;
                case 3:
                    // 2 parâmetros ...
                    break;
                case 4: 
                    // ...
                    break;
                default:
                    // tratamento de erros
            }

            bloco_declaracao = false;
            bloco_def = true;
            continue;

        }

        if (bloco_def) {
            if (strncmp(linha, "def", 3)) continue;
            
            // código aqui
            
            if (strncmp(linha, "enddef", 6)) {
                bloco_def = false;
                bloco_funcao = true;
                continue;
            }
        }

        if (bloco_funcao) {

        }

    }

}
