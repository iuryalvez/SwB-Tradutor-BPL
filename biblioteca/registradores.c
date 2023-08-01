#include <../include/registradores.h>

Registrador REGISTRADORES[MAX_REG];

void iniciar_registradores() {
    
    strcpy(REGISTRADORES[0].nome32, "eax");
    strcpy(REGISTRADORES[0].nome64, "rax");
    REGISTRADORES[0].tipo = CALLER_SAVED;
    REGISTRADORES[0].livre = true;

    strcpy(REGISTRADORES[1].nome32, "ebx");
    strcpy(REGISTRADORES[1].nome64, "rbx");
    REGISTRADORES[1].tipo = CALLEE_SAVED;
    REGISTRADORES[1].livre = true;
    
    strcpy(REGISTRADORES[2].nome32, "ecx");
    strcpy(REGISTRADORES[2].nome64, "rcx");
    REGISTRADORES[2].tipo = CALLER_SAVED;
    REGISTRADORES[2].livre = true;

    strcpy(REGISTRADORES[3].nome32, "edx");
    strcpy(REGISTRADORES[3].nome64, "rdx");
    REGISTRADORES[3].tipo = CALLER_SAVED;
    REGISTRADORES[3].livre = true;

    strcpy(REGISTRADORES[4].nome32, "esi");
    strcpy(REGISTRADORES[4].nome64, "rsi");
    REGISTRADORES[4].tipo = CALLER_SAVED;
    REGISTRADORES[4].livre = true;
    
    strcpy(REGISTRADORES[5].nome32, "edi");
    strcpy(REGISTRADORES[5].nome64, "rdi");
    REGISTRADORES[5].tipo = CALLER_SAVED;
    REGISTRADORES[5].livre = true;
    
    strcpy(REGISTRADORES[6].nome32, "ebp");
    strcpy(REGISTRADORES[6].nome64, "rbp");
    REGISTRADORES[6].tipo = CALLER_SAVED;
    REGISTRADORES[6].livre = true;
    
    strcpy(REGISTRADORES[7].nome32, "esp");
    strcpy(REGISTRADORES[7].nome64, "rsp");
    REGISTRADORES[7].tipo = CALLER_SAVED;
    REGISTRADORES[7].livre = true;

    strcpy(REGISTRADORES[8].nome32, "r8d");
    strcpy(REGISTRADORES[8].nome64, "r8");
    REGISTRADORES[8].tipo = CALLER_SAVED;
    REGISTRADORES[8].livre = true;
    
    strcpy(REGISTRADORES[9].nome32, "r9d");
    strcpy(REGISTRADORES[9].nome64, "r9");
    REGISTRADORES[9].tipo = CALLER_SAVED;
    REGISTRADORES[9].livre = true;
    
    strcpy(REGISTRADORES[10].nome32, "r10d");
    strcpy(REGISTRADORES[10].nome64, "r10");
    REGISTRADORES[10].tipo = CALLER_SAVED;
    REGISTRADORES[10].livre = true;
    
    strcpy(REGISTRADORES[11].nome32, "r11d");
    strcpy(REGISTRADORES[11].nome64, "r11");
    REGISTRADORES[11].tipo = CALLER_SAVED;
    REGISTRADORES[11].livre = true;
    
    strcpy(REGISTRADORES[12].nome32, "r12d");
    strcpy(REGISTRADORES[12].nome64, "r12");
    REGISTRADORES[12].tipo = CALLEE_SAVED;
    REGISTRADORES[12].livre = true;
    
    strcpy(REGISTRADORES[13].nome32, "r13d");
    strcpy(REGISTRADORES[13].nome64, "r13");
    REGISTRADORES[13].tipo = CALLEE_SAVED;
    REGISTRADORES[13].livre = true;
    
    strcpy(REGISTRADORES[14].nome32, "r14d");
    strcpy(REGISTRADORES[14].nome64, "r14");
    REGISTRADORES[14].tipo = CALLEE_SAVED;
    REGISTRADORES[14].livre = true;
    
    strcpy(REGISTRADORES[15].nome32, "r15d");
    strcpy(REGISTRADORES[15].nome64, "r15");
    REGISTRADORES[15].tipo = CALLEE_SAVED;
    REGISTRADORES[15].livre = true;

}
