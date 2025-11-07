/**
 * @file TramaBase.h
 * @brief Clase base abstracta para todas las tramas del protocolo PRT-7
 * @author Sistema de Decodificación PRT-7
 * @date 2025
 */

#ifndef TRAMA_BASE_H
#define TRAMA_BASE_H

// Forward declarations para evitar dependencias circulares
class ListaDeCarga;
class RotorDeMapeo;

/**
 * @class TramaBase
 * @brief Clase base abstracta que define la interfaz para todas las tramas
 * 
 * Esta clase utiliza polimorfismo para permitir que diferentes tipos de tramas
 * (LOAD y MAP) sean procesadas de manera uniforme. El uso de un destructor virtual
 * es CRÍTICO para evitar fugas de memoria al eliminar objetos derivados a través
 * de punteros a la clase base.
 */
class TramaBase {
public:
    /**
     * @brief Destructor virtual - OBLIGATORIO para polimorfismo seguro
     * 
     * Sin un destructor virtual, al hacer delete sobre un puntero TramaBase*
     * que apunta a un objeto derivado, solo se llamaría el destructor de TramaBase,
     * causando fugas de memoria si las clases derivadas tienen recursos propios.
     */
    virtual ~TramaBase() {}
    
    /**
     * @brief Método virtual puro para procesar la trama
     * @param carga Puntero a la lista donde se almacenan los datos decodificados
     * @param rotor Puntero al rotor de mapeo que realiza la transformación de caracteres
     * 
     * Este método debe ser implementado por todas las clases derivadas.
     * Define el comportamiento específico de cada tipo de trama.
     */
    virtual void procesar(ListaDeCarga* carga, RotorDeMapeo* rotor) = 0;
    
    /**
     * @brief Obtiene una representación en texto de la trama para depuración
     * @return Cadena con la representación de la trama
     */
    virtual const char* obtenerRepresentacion() const = 0;
};

#endif // TRAMA_BASE_H
