/**
 * @file TramaMap.cpp
 * @brief Implementación de la clase TramaMap
 */

#include "TramaMap.h"
#include <cstdio>  // Para sprintf

TramaMap::TramaMap(int n) : rotacion(n) {
    // Inicializar representación
    representacion[0] = '\0';
}

TramaMap::~TramaMap() {
    // No hay recursos dinámicos que liberar
}

void TramaMap::procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) {
    // Simplemente rotar el rotor
    // La lista de carga no se usa en tramas MAP
    (void)carga; // Evitar warning de parámetro no utilizado
    
    rotor->rotar(rotacion);
}

const char* TramaMap::obtenerRepresentacion() const {
    // Usar const_cast para modificar el buffer mutable
    char* buffer = const_cast<char*>(representacion);
    sprintf(buffer, "M,%d", rotacion);
    return buffer;
}
