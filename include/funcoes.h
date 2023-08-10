#ifndef FUNCOES_H
#define FUNCOES_H

#include "padrao.h"
#include "registradores.h"
#include <stdbool.h>

// armazena informações padrões de uma variável local de pilha
struct variavel_local {
    int indice;
    int pos_pilha;
    int size;
};

typedef struct variavel_local vl;

/**
 * @brief Inicializa ou reinicializa todas as variaveis de controle
 * 
 * @param indice_funcao variavel de controle
 * @param nro_param variavel de controle
 * @param tipo_param variavel de controle
 * @param tampilha variavel de controle
 * @param vlp variavel de controle
 * @param ind_reg variavel de controle
 * @param cont_reg variavel de controle
 * @param cont_vlp variavel de controle
 */
void reinicializa_parametros(int *indice_funcao,int *nro_param, char * tipo_param, int *tampilha, vl * vlp, int * ind_reg, int * cont_reg, int * cont_vlp);

/**
 * @brief Printa todas as variaveis de controle
 * 
 * @param indice_funcao variavel de controle
 * @param nro_param variavel de controle
 * @param tipo_param variavel de controle
 * @param tampilha variavel de controle
 * @param vlp variavel de controle
 * @param ind_reg variavel de controle
 * @param cont_reg variavel de controle
 * @param cont_vlp variavel de controle
 */
void printacontrole(int indice_funcao, int nro_param, char * tipo_param, int tampilha, vl * vlp, int * ind_reg, int cont_reg, int cont_vlp);

/**
 * @brief salva os parâmetros de uma função acordo com sua quantidade
 * 
 * @param nro_param número de parâmetros da função
 * @param tampilha tamanho da pilha da função
 * @param r registradores 
 */
void salvar_parametros(int nro_param, int *tampilha, Registrador r[MAX_REG]);

/**
 * @brief recupera os parâmetros de uma função acordo com sua quantidade
 * 
 * @param nro_param número de parâmetros da função
 * @param tampilha tamanho da pilha da função
 * @param r registradores
 */
void recuperar_parametros(int nro_param, int *tampilha, Registrador r[MAX_REG]);

/**
 * @brief iniciar valores padrão (-1) no vetor de variáveis locais
 * 
 * @param vlp variáveis locais de pilha
 */
void iniciar_vlp(vl vlp[8]);

/**
 * @brief armazenamento de variáveis nos registradores 
 * 
 * @param ind_reg índice dos registradores
 * @param qtd_reg quantidade de variáveis de registradores
 * @param r registradores
 */
void armazenar_vr(int ind_reg[4], int qtd_reg, Registrador r[MAX_REG]);

/**
 * @brief armazenamento das variáveis locais da pilha
 * 
 * @param vlp variáveis locais de pilha
 * @param qtd_vlp quantidade de variáveis locais de pilha
 * @param tampilha tamanho da pilha
 */
void armazenar_vlp(vl vlp[8], int qtd_vlp, int tampilha);

/**
 * @brief alinha o tamanho da pilha para o múltiplo do alinhamento desejado
 *  
 * @param tampilha tamanho da pilha da função
 * @param alinhamento alinhamento desejado
 */
void alinhar_tampilha(int * tampilha, int alinhamento);

/**
 * @brief Recupera os registradores salvos
 * 
 * @param r Vetor de registradores
 * @param tampilha Posição final da pilha
 * @param salvos Vetor que possui o index dos registradores salvos
 */
void recuperar_reg(Registrador *r, int * tampilha, int * salvos);

/**
 * @brief Salva registradores
 * 
 * @param r Vetor de registradores
 * @param tampilha Posição final da pilha
 * @param salvos Vetor que possui o index dos registradores salvos
 */
void salvar_reg(Registrador *r, int * tampilha, int * salvos);

/**
 * @brief Multiplo de 16
 * 
 * @param x Numero analisado
 * @return int x é analisado, se for multiplo de 8, retorna o multiplo de 16 mais proximo acima,se for multiplo de 16 retorna o proprio x
 */
int multiplode16(int x);

/**
 * @brief inicializa o vetor de salvos
 * 
 * @param salvos Vetor que possui o index dos registradores salvos
 */
void inicializasalvos(int * salvos);

#endif
