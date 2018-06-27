//scanner.c
#include "types.h"
#include "scanner.h"

// Símbolos

// letra ::= [a-z]

// dígito ::= [0-9]

// id ::= (letra | "_") (letra | "_" | dígito)*

// oprelacional ::= <  |  >  |  <=  |  >=  |  ==  |  !=

// oparitmético ::= "+"  |  "-"  |  "*"  |  "/"  |  "="

// especial ::= ")"  |  "("  |  "{"  |  "}"  |  ","  |  ";"

// palreservada ::= main  |  if  |  else  |  while  |  do  |  for  |  int  |  float  |  char

// inteiro ::= dígito+

// float ::= dígito*.dígito+

// char ::= 'letra'  |  'dígito'         // Uma constante do tipo char (entre aspas simples)

TOKEN scanner(FILE *arq) {	
	int i;
	static char c;
 
	while (1) {
 
		c = fgetc(arq);
		while (c==' ') { c = fgetc(arq); }
 
		// ********************************************* OP RELACIONAL *********************************************
		if (c == '>') {
			c = fgetc(arq); coluna++;
			if (c == '=') {
				token.tipo = MAigual;
				return token;
			}else{
				ungetc(c,arq);
				token.tipo = Maior;
				return token;
			}
		}
		if (c == '<') {
			c = fgetc(arq); coluna++;
			if (c == '=') {
				token.tipo = MEigual;
				return token;
			}else{
				ungetc(c,arq);
				token.tipo = Menor;
				return token;
			}
		}
		if (c == '!') {
			c = fgetc(arq); coluna++;
			if (c == '=') {
				token.tipo = diferente;
				return token;
			}else{
				ungetc(c,arq);
				printf("(SCANNER)ERRO na linha %d, coluna %d, ultimo token lido:%c\nCaracter invalido!\n", linha, coluna, c);
				token.tipo = erro;
				return token;
			}
		}
 
		// // ********************************************* OP ARITIMETICO *********************************************
		if (c == '+') {token.tipo = adicao;return token;}
		if (c == '-') {token.tipo = subtracao;return token;}
		if (c == '*') {token.tipo = multiplicacao;return token;}
		if (c == '=') {
			c = fgetc(arq); coluna++;
			if (c == '=') {
				token.tipo = igualigual;
				return token;
			}else{
				ungetc(c,arq);
				token.tipo = igual;
				return token;
			}
		}
		if (c == '/') {
			c = fgetc(arq); coluna++;
			if (c == '/') {
				do {
					c = fgetc(arq); coluna++;
				} while (c != '\n' && !feof(arq));
				if (feof(arq)) {
					token.tipo=fim_de_arquivo;
					return token;
				}else {
					continue;
				}
			}else if (c == '*') {
				volta:do {
					c = fgetc(arq); coluna++;
				} while (c != '*' && !feof(arq));
				if (feof(arq)) {
						ungetc(c,arq);
						printf("(SCANNER)ERRO na linha %d, coluna %d, ultimo token lido :%c\nErro ao fechar comentario!\n", linha, coluna, c);
						token.tipo = erro;
						return token;
				}else if (c == '*') {
					c = fgetc(arq);
					if (feof(arq)) {
						ungetc(c,arq);
						printf("(SCANNER)ERRO na linha %d, coluna %d, ultimo token lido :%c\nErro ao fechar comentario!\n", linha, coluna, c);
						token.tipo = erro;
						return token;
					}else if (c == '/') {
						continue;
					}else{
						goto volta;
					}
				}
 
			}
			ungetc(c,arq);
			token.tipo = divisao;
			return token;
		}
 
		// // ********************************************* ESPECIAIS *********************************************
		if (c == '(') {token.tipo = parenteses1;return token;}
		if (c == ')') {token.tipo = parenteses2;return token;}
		if (c == '{') {token.tipo = colchete1;return token;}
		if (c == '}') {token.tipo = colchete2;return token;}
		if (c == ';') {token.tipo = pvirgula;return token;}
		if (c == ',') {token.tipo = virgula;return token;}
 		
		// ***************************** IDENTIFICADOR , DIGITOS , PALAVRA RESERVADA *****************************
		if (isalpha(c) || c == '_') {// IDENTIFICADOR
			for (i = 0; isalpha(c) || c == '_' || isalnum(c); i++) {
				token.valor[i] = c;
				c = fgetc(arq); coluna++;
			}
			ungetc(c,arq);
			token.valor[i]='\0';
		//  PALAVRA RESERVADA 
			if(strcmp(token.valor,"main")==0) token.tipo=_main;
			else if(strcmp(token.valor,"if")==0) token.tipo=_if;
			else if(strcmp(token.valor,"else")==0) token.tipo=_else;
			else if(strcmp(token.valor,"do")==0) token.tipo=_do;
			else if(strcmp(token.valor,"while")==0) token.tipo=_while;
			else if(strcmp(token.valor,"int")==0) token.tipo=_int;
			else if(strcmp(token.valor,"float")==0) token.tipo=_float;
			else if(strcmp(token.valor,"char")==0) token.tipo=_char;
			else token.tipo = identificador;
			return token;
		}

		if (isdigit(c)) {// DIGITOS
			for (i = 0; isalnum(c); i++) {
				token.valor[i] = c;
				c = fgetc(arq); coluna++;
			}
			if (c == '.') {
				token.valor[i] = '.';
				c = fgetc(arq); coluna++;
				if(isalnum(c)){
					for (i = ++i; isalnum(c); i++) {
					token.valor[i] = c;
					c = fgetc(arq); coluna++;
					}
					ungetc(c,arq);
					token.valor[i] = 0;token.valor[++i] = '\0';
					token.tipo = digito_float;
					return token;
				}else{
					ungetc(c,arq);
					printf("(SCANNER)ERRO na linha %d, coluna %d, ultimo token lido: %c\nFloat mal formulado!\n", linha, coluna, c);
					token.tipo = erro;
					return token;
				}
			}else{
				ungetc(c,arq);
				token.valor[i]='\0';
				token.tipo = digito_int;
				return token;
			}
		}
		if(c=='.'){// DIGITO FLOAT
			token.valor[0]='0';
			token.valor[1]='.';
			c=fgetc(arq);coluna++;
			if(!isdigit(c)){
				ungetc(c,arq);
				printf("(SCANNER)ERRO na linha %d, coluna %d, ultimo token lido:%c\nFloat mal formado!\n", linha, coluna, c);
				token.tipo = erro;
				return token;
			}else{
				i=2;
				while(isdigit(c)){
					token.valor[i]=c;c=fgetc(arq);coluna++;i++;
				}
				ungetc(c,arq);
				token.valor[i] = 0;token.valor[i] = '\0';
				token.tipo = digito_float;
				return token;
			}
		}


		if(c=='\''){// ASPAS SIMPLES 
			token.valor[0]='\'';
			c=fgetc(arq);coluna++;
			token.valor[1]=c;
			c=fgetc(arq);coluna++;
			if(c=='\''){
				token.valor[2]='\'';token.valor[3]='\0';
				token.tipo=caracter;
				return token;
			}else{
				ungetc(c,arq);
				printf("(SCANNER)ERRO na linha %d, coluna %d, ultimo token lido:%c\nErro ao indentificar um caracter!\n", linha, coluna, c);
				token.tipo = erro;
				return token;
			}
 
		}
 
		// FIM DE ARQUIVO
		if (feof(arq)) {
			token.tipo = fim_de_arquivo;
			return token;
		}
 
		// \n \r \t
		if(c=='\r'){
			coluna=0;
		}else if(c=='\t'){
			coluna = coluna + 4;
		}else if (c == '\n') {
			linha++; coluna = 0;
		}else{// CARACTER INVALIDO
			printf("(SCANNER)\nCaracter nao existente na linguagem!\n");
			printf("(SCANNER)ERRO na linha %d, coluna %d, ultimo token lido:%c\n", linha, coluna, c);
			token.tipo = erro;
			return token;
		}
	}
}

void printar(TOKEN token) {
	if (token.tipo == 500)printf("(SCANNER)tipo:%d (fim de arquivo)\n", fim_de_arquivo);
	if (token.tipo == 404)printf("(SCANNER)tipo:%d (erro)\n", erro);
	if (token.tipo == 10)printf("(SCANNER)tipo:%d (Maior Igual)\n", MAigual);
	if (token.tipo == 11)printf("(SCANNER)tipo:%d (Maior)\n", Maior);
	if (token.tipo == 12)printf("(SCANNER)tipo:%d (Menor Igual)\n", MEigual);
	if (token.tipo == 13)printf("(SCANNER)tipo:%d (Menor)\n", Menor);
	if (token.tipo == 14)printf("(SCANNER)tipo:%d (Igual Igual)\n", igualigual);
	if (token.tipo == 15)printf("(SCANNER)tipo:%d (Diferente)\n", diferente);
	if (token.tipo == 20)printf("(SCANNER)tipo:%d (Adicao)\n", adicao);
	if (token.tipo == 21)printf("(SCANNER)tipo:%d (Subtracao)\n", subtracao);
	if (token.tipo == 22)printf("(SCANNER)tipo:%d (Divisao)\n", divisao);
	if (token.tipo == 23)printf("(SCANNER)tipo:%d (multiplicacao)\n", multiplicacao);
	if (token.tipo == 24)printf("(SCANNER)tipo:%d (Igual)\n", igual);
	if (token.tipo == 30)printf("(SCANNER)tipo:%d (Abre Parenteses)\n", parenteses1);
	if (token.tipo == 31)printf("(SCANNER)tipo:%d (Fecha Parenteses)\n", parenteses2);
	if (token.tipo == 32)printf("(SCANNER)tipo:%d (Abre Chaves)\n", colchete1);
	if (token.tipo == 33)printf("(SCANNER)tipo:%d (Fecha Chaves)\n", colchete2);
	if (token.tipo == 34)printf("(SCANNER)tipo:%d (Virgula)\n", virgula);
	if (token.tipo == 35)printf("(SCANNER)tipo:%d (Ponto e Virgula)\n", pvirgula);
	if (token.tipo == 40)printf("(SCANNER)tipo:%d (main)\n", _main);
	if (token.tipo == 41)printf("(SCANNER)tipo:%d (if)\n", _if);
	if (token.tipo == 42)printf("(SCANNER)tipo:%d (else)\n", _else);
	if (token.tipo == 43)printf("(SCANNER)tipo:%d (while)\n", _while);
	if (token.tipo == 44)printf("(SCANNER)tipo:%d (do)\n", _do);
	if (token.tipo == 45)printf("(SCANNER)tipo:%d (for)\n", _for);
	if (token.tipo == 46)printf("(SCANNER)tipo:%d (int)\n", _int);
	if (token.tipo == 47)printf("(SCANNER)tipo:%d (float)\n", _float);
	if (token.tipo == 48)printf("(SCANNER)tipo:%d (char)\n", _char);
	if (token.tipo == 100)printf("(SCANNER)tipo:%d (Identificador)\nlexema:%s\n", identificador, token.valor);
	if (token.tipo == 200)printf("(SCANNER)tipo:%d (Digito float)\nlexema:%s\n", digito_float, token.valor);
	if (token.tipo == 300)printf("(SCANNER)tipo:%d (Digito int)\nlexema:%s\n", digito_int, token.valor);
	if (token.tipo == 400)printf("(SCANNER)tipo:%d (Caracter)\nlexema:%s\n", caracter, token.valor);
}

void printEnum(){
	printf("%d - Fim de arquivo\t\t%d - ERRO\n",fim_de_arquivo,erro);
	printf("%d - Maior igual\t\t%d - Maior\t\t%d - Menor igual\n",MAigual,Maior,MEigual);
	printf("%d - Menor\t\t\t%d - Igual igual\t%d - Diferente\n",Menor,igualigual,diferente);
	printf("%d - Adicao\t\t\t%d - Subtracao\t\t%d - Divisao\n",adicao,subtracao,divisao);
	printf("%d - Multiplicacao\t\t%d - Igual\t\t%d - Abre parenteses\n",multiplicacao,igual,parenteses1);
	printf("%d - Fecha parenteses\t\t%d - Abre chaves\t%d - Fecha chaves\n",parenteses2,colchete1,colchete2);
	printf("%d - Virgula\t\t\t%d - Ponto e virgula\t%d - Main\n",virgula,pvirgula,_main);
	printf("%d - If\t\t\t\t%d - Else\t\t%d - While\n",_if,_else,_while);
	printf("%d - Do\t\t\t\t%d - For\t\t%d - Int\n",_do,_for,_int);
	printf("%d - Float\t\t\t%d - Char\t\t%d - Identificador\n",_float,_char,identificador);
	printf("%d - Digito float\t\t%d - Digito int\t%d - Caracter\n\n",digito_float,digito_int,caracter);
}
 