#ifndef TRADUTOR_H
#define TRADUTOR_H

#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

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
int ler_linha(char *string);

typedef struct reg_param {
    char nome32[4]; // nome de 32 bits de um registrador 
    char nome64[4]; // nome de 64 bits de um registrador 
} Reg_param;

void iniciar_r(Reg_param r[3]);

// representa uma variávei local do tipo var
typedef struct Var{
    int ind; // índice da variável
    int pos; // posição na pilha da variável
} Var;

// representa uma variávei local do tipo vet
typedef struct vet {
    int ind; // índice da variável
    int pos; // posição na pilha da variável
    int size; // size do vet
} Vet;

// representa uma variávei local do tipo reg
typedef struct reg {
    int ind; // índice da variável
    int pos; // posição na pilha da variável
} Reg;

typedef struct typecharint {
    char x; // Char que define se é constante, parametro ou var;
    char type; // Char que armazena o tipo da variavel;
    int  index; // Indice da variavel;
} Typecharint;

typedef struct exp {
    char tipo1[2];
    char tipo2[2];
    int ind1;
    int ind2;
} Exp;

// representa um parâmetro da declaração de função
typedef struct param {
    char tipo;
    int pos;
} Param;

// representa todas as variáveis de declaração e locais que podem entrar na pilha
typedef struct pilha {
    Param param[3]; // variável que guarda os parâmetros da função 
    Var var[4];     // vetor que guarda as vars
    Vet vet[4];     // vetor que guarda os arrays
    Reg reg[4];     // vetor que guarda os registradores
    int var_qtd;    // quantidade de var
    int vet_qtd;    // quantidade de vet
    int reg_qtd;    // quantidade de reg
    int param_qtd;  // quantidade de parâmetros
    int rsp;        // registra o ponto atual da pilha
} Pilha;

/**
 * @brief exibe o salvamento dos parâmetros da função na pilha antes de uma chamada de função
 * 
 * @param pilha carrega todas as variáveis importantes da função
 */
void salvar_parametros(Pilha pilha);

/**
 * @brief exibe a recuperação dos parâmetros da função na pilha após a chamada de função
 * 
 * @param pilha carrega todas as variáveis importantes da função
 */
void recuperar_parametros(Pilha pilha);

/**
 * @brief inicializa os valores da variável pilha
 * 
 * @param pilha carrega todas as variáveis importantes da função
 */
void inicializar_pilha(Pilha *pilha);

/**
 * @brief salva as variáveis locais de pilha
 * 
 * @param pilha carrega todas as variáveis importantes da função
 */
void armazenar_pilha(Pilha *pilha);

/**
 * @brief exibe o salvamento das variáveis locais de pilha
 * 
 * @param pilha carrega todas as variáveis importantes da função
 */
void print_armazenamento(Pilha pilha);

/**
 * @brief exibe a recuperação das variáveis locais de pilha
 * 
 * @param pilha carrega todas as variáveis importantes da função
 */
void print_recuperacao(Pilha pilha);

/**
 * @brief alinha o tamanho de acordo com o alinhamento
 * 
 * @param tamanho ponteiro para variável que será alinhada
 * @param alinhamento múltiplo de alinhamento
 */
void alinhar(int *tamanho, int alinhamento);

/**
 * @brief Inicializa as informações dos parametros para call.
 * 
 * @param p Passa as informações dos parametros que irao ser atribuidos no call.
*/
void inicializar_parameters(Typecharint * p);

/**
 * @brief Printa a chamada de função
 * 
 * @param s Nome da função a ser chamada
 */
void callfuncao(char * s);

/**
 * @brief Função responsavel por atribuir os parametros para realizar a chamada de uma função
 * 
 * @param pilha Variaveis locais e posições na pilha
 * @param parameters Informações dos parametros lidos após o call
 * @param numero_args Quantidade de parametros lidos
 * @param registers_param Vetor de string que possui di, si, cx
 */
void atribui_call(Pilha pilha, Typecharint * parameters, int numero_args, char ** registers_param);

void expressions(Pilha pilha, char operator, Typecharint * parameters, int qtd, char ** registers_param);

int pegapos(Typecharint parameters, Pilha pilha, int * x);

int encontrar_indvet(Pilha pilha, int ind);

int encontrar_indvar(Pilha pilha, int ind);

int encontrar_indreg(Pilha pilha, int ind);

#endif
