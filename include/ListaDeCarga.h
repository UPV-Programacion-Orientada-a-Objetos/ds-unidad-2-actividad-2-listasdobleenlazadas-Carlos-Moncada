/**
 * @file ListaDeCarga.h
 * @brief Lista doblemente enlazada para almacenar caracteres decodificados
 * @author Sistema de Decodificación PRT-7
 * @date 2025
 */

#ifndef LISTA_DE_CARGA_H
#define LISTA_DE_CARGA_H

/**
 * @struct NodoCarga
 * @brief Nodo de la lista doblemente enlazada
 */
struct NodoCarga {
    char dato;              ///< Carácter almacenado
    NodoCarga* siguiente;   ///< Puntero al siguiente nodo
    NodoCarga* previo;      ///< Puntero al nodo anterior
    
    /**
     * @brief Constructor del nodo
     * @param c Carácter a almacenar
     */
    explicit NodoCarga(char c) : dato(c), siguiente(nullptr), previo(nullptr) {}
};

/**
 * @class ListaDeCarga
 * @brief Lista doblemente enlazada para almacenar el mensaje decodificado
 * 
 * Esta lista almacena los caracteres decodificados en el orden en que fueron
 * procesados. Al final del proceso, contiene el mensaje oculto completo.
 * 
 * Características:
 * - Inserción eficiente al final (O(1))
 * - Navegación bidireccional
 * - Preserva el orden de llegada de los datos
 */
class ListaDeCarga {
private:
    NodoCarga* cabeza;      ///< Puntero al primer nodo
    NodoCarga* cola;        ///< Puntero al último nodo
    int tamanio;            ///< Número de elementos en la lista
    
public:
    /**
     * @brief Constructor - Inicializa una lista vacía
     */
    ListaDeCarga();
    
    /**
     * @brief Destructor - Libera toda la memoria de los nodos
     */
    ~ListaDeCarga();
    
    /**
     * @brief Inserta un carácter al final de la lista
     * 
     * Complejidad: O(1) gracias al puntero 'cola'
     * 
     * @param dato Carácter a insertar
     */
    void insertarAlFinal(char dato);
    
    /**
     * @brief Imprime el mensaje completo almacenado en la lista
     * 
     * Recorre la lista desde la cabeza hasta la cola e imprime
     * cada carácter. Este es el mensaje oculto decodificado.
     */
    void imprimirMensaje() const;
    
    /**
     * @brief Obtiene el tamaño actual de la lista
     * @return Número de caracteres en la lista
     */
    int obtenerTamanio() const { return tamanio; }
    
    /**
     * @brief Verifica si la lista está vacía
     * @return true si la lista está vacía, false en caso contrario
     */
    bool estaVacia() const { return tamanio == 0; }
    
    /**
     * @brief Limpia toda la lista (elimina todos los nodos)
     */
    void limpiar();
    
    /**
     * @brief Obtiene el mensaje como una cadena (aloca memoria nueva)
     * @return Puntero a cadena con el mensaje (el llamador debe liberar con delete[])
     */
    char* obtenerMensajeComoString() const;
};

#endif // LISTA_DE_CARGA_H
