//parser.h
#include "types.h"


int variavel();

void atualizaT(TOKEN *token);
char * converte();
int TIPO( FILE *arq);
int DECL_VAR( FILE *arq);
int BLOCO( FILE *arq);
int COMANDO( FILE *arq);
int COMANDO_BASICO( FILE *arq);
int INTERACAO( FILE *arq);
int ATRIBUICAO( FILE *arq);
int EXPR_RELACIONAL( FILE *arq);
TOKEN EXPR_ARIT( FILE *arq);
TOKEN TERMO( FILE *arq);
TOKEN FATOR( FILE *arq);
void parser(FILE *arq);
