//fila.h
#include "types.h"

void push(TTOKEN **fila,TOKEN token);
void push_null(TTOKEN *fila);
void removeT(TTOKEN **fila);
int busca3(TTOKEN *fila,TOKEN token);
int busca(TTOKEN *fila,TOKEN token);
TOKEN busca2(TTOKEN *fila,TOKEN token);
int busca1(TTOKEN *fila,TOKEN token);

