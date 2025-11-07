/**
 * @file TramaLoad.cpp
 * @brief Implementación de la clase TramaLoad
 */

#include "TramaLoad.h"
#include <cstdio>  // Para sprintf

TramaLoad::TramaLoad(char c) : caracter(c) {
    // Inicializar representación
    representacion[0] = '\0';
}

TramaLoad::~TramaLoad() {
    // No hay recursos dinámicos que liberar
}

void TramaLoad::procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) {
    // Decodificar el carácter usando el rotor
    char decodificado = rotor->getMapeo(caracter);
    
    // Insertar el carácter decodificado en la lista de carga
    carga->insertarAlFinal(decodificado);
}

const char* TramaLoad::obtenerRepresentacion() const {
    // Usar const_cast para modificar el buffer mutable
    // Esto es seguro porque no cambia el estado lógico del objeto
    char* buffer = const_cast<char*>(representacion);
    sprintf(buffer, "L,%c", caracter);
    return buffer;
}
