#include <stdbool.h>
#include <string.h>

#define MAX_REG 16     // há 16 registradores
#define CALLER_SAVED 0 // tipo de registrador
#define CALLEE_SAVED 1 // tipo de registrador

struct registrador {
    char nome32[4]; // nome de 32 bits de um registrador 
    char nome64[4]; // nome de 64 bits de um registrador 
    int tipo;       // tipo de registrador 
    bool livre;     // disponibilidade do regisrador 
};

typedef struct registrador Registrador;

void iniciar_registradores();
