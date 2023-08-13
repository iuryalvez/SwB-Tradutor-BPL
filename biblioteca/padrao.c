#include "../include/padrao.h"
#include "../include/funcoes.h"

void remover_newline(char *ptr) {
	while (*ptr) {
		if (*ptr == '\n') *ptr = 0;
		else ptr++;
	}
}

bool ler_linha(char * linha) {
	if (fgets(linha, TAM_LINHA, stdin) != NULL) {
        remover_newline(linha);
        return true;
    }
    return false;
}
