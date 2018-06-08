//fila
#include "types.h"

void push(TTOKEN **fila,TOKEN token){ // coloco no inicio da fila 
	TTOKEN *aux;
	if((*fila)==NULL){
		(*fila)=(TTOKEN*)malloc(sizeof(TTOKEN));
		(*fila)->tipo=token.tipo;
		strcpy((*fila)->valor,token.valor);
		(*fila)->prox=NULL;
	}else{
		aux=(TTOKEN*)malloc(sizeof(TTOKEN));
		aux->prox=(*fila);
		aux->tipo=token.tipo;
		strcpy(aux->valor,token.valor);
		(*fila)=aux;
	}
}

void push_null(TTOKEN *fila){ //coloca um marco NULL na fila para marcar o escopo 
	TOKEN aux;
	aux.tipo=0;aux.valor[0]='\0';
	push(&fila,aux);
}

void removeT(TTOKEN **fila){ // remove da fila ate o ultimo NULL
	TTOKEN *aux;
 	while((*fila)!=NULL){
		aux=(*fila);
		(*fila)=(*fila)->prox;
		if(aux->tipo==0){
			free(aux);
			return;
		}
		free(aux);
	}
}

int busca3(TTOKEN *fila,TOKEN token){
	TTOKEN *aux=fila;
	while(aux!=NULL){
		if(strcmp(aux->valor,token.valor)==0){
			return 1;
		}
		aux=aux->prox;
	}
	return 0;
}

int busca(TTOKEN *fila,TOKEN token){ //retorna 0 se a vareavel nao foi declarada  e 1 se ja foi
	TTOKEN *aux=fila;
	int x=0,y=0;
	while(aux!=NULL){
		if(aux->tipo==0){
			y=y+1;
		}
		if(strcmp(aux->valor,token.valor)==0 && y==1){
			x=2;return 2;
		}
		if(strcmp(aux->valor,token.valor)==0 && y==0){
			x=1;return 1;
		}
		aux=aux->prox;
	}
	x=busca3(fila, token);
	return x;
}

int busca1(TTOKEN *fila,TOKEN token){
	TTOKEN *aux=fila;
	int x=0,y=0;
	while(aux!=NULL){
		if(aux->tipo==0){
			y=y+1;
		}
		if(strcmp(aux->valor,token.valor)==0 && y==1){
			x=2;return 2;
		}
		if(strcmp(aux->valor,token.valor)==0 && y==0){
			x=1;return 1;
		}
		aux=aux->prox;
	}
	return x;
}

TOKEN busca2(TTOKEN *fila,TOKEN token){
	TTOKEN *aux=fila;
	TOKEN tokenR;
	tokenR.tipo=0;tokenR.valor[0]='\0';
	while(aux!=NULL){
		if(strcmp(aux->valor,token.valor)==0){
			tokenR.tipo=aux->tipo;
			strcpy(tokenR.valor,aux->valor);
			return tokenR;
		}
		aux=aux->prox;
	}
	return token;
}
