#ifndef MISTRUCT
#define MISTRUCT

void* mem;
extern int tamano_actual;
extern struct Lista* lista_libres ;
extern struct Lista* lista_ocupados;

typedef struct Nodemem {
	int id;
	int tam;
	void* iniciomem_block;
} Nodemem;

typedef struct Nodo {
	struct Nodo* prev;
	struct Nodo* sig;
	struct Nodemem dato;
} Nodo;

typedef void (*t_anadir_nodo)(struct Lista* lista, int size, int align);
typedef void (*t_insertar_nodo)(struct Lista* lista, struct Nodo* nodo);
typedef int (*t_buscar_nodo)(struct Lista* lista, int id);
typedef struct Nodo* (*t_eliminar_nodo)(struct Lista* lista, int posicion);
typedef struct Nodo* (*t_get_primer_nodo)(struct Lista*);
typedef struct Nodo* (*t_get_ultimo_nodo)(struct Lista*);
typedef struct Nodo* (*t_get_nodo)(struct Lista*, int posicion);

typedef struct Lista {
	int size;
	struct Nodo* prim;
	struct Nodo* ult;
	t_anadir_nodo anadir_nodo;
	t_eliminar_nodo eliminar_nodo;
	t_insertar_nodo insertar_nodo;
	t_buscar_nodo buscar_nodo;
	t_get_primer_nodo get_primer_nodo;
	t_get_ultimo_nodo get_ultimo_nodo;
	t_get_nodo get_nodo;
} Lista;

void anadir_nodo(Lista* lista, int size, int align);
Nodo* eliminar_nodo(Lista* lista, int posicion);
void insertar_nodo(Lista* lista, Nodo* nodo);
int buscar_nodo(Lista* lista, int id);
Nodo* get_primer_nodo(Lista* lista);
Nodo* get_ultimo_nodo(Lista* lista);
Nodo* get_nodo(Lista* lista, int posicion);

void init(void* _mem, int tam);

void init_lista(Lista* lista);
void* m_malloc(int tam, int align);
void* m_malloc_aux(int tam);
void* m_aligned_malloc_aux(int tam, int align);
void* m_realloc(void* memo, int tam, int align);
void m_free(void* memo);


#endif
