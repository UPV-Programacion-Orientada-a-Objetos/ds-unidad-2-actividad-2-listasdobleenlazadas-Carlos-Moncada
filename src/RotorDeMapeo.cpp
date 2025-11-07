/**
 * @file RotorDeMapeo.cpp
 * @brief Implementación del rotor de mapeo circular
 */

#include "RotorDeMapeo.h"
#include <cstdio>   // Para printf
#include <cctype>   // Para toupper

RotorDeMapeo::RotorDeMapeo() : cabeza(nullptr), tamanio(0) {
    // Construir la lista circular con A-Z
    NodoRotor* primero = nullptr;
    NodoRotor* ultimo = nullptr;
    
    for (char c = 'A'; c <= 'Z'; ++c) {
        NodoRotor* nuevo = new NodoRotor(c);
        
        if (!primero) {
            // Primer nodo
            primero = nuevo;
            ultimo = nuevo;
        } else {
            // Enlazar nodos
            ultimo->siguiente = nuevo;
            nuevo->previo = ultimo;
            ultimo = nuevo;
        }
        tamanio++;
    }
    
    // Cerrar el círculo
    if (primero && ultimo) {
        ultimo->siguiente = primero;
        primero->previo = ultimo;
    }
    
    // Cabeza apunta inicialmente a 'A'
    cabeza = primero;
}

RotorDeMapeo::~RotorDeMapeo() {
    if (!cabeza) return;
    
    // Romper el círculo para evitar bucle infinito
    NodoRotor* ultimo = cabeza->previo;
    if (ultimo) {
        ultimo->siguiente = nullptr;
    }
    
    // Eliminar todos los nodos
    NodoRotor* actual = cabeza;
    while (actual) {
        NodoRotor* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    
    cabeza = nullptr;
}

NodoRotor* RotorDeMapeo::buscarNodo(char c) const {
    if (!cabeza) return nullptr;
    
    // Convertir a mayúscula
    c = toupper(c);
    
    NodoRotor* actual = cabeza;
    do {
        if (actual->dato == c) {
            return actual;
        }
        actual = actual->siguiente;
    } while (actual != cabeza);
    
    return nullptr;
}

int RotorDeMapeo::calcularDistancia(NodoRotor* desde, NodoRotor* hasta) const {
    if (!desde || !hasta) return 0;
    
    int distancia = 0;
    NodoRotor* actual = desde;
    
    while (actual != hasta && distancia < tamanio) {
        actual = actual->siguiente;
        distancia++;
    }
    
    return distancia;
}

void RotorDeMapeo::rotar(int n) {
    if (!cabeza || n == 0) return;
    
    // Normalizar la rotación (módulo del tamaño)
    n = n % tamanio;
    if (n < 0) n += tamanio;
    
    // Mover la cabeza n posiciones
    for (int i = 0; i < n; ++i) {
        cabeza = cabeza->siguiente;
    }
}

char RotorDeMapeo::getMapeo(char in) {
    // Casos especiales: espacio y otros caracteres no alfabéticos
    if (in == ' ' || in == '\n' || in == '\r' || in == '\t') {
        return in;  // Los espacios y caracteres especiales no se mapean
    }
    
    // Convertir a mayúscula
    in = toupper(in);
    
    // Si no está en A-Z, retornar tal cual
    if (in < 'A' || in > 'Z') {
        return in;
    }
    
    // Algoritmo correcto de mapeo:
    // La rotación del rotor desplaza el alfabeto
    // Si cabeza está en 'C' (rotación +2), entonces:
    //   A -> C (desplazamiento de +2)
    //   B -> D (desplazamiento de +2)
    //   etc.
    
    // Calcular el desplazamiento: distancia desde 'A' hasta 'cabeza'
    NodoRotor* nodoA = buscarNodo('A');
    if (!nodoA) {
        return in;
    }
    
    int desplazamiento = calcularDistancia(nodoA, cabeza);
    
    // Encontrar el nodo del carácter de entrada
    NodoRotor* nodoEntrada = buscarNodo(in);
    if (!nodoEntrada) {
        return in;
    }
    
    // Aplicar el desplazamiento: avanzar 'desplazamiento' posiciones
    NodoRotor* resultado = nodoEntrada;
    for (int i = 0; i < desplazamiento; ++i) {
        resultado = resultado->siguiente;
    }
    
    return resultado->dato;
}

void RotorDeMapeo::imprimirEstado() const {
    if (!cabeza) {
        printf("Rotor vacío\n");
        return;
    }
    
    printf("Estado del Rotor (cabeza en '%c'): ", cabeza->dato);
    
    NodoRotor* actual = cabeza;
    int contador = 0;
    do {
        printf("%c ", actual->dato);
        actual = actual->siguiente;
        contador++;
        if (contador >= tamanio) break;
    } while (actual != cabeza);
    
    printf("\n");
}
