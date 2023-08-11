#ifndef REGISTRADORES_H
#define REGISTRADORES_H

#include <stdbool.h>
#include <string.h>

#define MAX_REG 16     // há 16 registradores

typedef struct registrador {
    char nome32[4]; // nome de 32 bits de um registrador 
    char nome64[4]; // nome de 64 bits de um registrador 
    bool livre;     // disponibilidade do regisrador 
} Registrador;

void iniciar_registradores(Registrador r[MAX_REG]);

#endif
