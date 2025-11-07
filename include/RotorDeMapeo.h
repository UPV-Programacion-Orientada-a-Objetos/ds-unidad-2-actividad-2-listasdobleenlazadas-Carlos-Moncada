/**
 * @file RotorDeMapeo.h
 * @brief Lista circular doblemente enlazada que implementa el rotor de mapeo
 * @author Sistema de Decodificación PRT-7
 * @date 2025
 */

#ifndef ROTOR_DE_MAPEO_H
#define ROTOR_DE_MAPEO_H

/**
 * @struct NodoRotor
 * @brief Nodo de la lista circular doblemente enlazada
 */
struct NodoRotor {
    char dato;              ///< Carácter almacenado (A-Z)
    NodoRotor* siguiente;   ///< Puntero al siguiente nodo
    NodoRotor* previo;      ///< Puntero al nodo anterior
    
    /**
     * @brief Constructor del nodo
     * @param c Carácter a almacenar
     */
    explicit NodoRotor(char c) : dato(c), siguiente(nullptr), previo(nullptr) {}
};

/**
 * @class RotorDeMapeo
 * @brief Implementa un disco de cifrado rotatorio (similar a máquina Enigma)
 * 
 * Esta clase implementa una lista circular doblemente enlazada que contiene
 * el alfabeto A-Z. El rotor puede rotarse hacia adelante o atrás, y proporciona
 * un mapeo de caracteres basado en su posición actual.
 * 
 * Funcionamiento:
 * - Inicialmente, 'cabeza' apunta a 'A' (posición 0)
 * - Al rotar +N, 'cabeza' se mueve N posiciones hacia adelante
 * - Al rotar -N, 'cabeza' se mueve N posiciones hacia atrás
 * - El mapeo se calcula basándose en la posición relativa a 'cabeza'
 * 
 * Ejemplo:
 * Estado inicial: A B C D E F ... (cabeza en A)
 * Rotar +2:       C D E F G H ... (cabeza en C, ahora A->C, B->D, etc.)
 */
class RotorDeMapeo {
private:
    NodoRotor* cabeza;      ///< Puntero a la posición "cero" actual del rotor
    int tamanio;            ///< Número de elementos en el rotor (26 para A-Z)
    
    /**
     * @brief Encuentra un nodo que contiene un carácter específico
     * @param c Carácter a buscar
     * @return Puntero al nodo que contiene el carácter, o nullptr si no existe
     */
    NodoRotor* buscarNodo(char c) const;
    
    /**
     * @brief Calcula la distancia entre dos nodos en la lista circular
     * @param desde Nodo de inicio
     * @param hasta Nodo de destino
     * @return Número de pasos desde 'desde' hasta 'hasta'
     */
    int calcularDistancia(NodoRotor* desde, NodoRotor* hasta) const;
    
public:
    /**
     * @brief Constructor - Inicializa el rotor con el alfabeto A-Z
     */
    RotorDeMapeo();
    
    /**
     * @brief Destructor - Libera toda la memoria de los nodos
     */
    ~RotorDeMapeo();
    
    /**
     * @brief Rota el rotor N posiciones
     * 
     * Si N > 0: rota hacia adelante (siguiente)
     * Si N < 0: rota hacia atrás (previo)
     * Si N == 0: no hace nada
     * 
     * La rotación es circular, por lo que rotar 26 posiciones equivale a no rotar.
     * 
     * @param n Número de posiciones a rotar (puede ser negativo)
     */
    void rotar(int n);
    
    /**
     * @brief Obtiene el carácter mapeado según la rotación actual
     * 
     * Algoritmo:
     * 1. Buscar el carácter de entrada en el rotor
     * 2. Calcular su distancia desde 'cabeza'
     * 3. Retornar el carácter que está a esa misma distancia desde la 'A' original
     * 
     * Ejemplo:
     * Estado: cabeza en 'C' (rotación +2)
     * Input: 'A' -> está a -2 de 'cabeza' -> mapea a 'A'+2 = 'C'
     * Input: 'W' -> está a 20 de 'cabeza' -> mapea a 'A'+20 = 'U'... más rotación
     * 
     * @param in Carácter a mapear
     * @return Carácter mapeado según la rotación actual
     */
    char getMapeo(char in);
    
    /**
     * @brief Imprime el estado actual del rotor (para depuración)
     */
    void imprimirEstado() const;
    
    /**
     * @brief Obtiene el carácter actual en la posición de cabeza
     * @return Carácter en la posición de cabeza
     */
    char obtenerCabeza() const { return cabeza ? cabeza->dato : '\0'; }
};

#endif // ROTOR_DE_MAPEO_H
