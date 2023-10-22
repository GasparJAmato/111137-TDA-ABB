#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

//El compilador necesita que declare esta funcion antes de usarla.
nodo_abb_t *buscar_nodo(nodo_abb_t *nodoBusqueda, void *elemento,
			int (*comparador)(void *, void *));

nodo_abb_t *nodo_abb_crear(void *elemento)
{
	nodo_abb_t *nodo = malloc(sizeof(nodo_abb_t));
	if (nodo == NULL) {
		return NULL;
	}

	nodo->elemento = elemento;
	nodo->derecha = NULL;
	nodo->izquierda = NULL;

	return nodo;
}

abb_t *abb_crear(abb_comparador comparador)
{
	if (comparador == NULL) {
		return NULL;
	}
	abb_t *arbol = malloc(sizeof(abb_t));
	if (arbol == NULL) {
		return NULL;
	}

	arbol->nodo_raiz = NULL;
	arbol->comparador = comparador;
	arbol->tamanio = 0;

	return arbol;
}

abb_t *nodo_insertar_en_lugar_correspondiente(nodo_abb_t *nodoBusqueda,
					      int (*comparador)(void *, void *),
					      nodo_abb_t *nodoAinsertar,
					      abb_t *arbol)
{
	if (nodoBusqueda == NULL || comparador == NULL) {
		return NULL;
	}

	int comp = comparador(nodoAinsertar->elemento, nodoBusqueda->elemento);

	if (comp <= 0) {
		if (nodoBusqueda->izquierda == NULL) {
			nodoBusqueda->izquierda = nodoAinsertar;
			arbol->tamanio += 1;
			return arbol;
		}

		return nodo_insertar_en_lugar_correspondiente(
			nodoBusqueda->izquierda, arbol->comparador,
			nodoAinsertar, arbol);

	} else if (comp > 0) {
		if (nodoBusqueda->derecha == NULL) {
			nodoBusqueda->derecha = nodoAinsertar;
			arbol->tamanio += 1;
			return arbol;
		}
		return nodo_insertar_en_lugar_correspondiente(
			nodoBusqueda->derecha, arbol->comparador, nodoAinsertar,
			arbol);
	}
	return NULL;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (arbol == NULL || arbol->comparador == NULL) {
		return NULL;
	}

	nodo_abb_t *nodo = nodo_abb_crear(elemento);
	if (nodo == NULL) {
		return NULL;
	}

	//cAMBIOO
	if (arbol->nodo_raiz == NULL) {
		arbol->nodo_raiz = nodo;
		arbol->tamanio += 1;
		return arbol;

	} else {
		return nodo_insertar_en_lugar_correspondiente(
			arbol->nodo_raiz, arbol->comparador, nodo, arbol);
	}
	return arbol;
}

//Busca el nodo anterior al nodo ingresado dentro del arbol ingresado.
//devuelve NULL si no existe el nodo

nodo_abb_t *buscarNodoPadre(abb_t *arbol, nodo_abb_t *nodoAbuscar,
			    nodo_abb_t *nodoActual)
{
	if (arbol == NULL || nodoAbuscar == NULL || nodoActual == NULL) {
		return NULL;
	}

	if (nodoAbuscar == arbol->nodo_raiz) {
		return arbol->nodo_raiz;
	}

	int comp =
		arbol->comparador(nodoAbuscar->elemento, nodoActual->elemento);

	if (comp <= 0) {
		if (nodoActual->izquierda == nodoAbuscar) {
			return nodoActual;
		}
		return buscarNodoPadre(arbol, nodoAbuscar,
				       nodoActual->izquierda);

	} else if (comp > 0) {
		if (nodoActual->derecha == nodoAbuscar) {
			return nodoActual;
		}
		return buscarNodoPadre(arbol, nodoAbuscar, nodoActual->derecha);
	}
	return NULL;
}

nodo_abb_t *nodo_reemplazo(nodo_abb_t *nodoActual)
{
	if (nodoActual == NULL) {
		return NULL;
	}

	if (nodoActual->derecha == NULL) {
		return nodoActual;
	}

	return nodo_reemplazo(nodoActual->derecha);
}

//Elimina un nod que tnega dos hijos, contemplando el caso de que sea raiz.
void abb_quitar_con_2_hijos(nodo_abb_t *nodoAEliminar, abb_t *arbol)
{
	if (nodoAEliminar == NULL || arbol == NULL) {
		return;
	}

	//Va una posicion hacia la izquierda y luego dentro de la funcion,
	//se desplaza hacia la derecha hasta encontarar un nodo que no tenga
	//un hijo hacia la derecha.
	nodo_abb_t *nodoReemplazo = nodo_reemplazo(nodoAEliminar->izquierda);
	nodo_abb_t *anteriorAPredecesor =
		buscarNodoPadre(arbol, nodoReemplazo, arbol->nodo_raiz);

	nodo_abb_t *anteriorAEliminado =
		buscarNodoPadre(arbol, nodoAEliminar, arbol->nodo_raiz);

	if (nodoAEliminar == arbol->nodo_raiz) {
		if (anteriorAPredecesor == arbol->nodo_raiz) {
			arbol->nodo_raiz = nodoReemplazo;
			nodoReemplazo->derecha = nodoAEliminar->derecha;
			return;
		}

		anteriorAPredecesor->derecha = nodoReemplazo->izquierda;

		nodoReemplazo->derecha = nodoAEliminar->derecha;
		nodoReemplazo->izquierda = nodoAEliminar->izquierda;

		arbol->nodo_raiz = nodoReemplazo;

		return;
	}

	if (anteriorAEliminado->derecha == nodoAEliminar) {
		if (anteriorAPredecesor == nodoAEliminar) {
			anteriorAEliminado->derecha = nodoReemplazo;
			nodoReemplazo->derecha = nodoAEliminar->derecha;
			return;

		} else {
			anteriorAPredecesor->derecha = nodoReemplazo->izquierda;
			anteriorAEliminado->derecha = nodoReemplazo;
		}

	} else if (anteriorAEliminado->izquierda == nodoAEliminar) {
		if (anteriorAPredecesor == nodoAEliminar) {
			anteriorAEliminado->izquierda = nodoReemplazo;
			nodoReemplazo->derecha = nodoAEliminar->derecha;
			return;

		} else {
			anteriorAPredecesor->derecha = nodoReemplazo->izquierda;
			anteriorAEliminado->izquierda = nodoReemplazo;
		}
	}

	nodoReemplazo->derecha = nodoAEliminar->derecha;
	nodoReemplazo->izquierda = nodoAEliminar->izquierda;
}
void abb_quitar_con_1_hijo(abb_t *arbol, nodo_abb_t *nodoAEliminar)
{
	nodo_abb_t *anterior =
		buscarNodoPadre(arbol, nodoAEliminar, arbol->nodo_raiz);
	if (nodoAEliminar == arbol->nodo_raiz) {
		if (arbol->nodo_raiz->derecha != NULL) {
			arbol->nodo_raiz = nodoAEliminar->derecha;

		} else {
			arbol->nodo_raiz = nodoAEliminar->izquierda;
		}
		return;
	}

	if (anterior->derecha == nodoAEliminar) {
		if (nodoAEliminar->derecha != NULL) {
			anterior->derecha = nodoAEliminar->derecha;

		} else {
			anterior->derecha = nodoAEliminar->izquierda;
		}
	} else if (anterior->izquierda == nodoAEliminar) {
		if (nodoAEliminar->derecha != NULL) {
			anterior->izquierda = nodoAEliminar->derecha;

		} else {
			anterior->izquierda = nodoAEliminar->izquierda;
		}
	}
}

void abb_quitar_con_0_hijo(abb_t *arbol, nodo_abb_t *nodoAEliminar)
{
	if (arbol == NULL || nodoAEliminar == NULL) {
		return;
	}
	if (nodoAEliminar == arbol->nodo_raiz) {
		arbol->nodo_raiz = NULL;

		return;
	}

	nodo_abb_t *anterior =
		buscarNodoPadre(arbol, nodoAEliminar, arbol->nodo_raiz);

	if (anterior->derecha == nodoAEliminar) {
		anterior->derecha = NULL;

	} else if (anterior->izquierda == nodoAEliminar) {
		anterior->izquierda = NULL;
	}
}

//si hay dos elementos iguales, elimina el primero que encuentre. y que devuelva el elemento
void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (arbol == NULL ||
	    buscar_nodo(arbol->nodo_raiz, elemento, arbol->comparador) ==
		    NULL ||
	    arbol->nodo_raiz == NULL) {
		return NULL;
	}

	nodo_abb_t *nodoAEliminar =
		buscar_nodo(arbol->nodo_raiz, elemento, arbol->comparador);
	if (nodoAEliminar == NULL) {
		return NULL;
	}

	//Caso de 0 hijos
	if (nodoAEliminar->derecha == NULL &&
	    nodoAEliminar->izquierda == NULL) {
		abb_quitar_con_0_hijo(arbol, nodoAEliminar);

		//caso de 1 hijo
	} else if ((nodoAEliminar->derecha == NULL) ^
		   (nodoAEliminar->izquierda == NULL)) {
		abb_quitar_con_1_hijo(arbol, nodoAEliminar);

		//caso de 2 hijos
	} else {
		abb_quitar_con_2_hijos(nodoAEliminar, arbol);
	}

	arbol->tamanio -= 1;
	free(nodoAEliminar);

	return elemento;
}

//Devuleve el primer nodo que coincida con el elemento dado.
nodo_abb_t *buscar_nodo(nodo_abb_t *nodoBusqueda, void *elemento,
			int (*comparador)(void *, void *))
{
	if (nodoBusqueda == NULL || comparador == NULL) {
		return NULL;
	}

	int comp = comparador(elemento, nodoBusqueda->elemento);

	if (comp == 0) {
		return nodoBusqueda;

	} else if (comp < 0) {
		return buscar_nodo(nodoBusqueda->izquierda, elemento,
				   comparador);

	} else {
		return buscar_nodo(nodoBusqueda->derecha, elemento, comparador);
	}
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (arbol == NULL || arbol->nodo_raiz == NULL) {
		return NULL;
	}

	nodo_abb_t *nodoEncontrado =
		buscar_nodo(arbol->nodo_raiz, elemento, arbol->comparador);
	if (nodoEncontrado == NULL) {
		return NULL;
	}

	return nodoEncontrado->elemento;
}

bool abb_vacio(abb_t *arbol)
{
	if (arbol == NULL || arbol->tamanio == 0 || arbol->nodo_raiz == NULL) {
		return true;
	} else {
		return false;
	}
}

size_t abb_tamanio(abb_t *arbol)
{
	if (arbol == NULL) {
		return 0;
	}
	return arbol->tamanio;
}

void abb_destruir_nodo(nodo_abb_t *nodoActual)
{
	if (nodoActual == NULL) {
		return;
	}

	abb_destruir_nodo(nodoActual->izquierda);
	abb_destruir_nodo(nodoActual->derecha);

	free(nodoActual);
	return;
}

void abb_destruir(abb_t *arbol)
{
	if (arbol == NULL) {
		return;
	}

	if (arbol->nodo_raiz != NULL) {
		abb_destruir_nodo(arbol->nodo_raiz);
	}

	free(arbol);
}

void abb_destruir_todo_nodos(nodo_abb_t *nodoActual, void (*destructor)(void *))
{
	if (nodoActual == NULL || destructor == NULL) {
		return;
	}

	if (nodoActual->izquierda != NULL) {
		abb_destruir_todo_nodos(nodoActual->izquierda, destructor);
	}
	if (nodoActual->derecha != NULL) {
		abb_destruir_todo_nodos(nodoActual->derecha, destructor);
	}

	if (destructor != NULL) {
		destructor(nodoActual->elemento);
	}

	free(nodoActual);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (arbol == NULL) {
		return;
	}
	if (destructor == NULL) {
		abb_destruir(arbol);
		return;
	}

	abb_destruir_todo_nodos(arbol->nodo_raiz, destructor);
	free(arbol);
}

typedef struct contador {
	size_t contador;

} contador_t;

bool abb_con_cada_elemento_inorden(nodo_abb_t *nodoActual,
				   bool (*funcion)(void *, void *), void *aux,
				   contador_t *contador)
{
	if (!nodoActual) {
		return true;
	}

	if (!abb_con_cada_elemento_inorden(nodoActual->izquierda, funcion, aux,
					   contador)) {
		return false;
	}

	contador->contador += 1;
	if (!funcion(nodoActual->elemento, aux)) {
		return false;
	}

	if (!abb_con_cada_elemento_inorden(nodoActual->derecha, funcion, aux,
					   contador)) {
		return false;
	}

	return true;
}

bool abb_con_cada_elemento_preorden(nodo_abb_t *nodoActual,
				    bool (*funcion)(void *, void *), void *aux,
				    contador_t *contador)
{
	if (!nodoActual) {
		return true;
	}

	contador->contador += 1;
	if (!funcion(nodoActual->elemento, aux)) {
		return false;
	}

	if (!abb_con_cada_elemento_preorden(nodoActual->izquierda, funcion, aux,
					    contador)) {
		return false;
	}

	if (!abb_con_cada_elemento_preorden(nodoActual->derecha, funcion, aux,
					    contador)) {
		return false;
	}

	return true;
}

bool abb_con_cada_elemento_postorden(nodo_abb_t *nodoActual,
				     bool (*funcion)(void *, void *), void *aux,
				     contador_t *contador)
{
	if (!nodoActual) {
		return true;
	}

	if (!abb_con_cada_elemento_postorden(nodoActual->izquierda, funcion,
					     aux, contador)) {
		return false;
	}

	if (!abb_con_cada_elemento_postorden(nodoActual->derecha, funcion, aux,
					     contador)) {
		return false;
	}

	contador->contador += 1;
	if (!funcion(nodoActual->elemento, aux)) {
		return false;
	}

	return true;
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (arbol == NULL || funcion == NULL || arbol->nodo_raiz == NULL) {
		return 0;
	}

	contador_t *contador = malloc(sizeof(contador_t));
	contador->contador = 0;

	if (recorrido == INORDEN) {
		abb_con_cada_elemento_inorden(arbol->nodo_raiz, funcion, aux,
					      contador);

	} else if (recorrido == PREORDEN) {
		abb_con_cada_elemento_preorden(arbol->nodo_raiz, funcion, aux,
					       contador);

	} else if (recorrido == POSTORDEN) {
		abb_con_cada_elemento_postorden(arbol->nodo_raiz, funcion, aux,
						contador);
	}

	size_t cont = contador->contador;
	free(contador);
	return cont;
}
bool funcion2(contador_t *contador, size_t tamanio_array, void **array,
	      nodo_abb_t *nodo)
{
	array[contador->contador] = nodo->elemento;
	contador->contador += 1;
	if (contador->contador == tamanio_array) {
		return false;
	}

	return true;
}

bool abb_recorrer_inorden(nodo_abb_t *nodoActual, void **array,
			  contador_t *contador, size_t tamanio_array)
{
	if (!nodoActual) {
		return true;
	}

	if (!abb_recorrer_inorden(nodoActual->izquierda, array, contador,
				  tamanio_array)) {
		return false;
	}

	if (!funcion2(contador, tamanio_array, array, nodoActual)) {
		return false;
	}

	if (!abb_recorrer_inorden(nodoActual->derecha, array, contador,
				  tamanio_array)) {
		return false;
	}

	return true;
}

bool abb_recorrer_preorden(nodo_abb_t *nodoActual, void **array,
			   contador_t *contador, size_t tamanio_array)
{
	if (!nodoActual) {
		return true;
	}

	if (!funcion2(contador, tamanio_array, array, nodoActual)) {
		return false;
	}

	if (!abb_recorrer_preorden(nodoActual->izquierda, array, contador,
				   tamanio_array)) {
		return false;
	}

	if (!abb_recorrer_preorden(nodoActual->derecha, array, contador,
				   tamanio_array)) {
		return false;
	}

	return true;
}

bool abb_recorrer_postorden(nodo_abb_t *nodoActual, void **array,
			    contador_t *contador, size_t tamanio_array)
{
	if (!nodoActual) {
		return true;
	}

	if (!abb_recorrer_postorden(nodoActual->izquierda, array, contador,
				    tamanio_array)) {
		return false;
	}

	if (!abb_recorrer_postorden(nodoActual->derecha, array, contador,
				    tamanio_array)) {
		return false;
	}

	if (!funcion2(contador, tamanio_array, array, nodoActual)) {
		return false;
	}

	return true;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (arbol == NULL || arbol->nodo_raiz == NULL || array == NULL ||
	    tamanio_array == 0) {
		return 0;
	}

	contador_t *contador = malloc(sizeof(contador_t));
	contador->contador = 0;

	if (recorrido == INORDEN) {
		abb_recorrer_inorden(arbol->nodo_raiz, array, contador,
				     tamanio_array);

	} else if (recorrido == PREORDEN) {
		abb_recorrer_preorden(arbol->nodo_raiz, array, contador,
				      tamanio_array);

	} else if (recorrido == POSTORDEN) {
		abb_recorrer_postorden(arbol->nodo_raiz, array, contador,
				       tamanio_array);
	}

	size_t cont = contador->contador;
	free(contador);
	return cont;
}
