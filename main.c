//main.c
#include "types.h"
#include "scanner.h"
#include "parser.h"

// salvar na internet 

FILE * criarArquivo(char nome[]) {
	FILE * arq;
	arq = fopen(nome, "r+b");
	if (arq == NULL) {
		arq = fopen(nome, "w+b");
	}
	return arq;
}

 ///PARA PARCER
int main(int argc,char *argv[]) {
	TOKEN token;
	FILE *arq;
	linha =1;
	coluna=0;
	printEnum();
	if (argc > 2) {
		printf("Quantidade de parametros incorreta!\n");
	}
	else {
		arq = criarArquivo("teste.txt");
		parser(arq);
	}
	return 0;
}

//PARA SCANNER
/*
int	main(int argc, char *argv[]) {
	TOKEN token;
	int i;
	printEnum();
	FILE *arq = criarArquivo("teste.txt");//argv[1]
	if (argc > 2) {//argc != 2
		printf("Quantidade de parametros incorreta!\n");
	}
	else {
		while (!feof(arq)) {
			token = scanner(arq);
			printar(token);
		}
	}
	system("pause");
	return 0;
}
*/
