/**
 * @file TramaLoad.h
 * @brief Clase para tramas de tipo LOAD que contienen fragmentos de datos
 * @author Sistema de Decodificación PRT-7
 * @date 2025
 */

#ifndef TRAMA_LOAD_H
#define TRAMA_LOAD_H

#include "TramaBase.h"
#include "ListaDeCarga.h"
#include "RotorDeMapeo.h"

/**
 * @class TramaLoad
 * @brief Representa una trama LOAD del protocolo PRT-7
 * 
 * Las tramas LOAD contienen un carácter que debe ser decodificado usando
 * el estado actual del RotorDeMapeo y luego almacenado en la ListaDeCarga.
 * 
 * Formato: L,X donde X es el carácter a decodificar
 * Ejemplo: L,H significa "cargar el carácter H (después de decodificarlo)"
 */
class TramaLoad : public TramaBase {
private:
    char caracter;           ///< Carácter contenido en la trama
    char representacion[10]; ///< Buffer para la representación en texto
    
public:
    /**
     * @brief Constructor
     * @param c Carácter que contiene esta trama LOAD
     */
    explicit TramaLoad(char c);
    
    /**
     * @brief Destructor
     */
    ~TramaLoad();
    
    /**
     * @brief Procesa la trama LOAD
     * 
     * 1. Toma el carácter almacenado
     * 2. Lo decodifica usando el rotor actual
     * 3. Inserta el resultado en la lista de carga
     * 
     * @param carga Lista donde se insertará el carácter decodificado
     * @param rotor Rotor usado para decodificar el carácter
     */
    void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) override;
    
    /**
     * @brief Obtiene una representación en texto de la trama
     * @return Cadena con formato "L,X"
     */
    const char* obtenerRepresentacion() const override;
    
    /**
     * @brief Obtiene el carácter almacenado
     * @return Carácter de la trama
     */
    char obtenerCaracter() const { return caracter; }
};

#endif // TRAMA_LOAD_H
