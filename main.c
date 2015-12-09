#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "m_malloc.h"

void printTamanoActual();
void pintar_lista(Lista* lista);
void copiar_matriz(Lista* lista);

int main(){
	printf("\n\n");
	int tam = 1024*1024*100; //104857600
	void* memaux;
	tamano_actual = 104857600;
	memaux = (void*)malloc(tam);
	memset(memaux, 0, tam);
	init(memaux,tam);

	char* aux;

	Nodo* mi_nodo = m_malloc(sizeof(int)*10000, 16);
	int* matriz = mi_nodo->dato.iniciomem_block;

	pintar_lista(lista_ocupados);

	//mi_nodo = m_realloc(mi_nodo, sizeof(int)*10000, 16);

	copiar_matriz(lista_ocupados);

	m_free(lista_ocupados->get_nodo(lista_ocupados, 0));
	free(memaux);
}

void copiar_matriz(Lista* lista){
	int c, n = 0;
	FILE* fp;
	fp = fopen("matriz", "r");

	Nodo* nodo = lista->get_nodo(lista, 0);
	int* memoria = (int*)nodo->dato.iniciomem_block;

	if (fp == NULL) perror ("Error opening file");
   	else {
    	do {
    		fscanf (fp, "%d", &c);
	    	if(c != '\n') {
	    		memoria[n] = c;
	    		n++; 	
    		}
    	} while (!feof (fp));
    	fclose (fp);
  	}
   	return;
}