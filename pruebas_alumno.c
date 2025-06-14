#include "pa2m.h"
#include "src/abb.h"

extern int (*abb_comparador(const abb_t *abb))(const void *, const void *);
extern void *abb_raiz(const abb_t *abb);

int comparador(const void *a, const void *b)
{
	if (!a || !b) {
		printf("Error: Comparador recibió puntero nulo\n");
		return 0;
	}
	if (!a)
		return -1; 
	if (!b)
		return 1;
	return *(int *)a - *(int *)b;
}

void pruebas_creacion_abb()
{
	pa2m_nuevo_grupo("Pruebas de CreaciÃ³n del ABB");

	abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(abb != NULL,
		     "Se puede crear un ABB con un comparador vÃ¡lido");

	int (*comparador_guardado)(const void *, const void *) =
		abb_comparador(abb);
	pa2m_afirmar(comparador_guardado == comparador,
		     "El ABB almacena correctamente el comparador");

	pa2m_afirmar(abb_vacio(abb), "El ABB comienza vacÃ­o");

	pa2m_afirmar(abb_tamanio(abb) == 0, "El ABB comienza con 0 nodos");

	abb_t *abb_nulo = abb_crear(NULL);
	pa2m_afirmar(abb_nulo == NULL,
		     "No se puede crear un ABB con un comparador nulo");

	abb_destruir(abb);
}

void pruebas_insercion_abb()
{
	pa2m_nuevo_grupo("Pruebas de InserciÃ³n en el ABB");

	abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(abb != NULL,
		     "Se puede crear un ABB vÃ¡lido para las pruebas");

	int elemento1 = 10;
	pa2m_afirmar(abb_insertar(abb, &elemento1),
		     "Se puede insertar un elemento en un ABB vacÃ­o");
	pa2m_afirmar(!abb_vacio(abb),
		     "El ABB ya no estÃ¡ vacÃ­o despuÃ©s de la inserciÃ³n");
	pa2m_afirmar(
		abb_tamanio(abb) == 1,
		"El tamaÃ±o del ABB es 1 despuÃ©s de insertar un elemento");
	pa2m_afirmar(abb_existe(abb, &elemento1),
		     "El elemento insertado existe en el ABB");

	int elemento2 = 20;
	pa2m_afirmar(abb_insertar(abb, &elemento2),
		     "Se puede insertar un elemento mayor en el ABB");
	pa2m_afirmar(abb_existe(abb, &elemento2),
		     "El elemento mayor existe en el ABB");

	int elemento3 = 5;
	pa2m_afirmar(abb_insertar(abb, &elemento3),
		     "Se puede insertar un elemento menor en el ABB");
	pa2m_afirmar(abb_existe(abb, &elemento3),
		     "El elemento menor existe en el ABB");

	pa2m_afirmar(abb_insertar(abb, &elemento1),
		     "Se puede insertar un elemento repetido en el ABB");
	pa2m_afirmar(
		abb_tamanio(abb) == 4,
		"El tamaÃ±o del ABB refleja la inserciÃ³n de un elemento repetido");

	pa2m_afirmar(!abb_insertar(NULL, &elemento2),
		     "No se puede insertar en un ABB nulo");

	abb_destruir(abb);
}

void pruebas_insercion_nulo()
{
	pa2m_nuevo_grupo("Pruebas de Inserción de Elementos Nulos en el ABB");

	abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(abb != NULL,
		     "Se puede crear un ABB válido para las pruebas");

	
	pa2m_afirmar(abb_insertar(abb, NULL),
		     "Es posible insertar un elemento nulo en un ABB válido");

	
	pa2m_afirmar(
		!abb_vacio(abb),
		"El ABB no está vacío después de insertar un elemento nulo");

	pa2m_afirmar(abb_tamanio(abb) == 1,
		     "El ABB posee un elemento después de insertar NULL");

	abb_destruir(abb);
}

void pruebas_busqueda_abb()
{
	pa2m_nuevo_grupo("Pruebas de BÃºsqueda en el ABB");

	struct abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(
		abb != NULL,
		"Se puede crear un ABB vÃ¡lido para las pruebas de bÃºsqueda");

	int elemento1 = 10, elemento2 = 20, elemento3 = 5;

	pa2m_afirmar(!abb_existe(abb, &elemento1),
		     "No se encuentra un elemento en un ABB vacÃ­o");

	abb_insertar(abb, &elemento1);
	abb_insertar(abb, &elemento2);
	abb_insertar(abb, &elemento3);

	pa2m_afirmar(abb_existe(abb, &elemento1),
		     "Se encuentra el elemento insertado en el ABB (10)");
	pa2m_afirmar(abb_existe(abb, &elemento2),
		     "Se encuentra el elemento insertado en el ABB (20)");
	pa2m_afirmar(abb_existe(abb, &elemento3),
		     "Se encuentra el elemento insertado en el ABB (5)");

	int elemento_no_existente = 15;
	pa2m_afirmar(
		!abb_existe(abb, &elemento_no_existente),
		"No se encuentra un elemento que no ha sido insertado en el ABB");

	pa2m_afirmar(!abb_existe(NULL, &elemento1),
		     "No se puede buscar en un ABB nulo");

	pa2m_afirmar(!abb_existe(abb, NULL),
		     "No se puede buscar un elemento nulo en el ABB");

	abb_destruir(abb);
}

void pruebas_busqueda_detallada_abb()
{
	pa2m_nuevo_grupo("Pruebas de BÃºsqueda Detallada en el ABB");

	struct abb_t *abb = abb_crear(comparador);

	int elemento1 = 10, elemento2 = 20, elemento3 = 5;

	pa2m_afirmar(abb_buscar(abb, &elemento1) == NULL,
		     "No se encuentra un elemento en un ABB vacÃ­o");

	abb_insertar(abb, &elemento1);
	abb_insertar(abb, &elemento2);
	abb_insertar(abb, &elemento3);

	pa2m_afirmar(abb_buscar(abb, &elemento1) == &elemento1,
		     "Se encuentra correctamente el elemento 10 en el ABB");
	pa2m_afirmar(abb_buscar(abb, &elemento2) == &elemento2,
		     "Se encuentra correctamente el elemento 20 en el ABB");
	pa2m_afirmar(abb_buscar(abb, &elemento3) == &elemento3,
		     "Se encuentra correctamente el elemento 5 en el ABB");

	int elemento_no_existente = 15;
	pa2m_afirmar(abb_buscar(abb, &elemento_no_existente) == NULL,
		     "No se encuentra un elemento que no ha sido insertado");

	pa2m_afirmar(abb_buscar(NULL, &elemento1) == NULL,
		     "No se puede buscar en un ABB nulo");

	pa2m_afirmar(abb_buscar(abb, NULL) == NULL,
		     "No se puede buscar un elemento nulo en el ABB");

	abb_destruir(abb);
}

void pruebas_eliminacion_abb()
{
	pa2m_nuevo_grupo("Pruebas de Eliminación en el ABB");

	abb_t *abb = abb_crear(comparador);

	int elemento1 = 10, elemento2 = 20, elemento3 = 5;

	pa2m_afirmar(abb_sacar(abb, &elemento1) == NULL,
		     "No se puede eliminar de un ABB vacío");

	abb_insertar(abb, &elemento1);
	abb_insertar(abb, &elemento2);
	abb_insertar(abb, &elemento3);

	pa2m_afirmar(abb_raiz(abb) != NULL,
		     "ABB tiene una raíz antes de eliminar");

	void *eliminado = abb_sacar(abb, &elemento1);

	pa2m_afirmar(eliminado == &elemento1,
		     "Se elimina correctamente la raíz del ABB (10)");
	pa2m_afirmar(abb_raiz(abb) != NULL,
		     "ABB tiene una nueva raíz después de eliminar");

	if (abb_raiz(abb) != NULL) {
		bool existe = abb_existe(abb, &elemento1);
		pa2m_afirmar(!existe,
			     "El elemento eliminado ya no existe en el ABB");
	}

	void *eliminado_sin_hijos = abb_sacar(abb, &elemento3);
	pa2m_afirmar(eliminado_sin_hijos == &elemento3,
		     "Se elimina correctamente un nodo sin hijos (5)");

	int elemento_no_existente = 15;
	pa2m_afirmar(
		abb_sacar(abb, &elemento_no_existente) == NULL,
		"No se puede eliminar un elemento que no existe en el ABB");

	pa2m_afirmar(abb_sacar(NULL, &elemento2) == NULL,
		     "No se puede eliminar en un ABB nulo");

	abb_destruir(abb);
}

void pruebas_tamanio_abb()
{
	pa2m_nuevo_grupo("Pruebas de TamaÃ±o del ABB");

	struct abb_t *abb = abb_crear(comparador);

	int elemento1 = 10, elemento2 = 20, elemento3 = 5;

	pa2m_afirmar(abb_tamanio(abb) == 0, "El tamaÃ±o de un ABB vacÃ­o es 0");

	abb_insertar(abb, &elemento1);
	abb_insertar(abb, &elemento2);
	abb_insertar(abb, &elemento3);

	pa2m_afirmar(
		abb_tamanio(abb) == 3,
		"El tamaÃ±o del ABB es 3 despuÃ©s de insertar 3 elementos");

	abb_sacar(abb, &elemento1);

	pa2m_afirmar(
		abb_tamanio(abb) == 2,
		"El tamaÃ±o del ABB es 2 despuÃ©s de eliminar un elemento");

	pa2m_afirmar(abb_tamanio(NULL) == 0, "El tamaÃ±o de un ABB nulo es 0");

	abb_destruir(abb);
}

void pruebas_vacio_abb()
{
	pa2m_nuevo_grupo("Pruebas de ABB VacÃ­o");

	struct abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(abb != NULL, "Se puede crear un ABB vÃ¡lido");

	pa2m_afirmar(abb_vacio(abb), "El ABB reciÃ©n creado estÃ¡ vacÃ­o");

	int elemento = 10;
	abb_insertar(abb, &elemento);

	pa2m_afirmar(!abb_vacio(abb),
		     "El ABB no estÃ¡ vacÃ­o despuÃ©s de insertar un elemento");

	pa2m_afirmar(abb_vacio(NULL),
		     "Un ABB inexistente (NULL) se considera vacÃ­o");
	
	void *eliminado = abb_sacar(abb, &elemento);

	pa2m_afirmar(eliminado == &elemento,
		     "El elemento eliminado es el esperado");

	pa2m_afirmar(
		abb_vacio(abb),
		"El ABB ahora está vacío (después de eliminar su único elemento)");
	pa2m_afirmar(abb_tamanio(abb) == 0,
		     "El tamaño del ABB es 0 tras la última eliminación");
	pa2m_afirmar(
		abb_raiz(abb) == NULL,
		"La raíz del ABB es NULL después de la última eliminación");

	abb_destruir(abb);
}

bool imprimir_elemento(void *elemento, void *ctx)
{
	printf("%d ", *(int *)elemento);
	return true;
}

void pruebas_recorrido_abb()
{
	pa2m_nuevo_grupo("Pruebas de Recorrido del ABB");

	struct abb_t *abb = abb_crear(comparador);

	int elementos[] = { 10, 5, 15, 2, 8, 12, 20 };

	for (size_t i = 0; i < 7; i++) {
		abb_insertar(abb, &elementos[i]);
	}

	printf("Recorrido inorden: ");
	pa2m_afirmar(abb_recorrer(abb, ABB_INORDEN, imprimir_elemento, NULL) ==
			     7,
		     "Se recorren correctamente los 7 elementos en inorden");
	printf("\n");

	printf("Recorrido preorden: ");
	pa2m_afirmar(abb_recorrer(abb, ABB_PREORDEN, imprimir_elemento, NULL) ==
			     7,
		     "Se recorren correctamente los 7 elementos en preorden");
	printf("\n");

	printf("Recorrido postorden: ");
	pa2m_afirmar(abb_recorrer(abb, ABB_POSTORDEN, imprimir_elemento,
				  NULL) == 7,
		     "Se recorren correctamente los 7 elementos en postorden");
	printf("\n");

	pa2m_afirmar(abb_recorrer(NULL, ABB_INORDEN, imprimir_elemento, NULL) ==
			     0,
		     "El recorrido en un ABB nulo no recorre elementos");

	abb_destruir(abb);
}

void pruebas_vectorizacion_abb()
{
	pa2m_nuevo_grupo("Pruebas de VectorizaciÃ³n del ABB");

	struct abb_t *abb = abb_crear(comparador);

	int elementos[] = { 10, 5, 15, 2, 8, 12, 20 };
	void *vector[5];

	for (size_t i = 0; i < 7; i++) {
		abb_insertar(abb, &elementos[i]);
	}

	size_t cantidad = abb_vectorizar(abb, ABB_INORDEN, vector, 5);
	pa2m_afirmar(
		cantidad == 5,
		"Se guardan 5 elementos en el vector con capacidad limitada");
	pa2m_afirmar(*(int *)vector[0] == 2,
		     "El primer elemento del vector es correcto (2)");
	pa2m_afirmar(*(int *)vector[4] == 12,
		     "El Ultimo elemento del vector es correcto (10)");

	void *vector_completo[7];
	cantidad = abb_vectorizar(abb, ABB_INORDEN, vector_completo, 7);
	pa2m_afirmar(
		cantidad == 7,
		"Se guardan todos los elementos en el vector con capacidad suficiente");

	pa2m_afirmar(abb_vectorizar(NULL, ABB_INORDEN, vector, 5) == 0,
		     "No se puede vectorizar un ABB nulo");

	abb_destruir(abb);
}

void pruebas_destruccion_abb()
{
	pa2m_nuevo_grupo("Pruebas de DestrucciÃ³n del ABB (sin destructor)");

	struct abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(
		abb != NULL,
		"Se puede crear un ABB vÃ¡lido para las pruebas de destrucciÃ³n");

	int elementos[] = { 10, 20, 5, 15, 25 };
	for (size_t i = 0; i < 5; i++) {
		abb_insertar(abb, &elementos[i]);
	}

	abb_destruir(abb);
	pa2m_afirmar(
		true,
		"El ABB se destruye correctamente y libera toda la memoria");
}

void destructor_elemento(void *elemento)
{
	printf("Liberando elemento: %d\n", *(int *)elemento);
}

void pruebas_destruccion_completa_abb()
{
	pa2m_nuevo_grupo(
		"Pruebas de DestrucciÃ³n Completa del ABB (con destructor)");

	struct abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(
		abb != NULL,
		"Se puede crear un ABB vÃ¡lido para las pruebas de destrucciÃ³n completa");

	int elementos[] = { 10, 20, 5, 15, 25 };
	for (size_t i = 0; i < 5; i++) {
		abb_insertar(abb, &elementos[i]);
	}

	abb_destruir_todo(abb, destructor_elemento);
	pa2m_afirmar(
		true,
		"El ABB se destruye correctamente aplicando el destructor a cada elemento");
}

bool contar_hasta_tres(void *elemento, void *contador)
{
	(*(size_t *)contador)++;

	return (*(size_t *)contador < 3);
}

void prueba_recorrido_parcial()
{
	pa2m_nuevo_grupo("Prueba de Recorrido Parcial del ABB");

	abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(abb != NULL, "Se puede crear un ABB correctamente");

	int valores[] = { 10, 5, 15, 2, 8, 12, 20 };
	for (int i = 0; i < 7; i++) {
		abb_insertar(abb, &valores[i]);
	}
	pa2m_afirmar(abb_tamanio(abb) == 7,
		     "Se insertaron correctamente 7 elementos");

	size_t contador = 0;

	abb_recorrer(abb, ABB_INORDEN, contar_hasta_tres, &contador);

	pa2m_afirmar(
		contador == 3,
		"El contador refleja la cantidad correcta de elementos recorridos");

	abb_destruir(abb);
}

void pruebas_iterador_interno()
{
	pa2m_nuevo_grupo("Pruebas de Iterador Interno en el ABB");

	abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(abb != NULL, "Se puede crear un ABB válido para pruebas");

	int elementos[] = { 10, 5, 15, 2, 8, 12, 20 };

	for (size_t i = 0; i < 7; i++) {
		abb_insertar(abb, &elementos[i]);
	}

	size_t contador_inorden = 0;
	size_t elementos_recorridos_inorden = abb_recorrer(
		abb, ABB_INORDEN, contar_hasta_tres, &contador_inorden);

	pa2m_afirmar(
		elementos_recorridos_inorden == 3,
		"El iterador inorden detiene el recorrido después de 3 elementos");
	pa2m_afirmar(contador_inorden == 3,
		     "La función de iteración se invocó correctamente 3 veces");

	size_t contador_preorden = 0;
	size_t elementos_recorridos_preorden = abb_recorrer(
		abb, ABB_PREORDEN, contar_hasta_tres, &contador_preorden);

	pa2m_afirmar(
		elementos_recorridos_preorden == 3,
		"El iterador preorden detiene el recorrido después de 3 elementos");
	pa2m_afirmar(contador_preorden == 3,
		     "La función de iteración se invocó correctamente 3 veces");

	size_t contador_postorden = 0;
	size_t elementos_recorridos_postorden = abb_recorrer(
		abb, ABB_POSTORDEN, contar_hasta_tres, &contador_postorden);

	pa2m_afirmar(
		elementos_recorridos_postorden == 3,
		"El iterador postorden detiene el recorrido después de 3 elementos");
	pa2m_afirmar(contador_postorden == 3,
		     "La función de iteración se invocó correctamente 3 veces");

	abb_destruir(abb);
}

void pruebas_abb_vacio_despues_de_eliminar_null()
{
	pa2m_nuevo_grupo("Pruebas: ABB vacío tras eliminar NULL");

	abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(abb != NULL, "Se puede crear un ABB válido");

	void *null_element = NULL;
	pa2m_afirmar(abb_insertar(abb, null_element),
		     "Es posible insertar un elemento NULL en el ABB");

	pa2m_afirmar(!abb_vacio(abb),
		     "El ABB no está vacío después de insertar NULL");

	void *eliminado = abb_sacar(abb, null_element);
	pa2m_afirmar(
		eliminado == null_element,
		"El ABB devuelve correctamente el elemento eliminado (NULL)");

	pa2m_afirmar(
		abb_vacio(abb),
		"Después de eliminar NULL, el ABB debe reconocerse como vacío");

	abb_destruir(abb);
}

void prueba_creacion_y_destruccion()
{
	pa2m_nuevo_grupo("Creación y destrucción de ABBs");

	abb_t *abb = abb_crear(comparador);
	pa2m_afirmar(abb != NULL, "Se crea el ABB correctamente.");
	pa2m_afirmar(abb_vacio(abb), "El ABB se inicia vacío.");
	pa2m_afirmar(abb_tamanio(abb) == 0, "El ABB tiene tamaño 0 al inicio.");

	abb_destruir(abb);
	pa2m_afirmar(1, "El ABB vacío se destruye sin fugas de memoria.");

	abb_t *abb_invalido = abb_crear(NULL);
	pa2m_afirmar(abb_invalido == NULL,
		     "No se puede crear un ABB con comparador NULL.");

	abb_t *abb_con_elementos = abb_crear(comparador);
	int elementos[] = { 10, 20, 30 };
	abb_insertar(abb_con_elementos, &elementos[0]);
	abb_insertar(abb_con_elementos, &elementos[1]);
	abb_insertar(abb_con_elementos, &elementos[2]);
	pa2m_afirmar(abb_tamanio(abb_con_elementos) == 3,
		     "El ABB tiene 3 elementos.");

	abb_destruir_todo(abb_con_elementos, NULL);
	pa2m_afirmar(
		1,
		"El ABB con elementos y destructor NULL se destruye correctamente.");
}

int main()
{
	pa2m_nuevo_grupo("============== Pruebas del ABB ===============");
	pruebas_creacion_abb();
	pruebas_insercion_abb();
	pruebas_insercion_nulo();
	pruebas_busqueda_abb();
	pruebas_busqueda_detallada_abb();
	pruebas_eliminacion_abb();
	pruebas_tamanio_abb();
	pruebas_vacio_abb();
	pruebas_recorrido_abb();
	pruebas_vectorizacion_abb();
	pruebas_destruccion_abb();
	pruebas_destruccion_completa_abb();
	prueba_recorrido_parcial();
	pruebas_iterador_interno();
	pruebas_abb_vacio_despues_de_eliminar_null();

	return pa2m_mostrar_reporte();
}
