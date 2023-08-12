#include "../include/registradores.h"

void iniciar_registradores(Registrador r[MAX_REG]) {
    
    strcpy(r[0].nome32, "eax");
    strcpy(r[0].nome64, "rax");
    r[0].livre = true;

    strcpy(r[1].nome32, "ebx");
    strcpy(r[1].nome64, "rbx");
    r[1].livre = true;
    
    strcpy(r[2].nome32, "ecx");
    strcpy(r[2].nome64, "rcx");
    r[2].livre = true;

    strcpy(r[3].nome32, "edx");
    strcpy(r[3].nome64, "rdx");
    r[3].livre = true;

    strcpy(r[4].nome32, "esi");
    strcpy(r[4].nome64, "rsi");
    r[4].livre = true;
    
    strcpy(r[5].nome32, "edi");
    strcpy(r[5].nome64, "rdi");
    r[5].livre = true;
    
    strcpy(r[6].nome32, "ebp");
    strcpy(r[6].nome64, "rbp");
    r[6].livre = true;
    
    strcpy(r[7].nome32, "esp");
    strcpy(r[7].nome64, "rsp");
    r[7].livre = true;

    strcpy(r[8].nome32, "r8d");
    strcpy(r[8].nome64, "r8");
    r[8].livre = true;
    
    strcpy(r[9].nome32, "r9d");
    strcpy(r[9].nome64, "r9");
    r[9].livre = true;
    
    strcpy(r[10].nome32, "r10d");
    strcpy(r[10].nome64, "r10");
    r[10].livre = true;
    
    strcpy(r[11].nome32, "r11d");
    strcpy(r[11].nome64, "r11");
    r[11].livre = true;
    
    strcpy(r[12].nome32, "r12d");
    strcpy(r[12].nome64, "r12");
    r[12].livre = true;
    
    strcpy(r[13].nome32, "r13d");
    strcpy(r[13].nome64, "r13");
    r[13].livre = true;
    
    strcpy(r[14].nome32, "r14d");
    strcpy(r[14].nome64, "r14");
    r[14].livre = true;
    
    strcpy(r[15].nome32, "r15d");
    strcpy(r[15].nome64, "r15");
    r[15].livre = true;

}
