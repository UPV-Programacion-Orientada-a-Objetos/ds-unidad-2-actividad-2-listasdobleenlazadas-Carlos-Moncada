/**
 * @file TramaMap.h
 * @brief Clase para tramas de tipo MAP que modifican el rotor de mapeo
 * @author Sistema de Decodificación PRT-7
 * @date 2025
 */

#ifndef TRAMA_MAP_H
#define TRAMA_MAP_H

#include "TramaBase.h"
#include "ListaDeCarga.h"
#include "RotorDeMapeo.h"

/**
 * @class TramaMap
 * @brief Representa una trama MAP del protocolo PRT-7
 * 
 * Las tramas MAP modifican el estado del RotorDeMapeo rotándolo N posiciones.
 * Esto cambia dinámicamente cómo se decodificarán las futuras tramas LOAD.
 * 
 * Formato: M,N donde N es el número de rotaciones (positivo o negativo)
 * Ejemplo: M,5 significa "rotar el rotor 5 posiciones hacia adelante"
 * Ejemplo: M,-3 significa "rotar el rotor 3 posiciones hacia atrás"
 */
class TramaMap : public TramaBase {
private:
    int rotacion;            ///< Número de posiciones a rotar (puede ser negativo)
    char representacion[20]; ///< Buffer para la representación en texto
    
public:
    /**
     * @brief Constructor
     * @param n Número de posiciones a rotar el rotor
     */
    explicit TramaMap(int n);
    
    /**
     * @brief Destructor
     */
    ~TramaMap();
    
    /**
     * @brief Procesa la trama MAP
     * 
     * Rota el rotor N posiciones. Esto modifica cómo se decodificarán
     * las siguientes tramas LOAD.
     * 
     * @param carga Lista de carga (no se utiliza en tramas MAP)
     * @param rotor Rotor que será rotado
     */
    void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) override;
    
    /**
     * @brief Obtiene una representación en texto de la trama
     * @return Cadena con formato "M,N"
     */
    const char* obtenerRepresentacion() const override;
    
    /**
     * @brief Obtiene el valor de rotación
     * @return Número de posiciones a rotar
     */
    int obtenerRotacion() const { return rotacion; }
};

#endif // TRAMA_MAP_H
