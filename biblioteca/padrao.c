#include <../include/padrao.h>

void remover_newline(char *ptr) {
  while (*ptr) {
    if (*ptr == '\n')
      *ptr = 0;
    else
      ptr++;
  }
}
