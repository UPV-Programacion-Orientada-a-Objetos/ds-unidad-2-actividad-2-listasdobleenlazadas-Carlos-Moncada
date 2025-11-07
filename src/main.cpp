/**
 * @file main.cpp
 * @brief Programa principal del Decodificador PRT-7
 * @author Sistema de Decodificación PRT-7
 * @date 2025
 * 
 * Este programa decodifica mensajes ocultos transmitidos mediante el protocolo PRT-7
 * desde un dispositivo Arduino conectado por puerto serial.
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "TramaBase.h"
#include "TramaLoad.h"
#include "TramaMap.h"
#include "ListaDeCarga.h"
#include "RotorDeMapeo.h"
#include "SerialPort.h"

/**
 * @brief Parsea una línea recibida y crea la trama correspondiente
 * @param linea Línea de texto recibida del puerto serial (ej. "L,A" o "M,5")
 * @return Puntero a TramaBase (TramaLoad o TramaMap), o nullptr si el formato es inválido
 */
TramaBase* parsearTrama(const char* linea) {
    if (!linea || strlen(linea) < 3) {
        return nullptr;
    }
    
    // El formato esperado es: "X,Y" donde X es el tipo (L o M) y Y es el dato
    char tipo = linea[0];
    
    // Verificar que hay una coma
    if (linea[1] != ',') {
        printf("Advertencia: Formato inválido (falta coma): %s\n", linea);
        return nullptr;
    }
    
    // Obtener el dato después de la coma
    const char* dato = linea + 2;
    
    if (tipo == 'L' || tipo == 'l') {
        // Trama LOAD: L,X donde X es un carácter
        if (strlen(dato) < 1) {
            printf("Advertencia: Trama LOAD sin carácter\n");
            return nullptr;
        }
        return new TramaLoad(dato[0]);
    } 
    else if (tipo == 'M' || tipo == 'm') {
        // Trama MAP: M,N donde N es un número entero
        int rotacion = atoi(dato);
        return new TramaMap(rotacion);
    }
    else {
        printf("Advertencia: Tipo de trama desconocido: %c\n", tipo);
        return nullptr;
    }
}

/**
 * @brief Imprime el banner inicial del programa
 */
void imprimirBanner() {
    printf("\n");
    printf("====================================================\n");
    printf("     DECODIFICADOR DE PROTOCOLO INDUSTRIAL PRT-7    \n");
    printf("====================================================\n");
    printf("\n");
}

/**
 * @brief Imprime las instrucciones de uso
 */
void imprimirInstrucciones() {
    printf("Instrucciones:\n");
    printf("1. Conecte el Arduino al puerto serial\n");
    printf("2. El programa leerá las tramas automáticamente\n");
    printf("3. Al finalizar, se mostrará el mensaje decodificado\n");
    printf("\n");
}

/**
 * @brief Solicita al usuario el nombre del puerto serial
 * @param buffer Buffer donde se almacenará el nombre del puerto
 * @param tamBuffer Tamaño del buffer
 */
void solicitarPuerto(char* buffer, int tamBuffer) {
    printf("Ingrese el nombre del puerto serial\n");
#ifdef _WIN32
    printf("  (Windows: COM1, COM2, COM3, etc.): ");
#else
    printf("  (Linux: /dev/ttyUSB0, /dev/ttyACM0, etc.): ");
#endif
    
    if (fgets(buffer, tamBuffer, stdin)) {
        // Eliminar el salto de línea
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        }
    }
}

/**
 * @brief Procesa el flujo de tramas desde el puerto serial
 * @param puerto Puntero al objeto SerialPort
 * @param carga Puntero a la lista de carga
 * @param rotor Puntero al rotor de mapeo
 * @return Número de tramas procesadas
 */
int procesarFlujo(SerialPort* puerto, ListaDeCarga* carga, RotorDeMapeo* rotor) {
    const int BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE];
    int tramasProcesadas = 0;
    int lineasVacias = 0;
    const int MAX_LINEAS_VACIAS = 10;  // Timeout después de 10 líneas vacías consecutivas
    
    printf("\nEsperando tramas del Arduino...\n");
    printf("(Presione Ctrl+C para detener si es necesario)\n\n");
    
    while (true) {
        int bytesLeidos = puerto->leerLinea(buffer, BUFFER_SIZE);
        
        if (bytesLeidos < 0) {
            printf("Error al leer del puerto serial\n");
            break;
        }
        
        if (bytesLeidos == 0) {
            lineasVacias++;
            if (lineasVacias >= MAX_LINEAS_VACIAS) {
                printf("\nNo se reciben más datos. Finalizando...\n");
                break;
            }
            continue;
        }
        
        lineasVacias = 0;  // Resetear contador de líneas vacías
        
        // Parsear la trama
        TramaBase* trama = parsearTrama(buffer);
        
        if (!trama) {
            printf("Trama inválida: [%s]\n", buffer);
            continue;
        }
        
        // Mostrar información de la trama
        printf("Trama recibida: [%s] -> Procesando... ", trama->obtenerRepresentacion());
        
        // Procesar la trama
        trama->procesar(carga, rotor);
        tramasProcesadas++;
        
        // Mostrar información adicional según el tipo
        TramaLoad* tramaLoad = dynamic_cast<TramaLoad*>(trama);
        TramaMap* tramaMap = dynamic_cast<TramaMap*>(trama);
        
        if (tramaLoad) {
            printf("-> Carácter procesado. Mensaje parcial: ");
            carga->imprimirMensaje();
        } 
        else if (tramaMap) {
            int rot = tramaMap->obtenerRotacion();
            printf("-> ROTANDO ROTOR %+d (cabeza ahora en '%c')\n", rot, rotor->obtenerCabeza());
        }
        
        // Liberar memoria de la trama
        delete trama;
    }
    
    return tramasProcesadas;
}

/**
 * @brief Función principal del programa
 */
int main() {
    imprimirBanner();
    imprimirInstrucciones();
    
    // Solicitar puerto serial
    char nombrePuerto[100];
    solicitarPuerto(nombrePuerto, sizeof(nombrePuerto));
    
    printf("\nIniciando Decodificador PRT-7...\n");
    printf("Conectando a puerto: %s\n", nombrePuerto);
    
    // Abrir puerto serial
    SerialPort puerto(nombrePuerto);
    
    if (!puerto.estaConectado()) {
        printf("\nError: No se pudo conectar al puerto %s\n", nombrePuerto);
        printf("Verifique que:\n");
        printf("  - El Arduino está conectado\n");
        printf("  - El puerto es correcto\n");
        printf("  - Tiene permisos para acceder al puerto\n");
        return 1;
    }
    
    printf("Conexión establecida exitosamente.\n");
    
    // Inicializar estructuras de datos
    ListaDeCarga carga;
    RotorDeMapeo rotor;
    
    printf("\nEstructuras inicializadas:\n");
    printf("  - Lista de Carga: vacía\n");
    printf("  - Rotor de Mapeo: posición inicial (A-Z, cabeza en 'A')\n");
    
    // Procesar el flujo de tramas
    int tramasProcesadas = procesarFlujo(&puerto, &carga, &rotor);
    
    // Mostrar resultados
    printf("\n");
    printf("====================================================\n");
    printf("              DECODIFICACIÓN COMPLETADA             \n");
    printf("====================================================\n");
    printf("\n");
    printf("Estadísticas:\n");
    printf("  - Tramas procesadas: %d\n", tramasProcesadas);
    printf("  - Caracteres decodificados: %d\n", carga.obtenerTamanio());
    printf("\n");
    printf("---------------------------------------------------\n");
    printf("MENSAJE OCULTO ENSAMBLADO:\n");
    printf("---------------------------------------------------\n");
    
    if (carga.estaVacia()) {
        printf("(mensaje vacío - no se recibieron tramas LOAD)\n");
    } else {
        carga.imprimirMensaje();
    }
    
    printf("---------------------------------------------------\n");
    printf("\n");
    
    // Cerrar puerto
    puerto.cerrar();
    printf("Puerto serial cerrado.\n");
    printf("Sistema apagado correctamente.\n");
    printf("\n");
    
    return 0;
}
