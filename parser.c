//parser.c
// PARSER / SEMANTICO / GERADOR DE CODIGO
#include "types.h"
#include "scanner.h"
#include "parser.h"
#include "fila.h"

char T[50];
int temporalrelacional=0,temp1=0;
int temporal=0,label=0;

TTOKEN *fila=NULL;
/*
<programa> ::= int main"("")" <bloco>                                                                       |precisa X
<bloco> ::= �{� {<decl_var>}* {<comando>}* �}�                                                              |
<comando> ::= <comando_b�sico> | <itera��o> | if "("<expr_relacional>")" <comando> {else <comando>}?        |precisa X
<comando_b�sico> ::= <atribui��o> | <bloco>                                                                 |
<itera��o> ::= while "("<expr_relacional>")" <comando> | do <comando> while "("<expr_relacional>")"";"      |precisa X
<atribui��o> ::= <id> "=" <expr_arit> ";"                                                                   |precisa X
<expr_relacional> ::= <expr_arit> <op_relacional> <expr_arit>                                               |precisa X
<expr_arit> ::= <expr_arit> "+" <termo>   | <expr_arit> "-" <termo> | <termo>                               |precisa 
<termo> ::= <termo> "*" <fator> | <termo> �/� <fator> | <fator>                                             |precisa X
<fator> ::= �(� <expr_arit> �)� | <id> | <real> | <inteiro> | <char>                                        |precisa x
<decl_var> ::= <tipo> <id> {,<id>}* ";"                                                                     |
<tipo> ::= int | float | char                                                                               |

printf("ERRO na linha %d, coluna %d, ultimo token lido (%d): \n !\n", linha, coluna, token.tipo); return 0;
*/

//*************************************************************************************************************************
//Ctrl + K Ctrl + C / Ctrl K + Ctrl + U
char *converte(){
	sprintf(T,"T%d",temporal);
	temporal++;return T;
}

void atualizaT(TOKEN *token){
	strcpy((*token).valor,T);
}
//*************************************************************************************************************************


int TIPO( FILE *arq) {// <tipo> ::= int | float | char
	if (token.tipo == _int || token.tipo == _float || token.tipo == _char) {
		token=scanner(arq);return 1;
	}
	else {
		printf("(SEMANTICO)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTipo invalido!\n", linha, coluna, token.tipo); return 0;
	}
}

int DECL_VAR(FILE *arq) {// <decl_var> ::= <tipo> <id> {,<id>}* ";"
	int resp;
	if (token.tipo == _int ) {
loop_DECL_VAR_int:
		token = scanner(arq);
		if (token.tipo == identificador) {
			resp=busca1(fila,token);
			if(resp==1){
				printf("(SEMANTICO)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nVariavel ja declarada!\n", linha, coluna, token.tipo); return 0;
			}else{
				token.tipo=_int;
				push(&fila,token);
			}
			token = scanner(arq);
			if (token.tipo == pvirgula){
				token=scanner(arq);
				return 1;
			}
			else if (token.tipo == virgula) {
				goto loop_DECL_VAR_int;
			}
			else {
				printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de ',' ou ';' na declaracao de variavel!\n", linha, coluna, token.tipo); return 0;
			}
		}
		else {
			printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nNao foi identificado a variavel!\n", linha, coluna, token.tipo); return 0;
		}
	}
	else if ( token.tipo == _float ) {
loop_DECL_VAR_float:
		token = scanner(arq);
		if (token.tipo == identificador) {
			resp=busca1(fila,token);
			if(resp==1){
				printf("(SEMANTICO)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nVariavel ja declarada!\n", linha, coluna, token.tipo); return 0;
			}else{
				token.tipo=_float;
				push(&fila,token);
			}
			token = scanner(arq);
			if (token.tipo == pvirgula){
				token=scanner(arq);
				return 1;
			}
			else if (token.tipo == virgula) {
				goto loop_DECL_VAR_float;
			}
			else {
				printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de ',' ou ';' na declaracao de variavel!\n", linha, coluna, token.tipo); return 0;
			}
		}
		else {
			printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nNao foi identificado a variavel!\n", linha, coluna, token.tipo); return 0;
		}
	}
	else if ( token.tipo == _char) {
loop_DECL_VAR_char:
		token = scanner(arq);
		if (token.tipo == identificador) {
			resp=busca1(fila,token);
			if(resp==1){
				printf("(SEMANTICO)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nVariavel ja declarada!\n", linha, coluna, token.tipo); return 0;
			}else{
				token.tipo=_char;
				push(&fila,token);
			}
			token = scanner(arq);
			if (token.tipo == pvirgula){
				token=scanner(arq);
				return 1;
			}
			else if (token.tipo == virgula) {
				goto loop_DECL_VAR_char;
			}
			else {
				printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de ',' ou ';' na declaracao de variavel!\n", linha, coluna, token.tipo); return 0;
			}
		}
		else {
			printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nNao foi identificado a variavel!\n", linha, coluna, token.tipo); return 0;
		}
	}
	else{
		printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d):Falta de um tipo para declaracao de variavel\n !\n",linha,coluna,token.tipo);return 0;
	}
}

int BLOCO(FILE *arq) {// <bloco> ::= “{“ {<decl_var>}* {<comando>}* “}”
	int status;
	if (token.tipo == colchete1) {
		token.tipo=0;token.valor[0]='\0';
		push(&fila,token);
		token = scanner(arq);
		if (token.tipo != _int && token.tipo != _float && token.tipo != _char && token.tipo != identificador && token.tipo != colchete1 && token.tipo != _while && token.tipo != _do && token.tipo != _if && token.tipo!=colchete2) {
			printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nToken nao condigente com o sintaxe do bloco!\n", linha, coluna, token.tipo); return 0;
		}else{
			while (token.tipo == _int || token.tipo == _float || token.tipo == _char) {
				status = DECL_VAR(arq);
				if (status == 0) {
					return 0;
				}
			}
		}		
		if (token.tipo != identificador && token.tipo != colchete1 && token.tipo != _while && token.tipo != _do && token.tipo != _if && token.tipo != colchete2) {
			printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nToken nao condigente com o sintaxe do bloco!\n", linha, coluna, token.tipo); return 0;
		}else if (token.tipo == colchete2) {
			removeT(&fila);
			token = scanner(arq);
			return 1;
		}else {
			while (token.tipo == identificador || token.tipo == colchete1 || token.tipo == _while || token.tipo == _do || token.tipo == _if) {
				status = COMANDO(arq);
				if (status == 0) {
					return 0;
				}
			}
		}
		if (token.tipo == colchete2) {
			removeT(&fila);
			token = scanner(arq);
			return 1;
		}else {
			printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de um '}' no final do bloco!\n", linha, coluna, token.tipo); return 0;
		}
	}else {
		printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nErro falta de '{' no incio do bloco!\n", linha, coluna, token.tipo); return 0;
	}
}

int COMANDO(FILE *arq) {// <comando> ::= <comando_básico> | <iteração> | if "("<expr_relacional>")" <comando> {else <comando>}?
	int status,label1=label;
	if (token.tipo == identificador || token.tipo == colchete1) {
		status=COMANDO_BASICO(arq);
		if (status == 0) {
			return 0;
		}
		else {
			return 1;
		}
	}else if (token.tipo == _while || token.tipo == _do) {
		status = INTERACAO(arq);
		if (status == 0) {
			return 0;
		}
		else {
			return 1;
		}
	}else if (token.tipo == _if) {
		token = scanner(arq);
		printf("\t(if)\n");
		if (token.tipo == parenteses1) {
			token = scanner(arq);
			printf("\t(inicio expr_rela)\n");
			status = EXPR_RELACIONAL(arq);
			if (status == 0) {
				return 0;
			}
			else {
				printf("\t(fim expr_rela)\n");
				if (token.tipo == parenteses2) {
					token = scanner(arq);
					printf("\tIF T0 == FALSE goto L%d\n",label1);label++;label++;temporal=0;
					status = COMANDO(arq);
					if (status == 0) {
						return 0;
					}else{
						if (token.tipo == _else) {
							token = scanner(arq);
							printf("\tgoto L%d\n",label1+1);
							printf("\t(else)\n");
							printf("L%d:\n",label1);
							status = COMANDO( arq);
							if (status == 0) {
								return 0;
							}
							else {
								label1++;
								printf("L%d:\n",label1);
								return 1;
							}
						}
						else {
							printf("L%d:\n",label1);
							return 1;
						}
					}
				}
				else {
					printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de um fecha parenteses!\n", linha, coluna, token.tipo); return 0;
				}
			}
		}
		else {
			printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de abre parenteses!\n", linha, coluna, token.tipo); return 0;
		}
	}
	else {
		printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nErro comando nao reconhecido!\n", linha, coluna, token.tipo); return 0;
	}
}

int COMANDO_BASICO(FILE *arq) {// <comando_básico> ::= <atribuição> | <bloco>
	int status;
	if (token.tipo == identificador) {
		status=ATRIBUICAO( arq);
		if (status == 0) {
			return 0;
		}
		else {
			return 1;
		}
	}else if (token.tipo == colchete1) {
		status = BLOCO( arq);
		if (status == 0) {
			return 0;
		}
		else {
			return 1;
		}
	}
	else {
		printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nErro na escrita do comando basico!\n", linha, coluna, token.tipo); return 0;
	}
}

int INTERACAO(FILE *arq) {// <iteração> ::= while "("<expr_relacional>")" <comando> | do <comando> while "("<expr_relacional>")"";"
	int status,label1=label;
	if (token.tipo == _while) {
		label++;label++;
		token = scanner(arq);
		printf("\t(while)\n");
		printf("L%d:\n",label1);
		if (token.tipo == parenteses1) {
			token = scanner(arq);
			printf("\t(inicio expr_rela)\n");
			status = EXPR_RELACIONAL(arq);
			if (status == 0) {
				return 0;
			}
			else {
				printf("\t(fim expr_rela)\n");
				if (token.tipo == parenteses2) {
					token = scanner(arq);
					printf("\tIF T0 == FALSE goto L%d\n",label1+1);temporal=0;
					status=COMANDO( arq);
					if (status == 0) {
						return 0;
					}
					else {
						printf("\tgoto L%d\n",label1);label1++;
						printf("L%d:\n",label1);label1++;
						return 1;
					}
				}
				else {
					printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de um fecha parenteses!\n", linha, coluna, token.tipo); return 0;
				}
			}
		}
		else {
			printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta do abre parenteses!\n", linha, coluna, token.tipo); return 0;
		}
	}else if(token.tipo==_do){
		label++;
		printf("\t(do)\n");
		printf("L%d:\n",label1);
		token = scanner(arq);
		status=COMANDO( arq);
		if (status == 0) {
			return 0;
		}
		else {
			if (token.tipo == _while) {
				token = scanner(arq);
				if (token.tipo == parenteses1) {
					token = scanner(arq);
					printf("\t(while)\n");
					printf("\t(inicio expr_rela)\n");
					status = EXPR_RELACIONAL( arq);
					if (status == 0) {
						return 0;
					}
					else {
						printf("\t(fim expr_rela)\n");
						if (token.tipo == parenteses2) {
							token = scanner(arq);
							if (token.tipo == pvirgula) {
								token=scanner(arq);
								printf("\tIF T0 != FALSE goto L%d\n",label1);temporal=0;
								return 1;
							}
							else {
								printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de um ';' no final do interacao!\n", linha, coluna, token.tipo); return 0;
							}
						}
						else {
							printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de um fecha parenteses!\n", linha, coluna, token.tipo); return 0;
						}
					}
				}
				else {
					printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de abre parenteses!\n", linha, coluna, token.tipo); return 0;
				}
			}
			else {
				printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de um 'while'!\n", linha, coluna, token.tipo); return 0;
			}
		}
	}
	else {
		printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de 'do' ou 'while' para a formacao da interacao!\n", linha, coluna, token.tipo); return 0;
	}
}

int ATRIBUICAO(FILE *arq) {// <atribuição> ::= <id> "=" <expr_arit> ";"
	TOKEN aux,status;
	int resp;
	if (token.tipo != identificador) {
		printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \n Identificador nao identificado!\n", linha, coluna, token.tipo); return 0;
	}
	else {
		resp=busca(fila,token);
		if(resp==0){
			printf("(SEMANTICO)ERRO na linha %d, coluna %d, ultimo token lido (%d): \n Variavel nao declarada!\n", linha, coluna, token.tipo); return 0;
		}
		aux=busca2(fila,token);
		token = scanner(arq);
		if (token.tipo != igual) {
			printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nO '=' nao foi colocado na atribuicao!\n", linha, coluna, token.tipo); return 0;
		}
		else {
			token=scanner(arq);
			status = EXPR_ARIT(arq);
			if (status.tipo != 0) {
				if(((aux.tipo==_int || aux.tipo==digito_int) && (status.tipo==_int || status.tipo==digito_int)) || ((aux.tipo==_float || aux.tipo==digito_float) && (status.tipo==_float || status.tipo==digito_float || status.tipo==digito_int || status.tipo== _int)) || (aux.tipo == _char && status.tipo==_char)){
					if((status.tipo==_int || status.tipo==digito_int)&&(aux.tipo==_float || aux.tipo==digito_float)){
						printf("\t%s = float %s\n",converte(),status.valor);atualizaT(&status);
					}
					printf("\t%s = %s\n",aux.valor,status.valor);
					if (token.tipo != pvirgula) {
						printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de um ';' no fim do comando!\n", linha, coluna, token.tipo); return 0;
					}else{
						token=scanner(arq);
						return 1;
					}
				}else{
					printf("(SEMANTICO)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nErro de compatibilidade na atribuicao!\n", linha, coluna, token.tipo); return 0;
				}
			}
			else {
				return 0;
			}
		}
	}
}

int EXPR_RELACIONAL(FILE *arq) {// <expr_relacional> ::= <expr_arit> <op_relacional> <expr_arit>
	TOKEN status;
	TOKEN tokenRetorno,aux;
	char relacional[3];
	tokenRetorno.tipo=0;tokenRetorno.valor[0]='\0';
	status=EXPR_ARIT(arq);
	if (status.tipo == 0) {
		return 0;
	}
	else {
		if (token.tipo == Maior || token.tipo == Menor || token.tipo == MAigual || token.tipo == MEigual || token.tipo == igualigual || token.tipo == diferente) {
			if(token.tipo == Maior) strcpy(relacional,">");
			if(token.tipo == Menor) strcpy(relacional,"<");
			if(token.tipo == MAigual) strcpy(relacional,">=");
			if(token.tipo == MEigual) strcpy(relacional,"<=");
			if(token.tipo == igualigual) strcpy(relacional,"==");
			if(token.tipo == diferente) strcpy(relacional,"!=");
			token=scanner(arq);
			aux=EXPR_ARIT(arq);
			if (aux.tipo == 0) {
				return 0;
			}
			else {
				if(((aux.tipo==_float || aux.tipo==digito_float || aux.tipo==digito_int || aux.tipo== _int) && (status.tipo==_float || status.tipo==digito_float || status.tipo==digito_int || status.tipo== _int)) || (aux.tipo == _char && status.tipo==_char)){
					if((aux.tipo ==  _float || aux.tipo == digito_float)&&(status.tipo==_int || status.tipo==digito_int)){
						printf("\t%s = float %s\n",converte(),status.valor);atualizaT(&status);
					}
					if((status.tipo ==  _float || status.tipo == digito_float)&&(aux.tipo==_int || aux.tipo==digito_int)){
						printf("\t%s = float %s\n",converte(),aux.valor);atualizaT(&aux);
					}
					printf("\tT0 = %s %s %s\n",status.valor,relacional,aux.valor);
					return 1;
				}else{ 
					printf("(SEMANTICO)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nErro de compatibilidade de tipo na expressao aritimetica!\n", linha, coluna, token.tipo); return 0;
				}
			}
		}
		else {
			printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de um operador relacional valido!\n", linha, coluna, token.tipo); return 0;
		}
	}
}

TOKEN EXPR_ARIT(FILE *arq){// <expr_arit> ::= <expr_arit> "+" <termo>   | <expr_arit> "-" <termo> | <termo>
	TOKEN status,status2;
	TOKEN tokenRetorno;
	int tipo;
	tokenRetorno.tipo=0;tokenRetorno.valor[0]='\0';
	if (token.tipo == identificador || token.tipo == digito_float || token.tipo == digito_int || token.tipo == caracter || token.tipo == parenteses1){	
		status=TERMO(arq);
		if(status.tipo==0){
			return tokenRetorno;
		}else{
			while(token.tipo ==  adicao || token.tipo == subtracao){
				tipo=token.tipo;
				token=scanner(arq);
				if (token.tipo == identificador || token.tipo == digito_float || token.tipo == digito_int || token.tipo == caracter || token.tipo == parenteses1){	
					status2=TERMO(arq);
					if(status2.tipo==0)
						return tokenRetorno;
					else{
						if((status.tipo == _char)&&(status2.tipo != _char)){
							printf("(SEMANTICO)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTipo nao compativel com char!\n", linha, coluna, token.tipo); return tokenRetorno;
						}
						if((status.tipo == _float ||  status.tipo == digito_float)&&(status2.tipo != _int && status2.tipo != _float && status2.tipo !=  digito_int && status2.tipo != digito_float)){
							printf("(SEMANTICO)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTipo nao compativel com int ou float!\n", linha, coluna, token.tipo); return tokenRetorno;
						}
						if((status.tipo == _int || status.tipo == digito_int)&&(status2.tipo != _int  && status2.tipo !=  digito_int && status2.tipo != _float && status2.tipo != digito_float)){
							printf("(SEMANTICO)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTipo nao compativel com int!\n", linha, coluna, token.tipo); return tokenRetorno;
						}
						if(tipo == adicao){
							if((status.tipo==_int || status.tipo==digito_int)&&(status2.tipo==_float || status2.tipo==digito_float)){
								printf("\t%s = float %s\n",converte(),status.valor);atualizaT(&status);status.tipo=_float;
							}
							if((status2.tipo==_int || status2.tipo==digito_int)&&(status.tipo==_float || status.tipo==digito_float)){
								printf("\t%s = float %s\n",converte(),status2.valor);atualizaT(&status2);status.tipo=_float;
							}
							printf("\t%s = %s + %s\n",converte(),status.valor,status2.valor);atualizaT(&status);
						}else{
							if((status.tipo==_int || status.tipo==digito_int)&&(status2.tipo==_float || status2.tipo==digito_float)){
								printf("\t%s = float %s\n",converte(),status.valor);atualizaT(&status);status.tipo=_float;
							}
							if((status2.tipo==_int || status2.tipo==digito_int)&&(status.tipo==_float || status.tipo==digito_float)){
								printf("\t%s = float %s\n",converte(),status2.valor);atualizaT(&status2);status.tipo=_float;
							}
							printf("\t%s = %s - %s\n",converte(),status.valor,status2.valor);atualizaT(&status);
						}
					}
				}else {
					printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nExpr_arit incorreta, falta de um identificador,numero ou caracter!\n", linha, coluna, token.tipo); return tokenRetorno;
				}
			}
			return status;
		}
	}else {
		printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nExpr_art incorreto, falta de um identificador,numero ou caracter!\n", linha, coluna, token.tipo); return tokenRetorno;
	}

}


TOKEN TERMO(FILE *arq){// <termo> ::= <termo> "*" <fator> | <termo> “/” <fator> | <fator>
	TOKEN status,status2;
	TOKEN tokenRetorno;
	int tipo;
	tokenRetorno.tipo=0;tokenRetorno.valor[0]='\0';
	if (token.tipo == identificador || token.tipo == digito_float || token.tipo == digito_int || token.tipo == caracter || token.tipo == parenteses1){	
		status=FATOR(arq);
		if(status.tipo==0){
			return tokenRetorno;
		}else{
			while(token.tipo ==  multiplicacao || token.tipo == divisao){
				tipo=token.tipo;
				token=scanner(arq);
				if (token.tipo == identificador || token.tipo == digito_float || token.tipo == digito_int || token.tipo == caracter || token.tipo == parenteses1){	
					status2=FATOR(arq);
					if(status2.tipo==0)
						return tokenRetorno;
					else{
						if((status.tipo == _char)&&(status2.tipo != _char)){
							printf("(SEMANTICO)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTipo nao compativel com char!\n", linha, coluna, token.tipo); return tokenRetorno;
						}
						if((status.tipo == _float ||  status.tipo == digito_float)&&(status2.tipo != _int && status2.tipo != _float && status2.tipo !=  digito_int && status2.tipo != digito_float)){
							printf("(SEMANTICO)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTipo nao compativel com int ou float!\n", linha, coluna, token.tipo); return tokenRetorno;
						}
						if((status.tipo == _int || status.tipo == digito_int)&&(status2.tipo != _int  && status2.tipo !=  digito_int && status2.tipo != _float && status2.tipo != digito_float)){
							printf("(SEMANTICO)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTipo nao compativel com int!\n", linha, coluna, token.tipo); return tokenRetorno;
						}
						if(tipo == multiplicacao){
							if((status.tipo==_int || status.tipo==digito_int)&&(status2.tipo==_float || status2.tipo==digito_float)){
								printf("\t%s = float %s\n",converte(),status.valor);atualizaT(&status);status.tipo=_float;
							}
							if((status2.tipo==_int || status2.tipo==digito_int)&&(status.tipo==_float || status.tipo==digito_float)){
								printf("\t%s = float %s\n",converte(),status2.valor);atualizaT(&status2);status.tipo=_float;
							}
							printf("\t%s = %s * %s\n",converte(),status.valor,status2.valor);atualizaT(&status);
						}else{
							if(status.tipo==_int || status.tipo==digito_int){
								printf("\t%s = float %s\n",converte(),status.valor);atualizaT(&status);status.tipo=_float;
							}
							if(status2.tipo==_int || status2.tipo==digito_int){
								printf("\t%s = float %s\n",converte(),status2.valor);atualizaT(&status2);status.tipo=_float;
							}
							printf("\t%s = %s / %s\n",converte(),status.valor,status2.valor);atualizaT(&status);
						}
					}
				}else {
					printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTermo incorreto, falta de um identificador,numero ou caracter!\n", linha, coluna, token.tipo); return tokenRetorno;
				}
			}
			return status;
		}
	}else {
		printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTermo incorreto, falta de um identificador,numero ou caracter!\n", linha, coluna, token.tipo); return tokenRetorno;
	}

}

TOKEN FATOR(FILE *arq) {// <fator> ::= “(“ <expr_arit> “)” | <id> | <real> | <inteiro> | <char>
	TOKEN status;
	TOKEN tokenRetorno;
	char c[4];
	tokenRetorno.tipo=0;tokenRetorno.valor[0]='\0';
	if (token.tipo == parenteses1) {
		token=scanner(arq);
		status=EXPR_ARIT(arq);
		if (status.tipo == 0) {
			return tokenRetorno;
		}
		else {
			if (token.tipo != parenteses2) {
				printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta do ')' no fim do fator!\n", linha, coluna, token.tipo); return tokenRetorno;
			}
			else {
				token=scanner(arq);
				return status;
			}
		}
	}
	else if (token.tipo == identificador || token.tipo == digito_float || token.tipo == digito_int || token.tipo == caracter) {
		if(token.tipo==identificador && busca(fila,token) == 0){
			printf("(SEMANTICO)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nVariavel nao declarada!\n", linha, coluna, token.tipo); return tokenRetorno;
		}
		if(token.tipo == identificador){
			tokenRetorno=busca2(fila,token);
		}else if(token.tipo == caracter){
			tokenRetorno.tipo=_char;
			strcpy(tokenRetorno.valor,token.valor);
		}else {
			tokenRetorno=token;
		}
		token=scanner(arq);
		return tokenRetorno;
	}
	else {
		printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nErro token nao pertecente ao fator!\n", linha, coluna, token.tipo); return tokenRetorno;
	}
}

void parser(FILE *arq) {// PROGRAMA  int main (){ BLOCO }
	int status;
	printf("Programa:\n");
	token = scanner(arq);
	if (token.tipo != _int) {
		printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d):\n falta de 'int' no inicio do programa!\n", linha, coluna, token.tipo); return;
	}
	else {
		token = scanner(arq);
		if (token.tipo != _main) {
			printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \n falta de 'main' no inicio do programa!\n", linha, coluna, token.tipo); return;
		}
		else {
			token = scanner(arq);
			if (token.tipo != parenteses1) {
				printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \n falta de um '(' no inicio do programa!\n", linha, coluna, token.tipo); return;
			}
			else {
				token = scanner(arq);
				if (token.tipo != parenteses2) {
					printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de um ')' no inicio do programa !\n", linha, coluna, token.tipo); return;
				}
				else {
					token = scanner(arq);
					status = BLOCO(arq);
					if (status == 1) {
						if (token.tipo != fim_de_arquivo) {
							printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de fim de arquivo!\n", linha, coluna, token.tipo); return;
						}else{
							printf("Programa executado com SUCESSO!!!\n");return;
						}
					}
					else {
						return;
					}
				}
			}
		}
	}
}
