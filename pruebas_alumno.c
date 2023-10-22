#include "pa2m.h"
#include "src/abb.h"
#include "src/abb_estructura_privada.h"

void prueba_simple()
{
	int i = 14;
	int j = 16;
	pa2m_afirmar(i != j, "i=14 es diferente de j=16");
}

//En este caso voy a suponer que los elementos del arbol son int.
int comparador(void *elemento1, void *elemento2)
{
	if (elemento1 == NULL || elemento2 == NULL) {
		return 0;
	}

	if (*(int *)elemento1 > *(int *)elemento2) {
		return 1;
	}

	if (*(int *)elemento1 < *(int *)elemento2) {
		return -1;
	}

	return 0;
}

bool funcion(void *elemento1, void *elemento2)
{
	if (elemento1 == elemento2) {
		return false;
	}
	return true;
}
void prueba_crear()
{
	abb_t *arbol = abb_crear(comparador);

	//pa2m_afirmar((int)(arbol =! NULL), "Se creo el arbol correctamente");
	pa2m_afirmar(arbol->nodo_raiz == NULL, "se cre un arbol vacio");
	pa2m_afirmar(arbol->tamanio == 0, "el tamaño es 0");

	abb_destruir(arbol);
}

void prueba_insertar()
{
	int a = 23, b = 5, c = 37, d = 37, e = 14, f = 38, g = 3, h = 100,
	    i = 120, j = 15, k = 16;
	abb_t *arbol = abb_crear(comparador);

	abb_insertar(arbol, &a);
	pa2m_afirmar(*(int *)arbol->nodo_raiz->elemento == a,
		     "El nodo raiz es el correcto");

	abb_insertar(arbol, &b);
	pa2m_afirmar(*(int *)arbol->nodo_raiz->izquierda->elemento == b,
		     "1.Se inserto  en el lugar adecuado");
	pa2m_afirmar(arbol->tamanio == 2, "el tamaño es el correcto");

	abb_insertar(arbol, &c);
	pa2m_afirmar(*(int *)arbol->nodo_raiz->derecha->elemento == c,
		     "2. otro nodo se inserto correctamente ");

	abb_insertar(arbol, &d);
	abb_insertar(arbol, &e);
	abb_insertar(arbol, &f);
	abb_insertar(arbol, &g);
	abb_insertar(arbol, &h);
	abb_insertar(arbol, &i);
	abb_insertar(arbol, &j);
	abb_insertar(arbol, &k);

	pa2m_afirmar(arbol->tamanio == 11, "el tamaño es el correcto");
	pa2m_afirmar(!abb_vacio(arbol), "el arbol no esta vacio");

	abb_destruir(arbol);
}

void prueba_eliminar()
{
	int a = 23, b = 5, c = 37, d = 37, e = 14, f = 38, g = 3, h = 100,
	    i = 120, j = 15, k = 16;
	abb_t *arbol = abb_crear(comparador);

	abb_insertar(arbol, &a);
	abb_insertar(arbol, &b);
	abb_insertar(arbol, &c);
	abb_insertar(arbol, &d);
	abb_insertar(arbol, &e);
	abb_insertar(arbol, &f);
	abb_insertar(arbol, &g);
	abb_insertar(arbol, &h);
	abb_insertar(arbol, &i);
	abb_insertar(arbol, &j);
	abb_insertar(arbol, &k);

	abb_quitar(arbol, arbol->nodo_raiz->elemento);
	abb_quitar(arbol, arbol->nodo_raiz->elemento);
	abb_quitar(arbol, arbol->nodo_raiz->elemento);
	abb_quitar(arbol, arbol->nodo_raiz->elemento);
	abb_quitar(arbol, arbol->nodo_raiz->elemento);
	abb_quitar(arbol, arbol->nodo_raiz->elemento);
	abb_quitar(arbol, arbol->nodo_raiz->elemento);
	abb_quitar(arbol, arbol->nodo_raiz->elemento);
	abb_quitar(arbol, arbol->nodo_raiz->elemento);
	abb_quitar(arbol, arbol->nodo_raiz->elemento);

	int x = *(int *)abb_quitar(arbol, arbol->nodo_raiz->elemento);
	pa2m_afirmar(x = !NULL,
		     "Se elimina todo el arbol desde la raiz sin error");

	abb_insertar(arbol, &a);
	abb_insertar(arbol, &b);
	abb_insertar(arbol, &c);
	abb_insertar(arbol, &d);
	abb_insertar(arbol, &e);
	abb_insertar(arbol, &f);
	abb_insertar(arbol, &g);
	abb_insertar(arbol, &h);
	abb_insertar(arbol, &i);
	abb_insertar(arbol, &j);
	abb_insertar(arbol, &k);

	pa2m_afirmar(*(int *)abb_quitar(arbol, &a) == a,
		     "elimino el elemento correctamente");
	abb_quitar(arbol, &b);
	abb_quitar(arbol, &c);
	abb_quitar(arbol, &d);
	abb_quitar(arbol, &e);
	abb_quitar(arbol, &f);
	abb_quitar(arbol, &g);
	abb_quitar(arbol, &h);
	abb_quitar(arbol, &i);
	abb_quitar(arbol, &j);
	abb_quitar(arbol, &k);

	pa2m_afirmar(abb_vacio(arbol), "el arbol esta vacio");

	abb_insertar(arbol, &b);
	abb_insertar(arbol, &a);
	abb_insertar(arbol, NULL);
	abb_insertar(arbol, NULL);
	abb_insertar(arbol, NULL);
	abb_insertar(arbol, NULL);
	abb_insertar(arbol, NULL);
	abb_insertar(arbol, NULL);
	abb_insertar(arbol, NULL);
	abb_insertar(arbol, NULL);
	abb_insertar(arbol, NULL);

	printf("Tamaño: %lu\n", arbol->tamanio);
	pa2m_afirmar(arbol->tamanio == 11, "el tamaño del arbol es de: 11 ");

	abb_quitar(arbol, NULL);
	printf("Tamaño: %lu\n", arbol->tamanio);
	pa2m_afirmar(arbol->tamanio == 10, "el tamaño del arbol es de: 10 ");

	abb_quitar(arbol, &b);
	printf("Tamaño: %lu\n", arbol->tamanio);
	pa2m_afirmar(arbol->tamanio == 9, "el tamaño del arbol es de: 9 ");

	abb_quitar(arbol, NULL);
	abb_quitar(arbol, NULL);
	abb_quitar(arbol, &a);
	abb_quitar(arbol, NULL);
	abb_quitar(arbol, NULL);
	abb_quitar(arbol, NULL);
	abb_quitar(arbol, NULL);
	abb_quitar(arbol, NULL);

	printf("Tamaño: %lu\n", arbol->tamanio);
	pa2m_afirmar(arbol->tamanio == 1, "el tamaño del arbol es de: 1");

	abb_destruir(arbol);
}

void pruebas_busqueda()
{
	int a = 23, b = 5, c = 37, d = 37, e = 14, f = 38, g = 3, h = 100,
	    i = 120, j = 15, k = 16;
	abb_t *arbol = abb_crear(comparador);

	abb_insertar(arbol, &a);
	abb_insertar(arbol, &b);
	abb_insertar(arbol, &c);
	abb_insertar(arbol, &d);
	abb_insertar(arbol, &e);
	abb_insertar(arbol, &f);
	abb_insertar(arbol, &g);
	abb_insertar(arbol, &h);
	abb_insertar(arbol, &i);
	abb_insertar(arbol, &j);
	abb_insertar(arbol, &k);

	pa2m_afirmar((*(int *)abb_buscar(arbol, &c)) == c,
		     "encontro el valor correcto");
	pa2m_afirmar((*(int *)abb_buscar(arbol, &d)) == d,
		     "encontro el valor correcto");
	pa2m_afirmar((*(int *)abb_buscar(arbol, &k)) != a,
		     "no devuelve la raiz");

	abb_destruir(arbol);
	return;
}

void pruebas_null()
{
	pa2m_afirmar(abb_crear(NULL) == NULL, "No puede crear un abb");
	pa2m_afirmar(abb_insertar(NULL, NULL) == NULL, "No se puede ");
	pa2m_afirmar(abb_quitar(NULL, NULL) == NULL, "No se puede");
	pa2m_afirmar(abb_buscar(NULL, NULL) == NULL, "No se puede");
	pa2m_afirmar(abb_vacio(NULL) == 1, "No se puede");
	pa2m_afirmar(abb_tamanio(NULL) == 0, "Retorna 0");
	pa2m_afirmar(abb_con_cada_elemento(NULL, INORDEN, NULL, NULL) == 0,
		     "Retorna 0");
	pa2m_afirmar(abb_recorrer(NULL, INORDEN, NULL, 0) == 0, "devuelve 0");

	return;
}

void pruebas()
{
	pa2m_nuevo_grupo(
		"\n======================== PRUEBAS DE ABB ========================");

	pa2m_nuevo_grupo("\n== Crear == ");
	prueba_insertar();

	pa2m_nuevo_grupo("\n== Insertar elementos  ==");
	prueba_insertar();

	pa2m_nuevo_grupo("\n== Eliminar elementos ==");
	prueba_eliminar();

	pa2m_nuevo_grupo("\n== Buscar Elementos ==");
	pruebas_busqueda();

	pa2m_nuevo_grupo("\n== Pruebas con NULL ==");
	pruebas_null();
}

int main()
{
	pa2m_nuevo_grupo(
		"\n======================== XXX ========================");

	pruebas();

	return pa2m_mostrar_reporte();
}
