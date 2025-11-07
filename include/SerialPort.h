/**
 * @file SerialPort.h
 * @brief Clase para comunicación serial multiplataforma
 * @author Sistema de Decodificación PRT-7
 * @date 2025
 */

#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#ifdef _WIN32
    #include <windows.h>
#else
    #include <termios.h>
#endif

/**
 * @class SerialPort
 * @brief Maneja la comunicación con el puerto serial
 * 
 * Esta clase abstrae las diferencias entre Windows y Linux/Unix para
 * proporcionar una interfaz uniforme de comunicación serial.
 * 
 * Características:
 * - Multiplataforma (Windows/Linux)
 * - Configuración automática del puerto (9600 baud, 8N1)
 * - Lectura línea por línea
 * - Manejo de errores
 */
class SerialPort {
private:
#ifdef _WIN32
    HANDLE hSerial;         ///< Handle del puerto serial (Windows)
#else
    int fd;                 ///< File descriptor del puerto serial (Linux)
    struct termios oldtio;  ///< Configuración antigua del terminal
#endif
    
    bool conectado;         ///< Estado de la conexión
    
    /**
     * @brief Configurar parámetros del puerto serial
     * @return true si la configuración fue exitosa
     */
    bool configurarPuerto();
    
public:
    /**
     * @brief Constructor
     * @param nombrePuerto Nombre del puerto (ej. "COM3" en Windows, "/dev/ttyUSB0" en Linux)
     */
    explicit SerialPort(const char* nombrePuerto);
    
    /**
     * @brief Destructor - Cierra el puerto automáticamente
     */
    ~SerialPort();
    
    /**
     * @brief Verifica si el puerto está conectado
     * @return true si está conectado
     */
    bool estaConectado() const { return conectado; }
    
    /**
     * @brief Lee una línea completa del puerto serial
     * 
     * Lee caracteres hasta encontrar '\n' o hasta llenar el buffer.
     * Elimina automáticamente '\r' y '\n' del final.
     * 
     * @param buffer Buffer donde se almacenará la línea leída
     * @param longitudMax Tamaño máximo del buffer
     * @return Número de caracteres leídos, o -1 si hay error
     */
    int leerLinea(char* buffer, int longitudMax);
    
    /**
     * @brief Lee un solo carácter del puerto serial
     * @param c Referencia donde se almacenará el carácter leído
     * @return true si se leyó exitosamente
     */
    bool leerCaracter(char& c);
    
    /**
     * @brief Cierra el puerto serial
     */
    void cerrar();
};

#endif // SERIAL_PORT_H
