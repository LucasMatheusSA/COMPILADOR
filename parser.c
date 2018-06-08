//parser.c
#include "types.h"
#include "scanner.h"
#include "parser.h"
#include "fila.h"

int temporalrelacional=0,temp1=0;
int temporal=0,label=0;
TOKEN token;
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

printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \n !\n", linha, coluna, token.tipo); return 0;
*/

//*************************************************************************************************************************
void converte();

int variavel(){
	temporal++;
	return temporal;
}
//*************************************************************************************************************************


int TIPO( FILE *arq) {
	if (token.tipo == _int || token.tipo == _float || token.tipo == _char) {
		token=scanner(arq);return 1;
	}
	else {
		printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTipo invalido!\n", linha, coluna, token.tipo); return 0;
	}
}

int DECL_VAR(FILE *arq) {
	int resp;
	if (token.tipo == _int ) {
loop_DECL_VAR_int:
		token = scanner(arq);
		if (token.tipo == identificador) {
			resp=busca1(fila,token);
			if(resp==1){
				printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nVariavel ja declarada!\n", linha, coluna, token.tipo); return 0;
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
				printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nVariavel ja declarada!\n", linha, coluna, token.tipo); return 0;
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
				printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nVariavel ja declarada!\n", linha, coluna, token.tipo); return 0;
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

int BLOCO(FILE *arq) {
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

int COMANDO(FILE *arq) {
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
					printf("\tIF T0 == FLASE goto L%d\n",label1);label++;label++;temporal=0;
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

int COMANDO_BASICO(FILE *arq) {
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

int INTERACAO(FILE *arq) {
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
								printf("\tIF T0 != FALSE L%d\n",label1);temporal=0;
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

int ATRIBUICAO(FILE *arq) {
	TOKEN aux,status;
	int resp;
	if (token.tipo != identificador) {
		printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \n Identificador nao identificado!\n", linha, coluna, token.tipo); return 0;
	}
	else {
		resp=busca(fila,token);
		if(resp==0){
			printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \n Variavel nao declarada!\n", linha, coluna, token.tipo); return 0;
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
					if (token.tipo != pvirgula) {
						printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de um ';' no fim do comando!\n", linha, coluna, token.tipo); return 0;
					}else{
						if((aux.tipo ==  _float || aux.tipo == digito_float) && (status.tipo==_int || status.tipo==digito_int)){
							if(temporal != 0){
								printf("\tT%d = float T%d\n",temporal-1,temporal-1);
								printf("\t%s = T%d\n",aux.valor,temporal-1);
							}else{
								printf("\tT%d = float %s\n",temporal,status.valor);
								printf("\t%s = T%d\n",aux.valor,temporal);
							}
						}else {
							if(temporal != 0){
								printf("\t%s = T%d\n",aux.valor,temporal-1);
							}else{
								printf("\t%s = %s\n",aux.valor,status.valor);
							}
						}
						temporal=0;
						token=scanner(arq);
						return 1;
					}
				}else{
					printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nErro de compatibilidade na atribuicao!\n", linha, coluna, token.tipo); return 0;
				}
			}
			else {
				return 0;
			}
		}
	}
}

int EXPR_RELACIONAL(FILE *arq) {
	TOKEN status;
	char relacional[3];
	TOKEN tokenRetorno,aux;
	tokenRetorno.tipo=0;tokenRetorno.valor[0]='\0';
	status=EXPR_ARIT( arq);
	if(temporal==0){
		printf("\tT0 = %s\n",status.valor);
	}else{
		printf("\tT0 = T%d\n",temporal-1);
	}
	aux=status;temporal=0;temporalrelacional++;
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
			token = scanner(arq);
			status = EXPR_ARIT( arq);
			if(temporal==0){
				printf("\tT1 = %s\n",status.valor);
			}else{
				printf("\tT1 = T%d\n",temporal-1);
			}
			if (status.tipo == 0) {
				return 0;
			}
			else {
				if(((aux.tipo==_float || aux.tipo==digito_float || aux.tipo==digito_int || aux.tipo== _int) && (status.tipo==_float || status.tipo==digito_float || status.tipo==digito_int || status.tipo== _int)) || (aux.tipo == _char && status.tipo==_char)){
					if((aux.tipo == _float || aux.tipo == digito_float)&&(status.tipo == _int || status.tipo == digito_int)){
						printf("\tT1 = float T1\n");
					}else if((status.tipo == _int || status.tipo == digito_int)&&(aux.tipo == _float || aux.tipo == digito_float)){
						printf("\tT0 = float T0\n");
					}
					printf("\tT0 = T0 %s T1\n",relacional);
					temporalrelacional=0;
					return 1;
				}else 
					printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nErro de compatibilidade de tipo na expressao aritimetica!\n", linha, coluna, token.tipo); return 0;
			}
		}
		else {
			printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nFalta de um operador relacional valido!\n", linha, coluna, token.tipo); return 0;
		}
	}
}

TOKEN EXPR_ARIT(FILE *arq){
	int tipo,cont=0,temp2=0;
	TOKEN status,status2;
	TOKEN tokenRetorno;
	tokenRetorno.tipo=0;tokenRetorno.valor[0]='\0';
	if (token.tipo == identificador || token.tipo == digito_float || token.tipo == digito_int || token.tipo == caracter || token.tipo == parenteses1){	
		temp1=temporal;
		temporal=0;
		status=TERMO(arq);
		temp2=temporal;
		temporal=temp1;
		if(status.tipo==0){
			return tokenRetorno;
		}else{
			while(token.tipo ==  adicao || token.tipo == subtracao){
				cont++;
				tipo=token.tipo;
				token=scanner(arq);
				if (token.tipo == identificador || token.tipo == digito_float || token.tipo == digito_int || token.tipo == caracter || token.tipo == parenteses1){	
					temp1=temporal;
					temporal=0;
					status2=TERMO(arq);
					temp2=temporal;
					temporal=temp1;
					if(status2.tipo==0)
						return tokenRetorno;
					else{
						if((status.tipo == _char)&&(status2.tipo != _char)){
							printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTipo nao compativel com char!\n", linha, coluna, token.tipo); return tokenRetorno;
						}else if((status.tipo == _char) && (status2.tipo == _char)){
							if(temporal==0){				
								if(temporalrelacional!=0){temporal++;}
								if(tipo ==  adicao){
									printf("\tT%d = %s + %s\n",temporal,status.valor,status2.valor);temporal++;
								}else{
									printf("\tT%d = %s - %s\n",temporal,status.valor,status2.valor);temporal++;
								}
							}else{
								if(temp2==0){
									if(tipo == adicao){
										printf("\tT%d = T%d + %s\n",temporal,temporal-1,status2.valor);temporal++;
									}else{
										printf("\tT%d = T%d - %s\n",temporal,temporal-1,status2.valor);temporal++;
									}
								}else{
									if(tipo == adicao){
										printf("\tT%d = T%d + T%d\n",temporal,temporal-1,temp2-1);temporal++;
									}else{
										printf("\tT%d = T%d - T%d\n",temporal,temporal-1,temp2-1);temporal++;
									}
								}
							}
						}
						if((status.tipo == _float ||  status.tipo == digito_float)&&(status2.tipo != _int && status2.tipo != _float && status2.tipo !=  digito_int && status2.tipo != digito_float)){
							printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTipo nao compativel com int ou float!\n", linha, coluna, token.tipo); return tokenRetorno;
						}else if((status.tipo == _float ||  status.tipo == digito_float)&&(status2.tipo == _int || status2.tipo == _float || status2.tipo ==  digito_int || status2.tipo == digito_float)){
							if(temporal==0){
								if(temporalrelacional!=0){temporal++;}
								if(status2.tipo==_int || status2.tipo==digito_int){
									printf("\tT%d = float %s\n",temporal,status2.valor);temporal++;
									if(tipo ==  adicao){
										printf("\tT%d = %s + T%d\n",temporal,status.valor,temporal-1);temporal++;
									}else{
										printf("\tT%d = %s - T%d\n",temporal,status.valor,temporal-1);temporal++;
									}
								}else{
									if(tipo ==  adicao){
										printf("\tT%d = %s + %s\n",temporal,status.valor,status2.valor);temporal++;
									}else{
										printf("\tT%d = %s - %s\n",temporal,status.valor,status2.valor);temporal++;
									}
								}

							}else{
								if(temp2==0){
									if(status2.tipo==_int || status2.tipo==digito_int){
										printf("\tT%d = float %s\n",temporal,status2.valor);temporal++;
										if(tipo == adicao){
											printf("\tT%d = T%d + T%d\n",temporal,temporal-2,temporal-1);temporal++;
										}else{
											printf("\tT%d = T%d - T%d\n",temporal,temporal-2,temporal-1);temporal++;
										}
									}else{
										if(tipo == adicao){
											printf("\tT%d = T%d + %s\n",temporal,temporal-1,status2.valor);temporal++;
										}else{
											printf("\tT%d = T%d - %s\n",temporal,temporal-1,status2.valor);temporal++;
										}
									}
								}else {
									if(status2.tipo==_int || status2.tipo==digito_int){
										printf("\tT%d = float T%d\n",temporal,temp2-1);temporal++;
										if(tipo == adicao){
											printf("\tT%d = T%d + T%d\n",temporal,temporal-2,temporal-1);temporal++;
										}else{
											printf("\tT%d = T%d - T%d\n",temporal,temporal-2,temporal-1);temporal++;
										}
									}else{
										if(tipo == adicao){
											printf("\tT%d = T%d + T%d\n",temporal,temporal-1,temp2-1);temporal++;
										}else{
											printf("\tT%d = T%d - T%d\n",temporal,temporal-1,temp2-1);temporal++;
										}
									}
								}
							}
							status2.tipo=_float;
						}
						if((status.tipo == _int || status.tipo == digito_int)&&(status2.tipo != _int  && status2.tipo !=  digito_int && status2.tipo != _float && status2.tipo != digito_float)){
							printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTipo nao compativel com int!\n", linha, coluna, token.tipo); return tokenRetorno;
						}else if((status.tipo == _int || status.tipo == digito_int)&&(status2.tipo == _float || status2.tipo ==  digito_float)){
							if(temporal==0){
								if(temporalrelacional!=0){temporal++;}
								if(status.tipo==_int || status.tipo==digito_int){
									printf("\tT%d = float %s\n",temporal,status.valor);temporal++;
									if(tipo ==  adicao){
										printf("\tT%d = T%d + %s\n",temporal,temporal-1,status2.valor);temporal++;
									}else{
										printf("\tT%d = T%d - %s\n",temporal,temporal-1,status2.valor);temporal++;
									}
								}else{
									if(tipo ==  adicao){
										printf("\tT%d = %s + %s\n",temporal,status.valor,status2.valor);temporal++;
									}else{
										printf("\tT%d = %s - %s\n",temporal,status.valor,status2.valor);temporal++;
									}
								}
							}else{
								if(temp2==0){
									if(tipo == adicao){
										printf("\tT%d = T%d + %s\n",temporal,temporal-1,status2.valor);temporal++;
									}else{
										printf("\tT%d = T%d - %s\n",temporal,temporal-1,status2.valor);temporal++;
									}
								}else{
									if(tipo == adicao){
										printf("\tT%d = T%d + T%d\n",temporal,temporal-1,temp2-1);temporal++;
									}else{
										printf("\tT%d = T%d - T%d\n",temporal,temporal-1,temp2-1);temporal++;
									}
								}
							}
							status.tipo=_float;
						}else if((status.tipo == _int || status.tipo == digito_int)&&(status2.tipo == _int  || status2.tipo ==  digito_int)){
							if(temporal==0){
								if(temporalrelacional!=0)temporal++;
								if(tipo ==  adicao){
									printf("\tT%d = %s + %s\n",temporal,status.valor,status2.valor);temporal++;
								}else{
									printf("\tT%d = %s - %s\n",temporal,status.valor,status2.valor);temporal++;
								}
							}else{
								if(temp2==0){
									if(tipo ==  adicao){
										printf("\tT%d = T%d + %s\n",temporal,temporal-1,status2.valor);temporal++;
									}else{
										printf("\tT%d = T%d - %s\n",temporal,temporal-1,status2.valor);temporal++;
									}
								}else{
									if(tipo ==  adicao){
										printf("\tT%d = T%d + T%d\n",temporal,temporal-1,temp2-1);temporal++;
									}else{
										printf("\tT%d = T%d - T%d\n",temporal,temporal-1,temp2-1);temporal++;
									}
								}
							}
						}
						status=status2;
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


TOKEN TERMO(FILE *arq){
	int tipo,cont=0;
	TOKEN status,status2;
	TOKEN tokenRetorno;
	tokenRetorno.tipo=0;tokenRetorno.valor[0]='\0';
	if (token.tipo == identificador || token.tipo == digito_float || token.tipo == digito_int || token.tipo == caracter || token.tipo == parenteses1){	
		status=FATOR(arq);
		if(status.tipo==0){
			return tokenRetorno;
		}else{
			while(token.tipo ==  multiplicacao || token.tipo == divisao){
				cont++;
				tipo=token.tipo;
				token=scanner(arq);
				if (token.tipo == identificador || token.tipo == digito_float || token.tipo == digito_int || token.tipo == caracter || token.tipo == parenteses1){	
					status2=FATOR(arq);
					if(status2.tipo==0)
						return tokenRetorno;
					else{
						if((status.tipo == _char)&&(status2.tipo != _char)){
							printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTipo nao compativel com char!\n", linha, coluna, token.tipo); return tokenRetorno;
						}else if((status.tipo == _char) && (status2.tipo == _char)){
							if(temporal==0){				
								if(temporalrelacional!=0){temporal++;}
								if(temp1!=0){temporal=temp1;}
								if(tipo ==  multiplicacao){
									printf("\tT%d = %s * %s\n",temporal,status.valor,status2.valor);temporal++;
								}else{
									printf("\tT%d = %s / %s\n",temporal,status.valor,status2.valor);temporal++;
								}
							}else{
								if(tipo == multiplicacao){
									printf("\tT%d = T%d * %s\n",temporal,temporal-1,status2.valor);temporal++;
								}else{
									printf("\tT%d = T%d / %s\n",temporal,temporal-1,status2.valor);temporal++;
								}
							}
						}
						if((status.tipo == _float ||  status.tipo == digito_float)&&(status2.tipo != _int && status2.tipo != _float && status2.tipo !=  digito_int && status2.tipo != digito_float)){
							printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTipo nao compativel com int ou float!\n", linha, coluna, token.tipo); return tokenRetorno;
						}else if((status.tipo == _float ||  status.tipo == digito_float)&&(status2.tipo == _int || status2.tipo == _float || status2.tipo ==  digito_int || status2.tipo == digito_float)){
							if(temporal==0){
								if(temporalrelacional!=0){temporal++;}
								if(temp1!=0){temporal=temp1;}
								if(status2.tipo==_int || status2.tipo==digito_int){
									printf("\tT%d = float %s\n",temporal,status2.valor);temporal++;
									if(tipo ==  multiplicacao){
										printf("\tT%d = %s * T%d\n",temporal,status.valor,temporal-1);temporal++;
									}else{
										printf("\tT%d = %s / T%d\n",temporal,status.valor,temporal-1);temporal++;
									}
								}else{
									if(tipo ==  multiplicacao){
										printf("\tT%d = %s * %s\n",temporal,status.valor,status2.valor);temporal++;
									}else{
										printf("\tT%d = %s / %s\n",temporal,status.valor,status2.valor);temporal++;
									}
								}

							}else{
								if(status2.tipo==_int || status2.tipo==digito_int){
									printf("\tT%d = float %s\n",temporal,status2.valor);temporal++;
									if(tipo == multiplicacao){
										printf("\tT%d = T%d * T%d\n",temporal,temporal-2,temporal-1);temporal++;
									}else{
										printf("\tT%d = T%d / T%d\n",temporal,temporal-2,temporal-1);temporal++;
									}
								}else{
									if(tipo == multiplicacao){
										printf("\tT%d = T%d * %s\n",temporal,temporal-1,status2.valor);temporal++;
									}else{
										printf("\tT%d = T%d / %s\n",temporal,temporal-1,status2.valor);temporal++;
									}
								}

							}
						}
						if((status.tipo == _int || status.tipo == digito_int)&&(status2.tipo != _int  && status2.tipo !=  digito_int && status2.tipo != _float && status2.tipo != digito_float)){
							printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nTipo nao compativel com int!\n", linha, coluna, token.tipo); return tokenRetorno;
						}else if((status.tipo == _int || status.tipo == digito_int)&&(status2.tipo == _float || status2.tipo ==  digito_float)){
							if(temporal==0){
								if(temporalrelacional!=0){temporal++;}
								if(temp1!=0){temporal=temp1;}
								if(status.tipo==_int || status.tipo==digito_int){
									printf("\tT%d = float %s\n",temporal,status.valor);temporal++;
									if(tipo ==  multiplicacao){
										printf("\tT%d = T%d * %s\n",temporal,temporal-1,status2.valor);temporal++;
									}else{
										printf("\tT%d = T%d / %s\n",temporal,temporal-1,status2.valor);temporal++;
									}
								}else{
									if(tipo ==  multiplicacao){
										printf("\tT%d = %s * %s\n",temporal,status.valor,status2.valor);temporal++;
									}else{
										printf("\tT%d = %s / %s\n",temporal,status.valor,status2.valor);temporal++;
									}
								}
							}else{
								if(tipo == multiplicacao){
									printf("\tT%d = T%d * %s\n",temporal,temporal-1,status2.valor);temporal++;
								}else{
									printf("\tT%d = T%d / %s\n",temporal,temporal-1,status2.valor);temporal++;
								}
							}
							status.tipo=_float;
						}else if((status.tipo == _int || status.tipo == digito_int)&&(status2.tipo == _int  || status2.tipo ==  digito_int)){
							if(temporal==0){
								if(temporalrelacional!=0)temporal++;
								if(temp1!=0){temporal=temp1;}
								printf("\tT%d = float %s\n",temporal,status.valor);temporal++;
								printf("\tT%d = float %s\n",temporal,status2.valor);temporal++;
								if(tipo ==  multiplicacao){
									printf("\tT%d = T%d * T%d\n",temporal,temporal-2,temporal-1);temporal++;
								}else{
									printf("\tT%d = T%d / T%d\n",temporal,temporal-2,temporal-1);temporal++;
								}
							}else{
								printf("\tT%d = float T%d\n",temporal,temporal);temporal++;
								printf("\tT%d = float %s\n",temporal,status2.valor);temporal++;
								if(tipo ==  multiplicacao){
									printf("\tT%d = T%d * T%d\n",temporal,temporal-2,temporal-1);temporal++;
								}else{
									printf("\tT%d = T%d / T%d\n",temporal,temporal-2,temporal-1);temporal++;
								}
							}
							status2.tipo=_float;
							status.tipo=_float;
						}
						status=status2;
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

TOKEN FATOR(FILE *arq) {
	TOKEN status;
	TOKEN tokenRetorno;
	char c[4];
	tokenRetorno.tipo=0;tokenRetorno.valor[0]='\0';
	if (token.tipo == parenteses1) {
		token=scanner(arq);
		status=EXPR_ARIT(arq);
		if (status.tipo = 0) {
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
			printf("(PARSER)ERRO na linha %d, coluna %d, ultimo token lido (%d): \nVariavel nao declarada!\n", linha, coluna, token.tipo); return tokenRetorno;
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

void parser(FILE *arq) {
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
