#include "./include/tradutor.h"

int main() {
    
    cabecalho();
    
    // chamar uma função 
    call_funcao();

    return 0;
}

void cabecalho() {
    
    printf(".section rodata\n\n");
    printf(".data\n\n");
    printf(".text\n\n");
    
    return;

}