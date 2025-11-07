/**
 * @file ListaDeCarga.cpp
 * @brief Implementación de la lista de carga
 */

#include "ListaDeCarga.h"
#include <cstdio>   // Para printf

ListaDeCarga::ListaDeCarga() : cabeza(nullptr), cola(nullptr), tamanio(0) {
    // Lista vacía
}

ListaDeCarga::~ListaDeCarga() {
    limpiar();
}

void ListaDeCarga::insertarAlFinal(char dato) {
    NodoCarga* nuevo = new NodoCarga(dato);
    
    if (!cabeza) {
        // Lista vacía
        cabeza = nuevo;
        cola = nuevo;
    } else {
        // Insertar al final
        cola->siguiente = nuevo;
        nuevo->previo = cola;
        cola = nuevo;
    }
    
    tamanio++;
}

void ListaDeCarga::imprimirMensaje() const {
    if (!cabeza) {
        printf("(mensaje vacío)\n");
        return;
    }
    
    NodoCarga* actual = cabeza;
    while (actual) {
        printf("%c", actual->dato);
        actual = actual->siguiente;
    }
    printf("\n");
}

void ListaDeCarga::limpiar() {
    NodoCarga* actual = cabeza;
    while (actual) {
        NodoCarga* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    
    cabeza = nullptr;
    cola = nullptr;
    tamanio = 0;
}

char* ListaDeCarga::obtenerMensajeComoString() const {
    // Alocar memoria para la cadena (tamaño + 1 para '\0')
    char* mensaje = new char[tamanio + 1];
    
    int indice = 0;
    NodoCarga* actual = cabeza;
    while (actual) {
        mensaje[indice++] = actual->dato;
        actual = actual->siguiente;
    }
    
    mensaje[indice] = '\0';
    return mensaje;
}
