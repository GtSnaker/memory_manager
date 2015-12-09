#include <stdio.h>
#include <stdlib.h>
#include "m_malloc.h"
#include <string.h>
#include <tgmath.h>

int actual_id = 0;
void* mem;
Lista* lista_libres = NULL;
Lista* lista_ocupados = NULL;
int tamano_actual = 0;

void* m_malloc_aux(int tam) { 
	if(lista_libres->size > 0) {
		Nodo* nodo = lista_libres->prim;
		for (int i = 0; i < lista_libres->size; i++){
			if(nodo->dato.tam == tam){
				lista_libres->eliminar_nodo(lista_libres,
					lista_libres->buscar_nodo(lista_libres,
						nodo->dato.id));
				return nodo;
			}
			nodo = nodo->sig;
		}
	} 
	if(tamano_actual > tam){
		char* aux = (char*)mem;
		void* res = mem;
		aux += tam;
		mem = aux;
		tamano_actual -= tam;
		memset(res, 0, tam);
		return res;
	}
	return NULL;
}

void* m_aligned_malloc_aux(int tam, int align){

	void* dir = m_malloc_aux(tam+align);
	int num_bits = log2(align);
	
	if(sizeof(void*) == 8){
		unsigned long mask = 0xFFFFFFFFFFFFFFFF << num_bits;
		unsigned long direc = (unsigned long)dir;

		dir = (void*)(direc&mask);
	}
	else if(sizeof(void*) == 4){
		int mask = 0xFFFFFFFF << num_bits;
		int direc = (int)dir;

		dir = (void*)(direc&mask);
	}
	return dir;
}

void* m_malloc(int tam, int align){
	lista_ocupados->anadir_nodo(lista_ocupados, tam, align);
	Nodo* nodo = lista_ocupados->get_nodo(lista_ocupados, lista_ocupados->size-1);
	return nodo;
}

void m_free(void* memo){
	Nodo* nodo = (Nodo*)memo;
	lista_libres->insertar_nodo(lista_libres,
		lista_ocupados->eliminar_nodo(lista_ocupados,
			lista_ocupados->buscar_nodo(lista_ocupados, 
				nodo->dato.id)));
}

void* m_realloc(void* memo, int tam, int align){
	//solo hecho el nodo, falta memoria
	void* new_memo;
	if(align == 0){
		new_memo = m_malloc_aux(tam);
	} else {
		new_memo = m_aligned_malloc_aux(tam, align);
	}

	Nodo* old_nodo = (Nodo*)memo;
	Nodo* new_nodo = (Nodo*)memo;

	char* aux_old_nodo = (char*)old_nodo->dato.iniciomem_block;
	char* aux_new_nodo = (char*)new_nodo->dato.iniciomem_block;

	int old_tam = old_nodo->dato.tam;
	int new_tam = tam;

	new_nodo->prev = old_nodo->prev;
	new_nodo->sig = new_nodo->sig;

	if(old_nodo->prev == NULL && old_nodo->sig == NULL){
		lista_ocupados->prim = new_nodo;
	} else if(old_nodo->prev == NULL){
		lista_ocupados->prim = new_nodo;
		old_nodo->sig->prev = new_nodo;
	} else if(old_nodo->sig == NULL){
		old_nodo->prev->sig = new_nodo;
	} else {
		old_nodo->prev->sig = new_nodo;
		old_nodo->sig->prev = new_nodo;
	}

	new_nodo->dato.id = old_nodo->dato.id;
	new_nodo->dato.tam = tam;

	for (int i = 0; i < old_tam && i < new_tam; i++){
		aux_new_nodo[i] = aux_old_nodo[i];
	}

	m_free(memo);
	lista_ocupados->size++;
	return new_memo;
}

void init_nodo(Nodo* nodo, Nodo* prev){
	nodo->prev = prev;
	nodo->sig = NULL;
}

void anadir_nodo(struct Lista* lista, int size, int aligned){
	
	if(lista->prim == NULL){
		lista->prim = (Nodo*)m_malloc_aux(sizeof(Nodo));
		lista->prim->prev=NULL;
		init_nodo(lista->prim, NULL);
		if(aligned == 0) {
			lista->prim->dato.iniciomem_block = (void*)m_malloc_aux(size);
		} else {
			lista->prim->dato.iniciomem_block = (void*)m_aligned_malloc_aux(size, aligned);
		}
		lista->prim->dato.id = actual_id;
		lista->prim->dato.tam = size;
	} else {
		Nodo* aux = lista->prim;
		while(aux->sig != NULL){
			aux = aux->sig;
		}
		aux->sig = (Nodo*)m_malloc_aux(sizeof(Nodo));
		init_nodo(aux->sig, aux);
		if(aligned == 0) {
			aux->sig->dato.iniciomem_block = (void*)m_malloc_aux(size);
		} else {
			aux->sig->dato.iniciomem_block = (void*)m_aligned_malloc_aux(size, aligned);
		}
		aux->sig->dato.id = actual_id;
		aux->sig->dato.tam = size;
	}
	lista->size++;
	actual_id++;
}

Nodo* eliminar_nodo(struct Lista* lista, int posicion){
	Nodo* nodo_a_eliminar = NULL;
	nodo_a_eliminar = lista->get_nodo(lista, posicion);
	if (nodo_a_eliminar->prev == NULL && nodo_a_eliminar->sig == NULL) {
		//absolutamente nada
	} else if(nodo_a_eliminar->prev == NULL){
		lista_ocupados->prim = nodo_a_eliminar->sig;
	} else if(nodo_a_eliminar->sig == NULL) {
		nodo_a_eliminar->prev->sig = nodo_a_eliminar->sig;
	} else {
		nodo_a_eliminar->prev->sig = nodo_a_eliminar->sig;
		nodo_a_eliminar->sig->prev = nodo_a_eliminar->prev;
	}
	nodo_a_eliminar->prev = NULL;
	nodo_a_eliminar->sig = NULL;

	lista->size--;

	return nodo_a_eliminar;
}

void insertar_nodo(struct Lista* lista, struct Nodo* nodo){
	if(lista->prim == NULL){
		lista->prim = nodo;
		init_nodo(nodo, NULL);
	} else {
		Nodo* aux = lista->prim;
		while(aux->sig != NULL){
			aux = aux->sig;
		}
		init_nodo(nodo, aux);
	}
	tamano_actual += nodo->dato.tam;
	lista->size++;
}

int buscar_nodo(struct Lista* lista, int id){
	Nodo* aux = lista->prim;
	int i = 0;
	while(aux != NULL){
		if(id == aux->dato.id){
			return i;
		} else {
			aux = aux->sig;
		}
		i++;
	}
	return -1;
}

Nodo* get_nodo(struct Lista* lista, int posicion){
	Nodo* nodo = lista->prim;
	for (int i = 0; i < posicion; i++){
		nodo = nodo->sig;
	}
	return nodo;
}

Nodo* get_primer_nodo(struct Lista* lista){
	return lista->prim;
}

Nodo* get_ultimo_nodo(struct Lista* lista){
	return lista->ult;
}

void pintar_lista(Lista* lista){
	int size = lista->size;
	Nodo* nodo = lista->prim;
	for (int i = 0; i < size; i++){
		printf("id: %d; tam: %d\n", nodo->dato.id, nodo->dato.tam);
		nodo = nodo->sig;
	}
}

void printTamanoActual(){
	printf("tamano actual: %d\n", tamano_actual);
}

void init_lista(Lista* lista){
	lista->prim = NULL;
	lista->ult = NULL;
	lista->size = 0;
	lista->anadir_nodo = (t_anadir_nodo)anadir_nodo;
	lista->eliminar_nodo = (t_eliminar_nodo)eliminar_nodo;
	lista->insertar_nodo = (t_insertar_nodo)insertar_nodo;
	lista->buscar_nodo = (t_buscar_nodo)buscar_nodo;
	lista->get_primer_nodo = (t_get_primer_nodo)get_primer_nodo;
	lista->get_ultimo_nodo = (t_get_ultimo_nodo)get_ultimo_nodo;
	lista->get_nodo = (t_get_nodo)get_nodo;
}

void init(void* _mem, int tam){
	mem = _mem;
	char* aux = (char*)mem;

	//espacio de la lista libres

	aux += sizeof(Lista);
	Lista* new_lista = (Lista*)mem;
	lista_libres = new_lista;
	mem =(void*) (&aux[0]);
	init_lista(lista_libres);
	//espacio de la lista ocupados
	
	aux += sizeof(Lista);
	new_lista = (Lista*)mem;
	lista_ocupados = new_lista;
	mem = aux;
	init_lista(lista_ocupados);

	//resto el tamano
	tamano_actual -= sizeof(Lista) + sizeof(Lista);
}

