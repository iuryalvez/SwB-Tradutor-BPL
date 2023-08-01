#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINESZ 256

// Remove o '\n' do fim da linha
void remove_newline(char *ptr)
{
  while (*ptr) {
    if (*ptr == '\n')
      *ptr = 0;
    else
      ptr++;
  }
}

int main()
{
  char v1;
  int r, i1, i2;
  int inicio, fim, passo;
  char line[LINESZ];
  int count = 0;

  // Lê uma linha por vez
  while (fgets(line, LINESZ, stdin) != NULL) {
    count++;
    remove_newline(line);

    // Verifica se line começa com 'end' (3 letras)
    if (strncmp(line, "end", 3) == 0) {
      printf("Linha %d: %s\n", count, line);
      printf("Fim do arquivo\n");
      printf("---\n");	      
      continue;
    }

    // Verifica se é 'for' e quanto elementos possui
    r = sscanf(line, "for %c = %d, %d, %d", &v1, &inicio, &fim, &passo);
    if (r == 4) {
      printf("Linha %d: %s\n", count, line);
      printf("Lido '%c' valores: %d, %d, %d\n", v1, inicio, fim, passo);
      printf("---\n");
      continue;
    }
    if (r == 3) {
      printf("Linha %d: %s\n", count, line);
      printf("Lido '%c' valores: %d, %d\n", v1, inicio, fim);
      printf("---\n");
      continue;
    }
    if (r == 2) {
      printf("Linha %d: %s\n", count, line);
      printf("Lido '%c' valor: %d\n", v1, inicio);
      printf("---\n");
      continue;
    }
    
    // Verifica se é um 'if'
    r = sscanf(line, "if v%d > v%d", &i1, &i2);
    if (r == 2) {
      printf("Linha %d: %s\n", count, line);
      printf("Indices: %d e %d\n", i1, i2);
      printf("---\n");
      continue;
    }
  }

  return 0;
}
