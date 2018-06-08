//types.h
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
 
#ifndef DEFINICOES
#define DEFINICOES

int coluna,linha;

enum ID_Digito_Float {
	identificador = 100, digito_float = 200, digito_int = 300, caracter = 400, fim_de_arquivo = 500, erro = 404
};
enum oprelacional {
	MAigual = 10, Maior = 11, MEigual = 12, Menor = 13, igualigual = 14, diferente = 15
};
enum oparitimetico {
	adicao = 20, subtracao = 21, divisao = 22, multiplicacao = 23, igual = 24
};
enum especial {
	parenteses1 = 30, parenteses2 = 31, colchete1 = 32, colchete2 = 33, virgula = 34, pvirgula = 35
};
enum palreservada {
	_main = 40, _if = 41, _else = 42, _while = 43, _do = 44, _for = 45, _int = 46, _float = 47, _char = 48
};
 
typedef struct TOKEN {
	int tipo;
	char valor[50];
}TOKEN;
 
typedef struct TTOKEN {
	int tipo;
	char valor[50];
	struct TTOKEN *prox;
}TTOKEN;

#endif