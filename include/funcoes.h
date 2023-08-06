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
 * @brief salva os parâmetros de uma função acordo com sua quantidade
 * 
 * @param nro_param número de parâmetros da função
 * @param tampilha tamanho da pilha da função
 * @param r registradores da função
 */
void salvar_parametros(int nro_param, int *tampilha, Registrador r[16]);

/**
 * @brief recupera os parâmetros de uma função acordo com sua quantidade
 * 
 * @param nro_param número de parâmetros da função
 * @param tampilha tamanho da pilha da função
 * @param r registradores da função
 */
void recuperar_parametros(int nro_param, int *tampilha, Registrador r[16]);

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
 */
void armazenar_reg(int ind_reg[4], int qtd_reg);

/**
 * @brief armazenamento das variáveis locais da pilha
 * 
 * @param vls variáveis locais de pilha
 * @param qtd_vls quantidade de variáveis locais de pilha
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
