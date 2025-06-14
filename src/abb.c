#include "abb.h"
#include "abb_estructura_privada.h"
#include <stdlib.h>
#include <stdio.h>

void *abb_raiz(const abb_t *abb)
{
	if (!abb)
		return NULL;
	return (void *)abb->raiz;
}

int (*abb_comparador(const abb_t *abb))(const void *, const void *)
{
	if (!abb)
		return NULL;

	return abb->comparador;
}

abb_t *abb_crear(int (*cmp)(const void *, const void *))
{
	if (!cmp)
		return NULL;

	abb_t *abb = malloc(sizeof(abb_t));
	if (!abb)
		return NULL;

	abb->comparador = cmp;
	abb->raiz = NULL;
	abb->nodos = 0;

	return abb;
}

bool insertar_nodo(nodo_t **nodo, nodo_t *nuevo,
		   int (*comparador)(const void *, const void *))
{
	if (!*nodo) {
		*nodo = nuevo;
		return true;
	}

	if (!nuevo->elemento || !(*nodo)->elemento) {
		if (!nuevo->elemento && !(*nodo)->elemento)
			return insertar_nodo(
				&(*nodo)->izq, nuevo,
				comparador); 
		else if (!nuevo->elemento)
			return insertar_nodo(
				&(*nodo)->izq, nuevo,
				comparador); 
		else
			return insertar_nodo(&(*nodo)->der, nuevo, comparador);
	}

	int cmp_result = comparador(nuevo->elemento, (*nodo)->elemento);
	if (cmp_result <= 0) {
		return insertar_nodo(&(*nodo)->izq, nuevo, comparador);
	} else {
		return insertar_nodo(&(*nodo)->der, nuevo, comparador);
	}
}

bool abb_insertar(abb_t *abb, const void *elemento)
{
	if (!abb)
		return false; 

	nodo_t *nuevo = malloc(sizeof(nodo_t));
	if (!nuevo)
		return false;

	nuevo->elemento = (void *)
		elemento; 
	nuevo->izq = NULL;
	nuevo->der = NULL;

	bool resultado = insertar_nodo(&abb->raiz, nuevo, abb->comparador);
	if (resultado)
		abb->nodos++;

	return resultado;
}

bool abb_existe(const abb_t *abb, const void *elemento)
{
	if (!abb || !elemento)
		return false;
	nodo_t *actual = abb->raiz;

	while (actual) {
		if (!actual || !actual->elemento)
			return false;

		int cmp_result = abb->comparador(elemento, actual->elemento);
		if (cmp_result == 0) {
			return true;
		} else {
			actual = (cmp_result < 0) ? actual->izq : actual->der;
		}
	}

	return false;
}

void *abb_buscar(const abb_t *abb, const void *elemento)
{
	if (!abb || !elemento)
		return NULL;

	nodo_t *actual = abb->raiz;

	while (actual) {
		int cmp_result = abb->comparador(elemento, actual->elemento);
		if (cmp_result == 0) {
			return actual->elemento;
		}

		actual = (cmp_result < 0) ? actual->izq : actual->der;
	}

	return NULL;
}

nodo_t *eliminar_nodo_con_hijos_unicos(nodo_t *nodo)
{
	nodo_t *hijo = nodo->izq ? nodo->izq : nodo->der;
	free(nodo);
	return hijo;
}

nodo_t *encontrar_y_reemplazar_con_predecesor(nodo_t *nodo,
					      nodo_t **referencia_padre)
{
	nodo_t *predecesor = nodo->izq;
	nodo_t *padre_predecesor = nodo;

	while (predecesor->der) {
		padre_predecesor = predecesor;
		predecesor = predecesor->der;
	}

	nodo->elemento = predecesor->elemento;

	if (padre_predecesor->der == predecesor) {
		padre_predecesor->der = predecesor->izq;
	} else {
		padre_predecesor->izq = predecesor->izq;
	}

	predecesor->izq = NULL;
	predecesor->der = NULL;
	free(predecesor);

	return nodo;
}

nodo_t *eliminar_nodo_postorden(nodo_t *nodo, const void *elemento,
				int (*comparador)(const void *, const void *),
				void **elemento_eliminado, bool *encontrado)
{
	if (!nodo)
		return NULL;

	nodo->izq = eliminar_nodo_postorden(nodo->izq, elemento, comparador,
					    elemento_eliminado, encontrado);
	nodo->der = eliminar_nodo_postorden(nodo->der, elemento, comparador,
					    elemento_eliminado, encontrado);

	if (!*encontrado && comparador(elemento, nodo->elemento) == 0) {
		*encontrado = true;
		*elemento_eliminado = nodo->elemento;

		nodo_t *nuevo_nodo = NULL;
		if (!nodo->izq || !nodo->der) {
			nuevo_nodo = eliminar_nodo_con_hijos_unicos(nodo);
		} else {
			nuevo_nodo = encontrar_y_reemplazar_con_predecesor(
				nodo, &(nodo->izq));
		}

		nodo = NULL;

		free(nodo);

		return nuevo_nodo;
	}

	return nodo;
}

void *abb_sacar_todo(abb_t *abb, const void *elemento,
		     void (*destructor)(void *))
{
	if (!abb) 
		return NULL;

	void *elemento_eliminado = NULL;
	bool encontrado = false;

	nodo_t *nueva_raiz =
		eliminar_nodo_postorden(abb->raiz, elemento, abb->comparador,
					&elemento_eliminado, &encontrado);

	if (encontrado) {
		
		if (abb->nodos > 0) {
			abb->nodos--;
		}

		
		if (abb->nodos == 0 || nueva_raiz == NULL) {
			abb->raiz = NULL;
		} else {
			abb->raiz = nueva_raiz;
		}

		if (destructor && elemento_eliminado) {
			destructor(elemento_eliminado);
		}
	}

	return elemento_eliminado;
}

void *abb_sacar(abb_t *abb, const void *elemento)
{
	if (!abb)
		return NULL;

	void *eliminado = abb_sacar_todo(abb, elemento, NULL);

	if (abb->nodos == 0) {
		abb->raiz = NULL;
	}

	       abb->nodos, (void *)abb->raiz);

	return eliminado;
}

size_t abb_tamanio(const abb_t *abb)
{
	if (!abb)
		return 0;

	return abb->nodos;
}

bool abb_vacio(const abb_t *abb)
{
	if (!abb || abb->raiz == NULL)
		return true;

	return false;
}

size_t recorrer_preorden(nodo_t *nodo, enum abb_recorrido modo,
			 bool (*f)(void *, void *), void *ctx, size_t *contador,
			 bool *parar)
{
	if (!nodo || *parar)
		return *contador;

	if (!*parar && f) {
		(*contador)++;
		if (!f(nodo->elemento, ctx)) {
			*parar = true;
			return *contador;
		}
	}

	*contador = recorrer_preorden(nodo->izq, modo, f, ctx, contador, parar);
	*contador = recorrer_preorden(nodo->der, modo, f, ctx, contador, parar);

	return *contador;
}

size_t recorrer_inorden(nodo_t *nodo, enum abb_recorrido modo,
			bool (*f)(void *, void *), void *ctx, size_t *contador,
			bool *parar)
{
	if (!nodo || *parar)
		return *contador;

	*contador = recorrer_inorden(nodo->izq, modo, f, ctx, contador, parar);

	if (!*parar && f) {
		(*contador)++;
		if (!f(nodo->elemento, ctx)) {
			*parar = true;
			return *contador;
		}
	}
	*contador = recorrer_inorden(nodo->der, modo, f, ctx, contador, parar);
	return *contador;
}

size_t recorrer_postorden(nodo_t *nodo, enum abb_recorrido modo,
			  bool (*f)(void *, void *), void *ctx,
			  size_t *contador, bool *parar)
{
	if (!nodo || *parar)
		return *contador;

	*contador =
		recorrer_postorden(nodo->izq, modo, f, ctx, contador, parar);
	*contador =
		recorrer_postorden(nodo->der, modo, f, ctx, contador, parar);

	if (!*parar && f) {
		(*contador)++;
		if (!f(nodo->elemento, ctx)) {
			*parar = true;
			return *contador;
		}
	}

	return *contador;
}

size_t abb_recorrer(const abb_t *abb, enum abb_recorrido modo,
		    bool (*f)(void *, void *), void *ctx)
{
	if (!abb || f == NULL || abb->comparador == NULL)
		return 0;

	size_t contador = 0;
	bool parar = false;

	if (modo == ABB_PREORDEN) {
		recorrer_preorden(abb->raiz, modo, f, ctx, &contador, &parar);
	} else if (modo == ABB_INORDEN) {
		recorrer_inorden(abb->raiz, modo, f, ctx, &contador, &parar);
	} else if (modo == ABB_POSTORDEN) {
		recorrer_postorden(abb->raiz, modo, f, ctx, &contador, &parar);
	}

	return contador;
}

static void vectorizar_nodos(nodo_t *nodo, enum abb_recorrido modo,
			     void **vector, size_t capacidad, size_t *contador)
{
	if (!nodo || *contador >= capacidad)
		return;

	if (modo == ABB_PREORDEN) {
		vector[(*contador)++] = nodo->elemento;
		vectorizar_nodos(nodo->izq, modo, vector, capacidad, contador);
		vectorizar_nodos(nodo->der, modo, vector, capacidad, contador);
	} else if (modo == ABB_INORDEN) {
		vectorizar_nodos(nodo->izq, modo, vector, capacidad, contador);
		if (*contador < capacidad)
			vector[(*contador)++] = nodo->elemento;
		vectorizar_nodos(nodo->der, modo, vector, capacidad, contador);
	} else if (modo == ABB_POSTORDEN) {
		vectorizar_nodos(nodo->izq, modo, vector, capacidad, contador);
		vectorizar_nodos(nodo->der, modo, vector, capacidad, contador);
		if (*contador < capacidad)
			vector[(*contador)++] = nodo->elemento;
	}
}

size_t abb_vectorizar(const abb_t *abb, enum abb_recorrido modo, void **vector,
		      size_t capacidad)
{
	if (!abb || !vector || capacidad == 0)
		return 0;

	size_t contador = 0;

	vectorizar_nodos(abb->raiz, modo, vector, capacidad, &contador);

	return contador;
}

void destruir_arbol_con_destructor(nodo_t *nodo, void (*destructor)(void *))
{
	if (!nodo)
		return;

	destruir_arbol_con_destructor(nodo->izq, destructor);
	destruir_arbol_con_destructor(nodo->der, destructor);

	if (destructor)
		destructor(nodo->elemento);
	nodo->izq =
		NULL; 
	nodo->der = NULL;
	free(nodo);
}

void destruir_arbol(nodo_t *nodo)
{
	if (!nodo)
		return;

	if (nodo->izq)
		destruir_arbol(nodo->izq);
	if (nodo->der)
		destruir_arbol(nodo->der);
	nodo->izq = NULL; 
	nodo->der = NULL;
	free(nodo);
}

void abb_destruir_todo(abb_t *abb, void (*destructor)(void *))
{
	if (!abb)
		return;

	if (destructor) {
		destruir_arbol_con_destructor(abb->raiz, destructor);
	} else {
		destruir_arbol(
			abb->raiz);
	}

	abb->raiz = NULL;
	free(abb); 
}

void abb_destruir(abb_t *abb)
{
	if (!abb)
		return;

	if (abb->raiz) {
		destruir_arbol(abb->raiz);
		abb->raiz = NULL;
	}
	free(abb);
}