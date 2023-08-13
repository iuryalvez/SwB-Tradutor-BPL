#ifndef PADRAO_H
#define PADRAO_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "funcoes.h"

#define TAM_LINHA 256

/**
 * @brief remove /n do final da linha
 * 
 * @param ptr variável que percorre a linha até encontrar o /n
 */
void remover_newline(char *ptr);

/**
 * @brief lê uma nova linha
 * 
 * @return verdadeiro ou falso
 */
bool ler_linha(char *string);

#endif
