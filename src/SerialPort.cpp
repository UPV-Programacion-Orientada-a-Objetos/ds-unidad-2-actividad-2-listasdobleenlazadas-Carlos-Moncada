/**
 * @file SerialPort.cpp
 * @brief Implementación de comunicación serial multiplataforma
 */

#include "SerialPort.h"
#include <cstdio>
#include <cstring>

#ifndef _WIN32
    #include <fcntl.h>
    #include <unistd.h>
    #include <errno.h>
#endif

SerialPort::SerialPort(const char* nombrePuerto) : conectado(false) {
#ifdef _WIN32
    // Windows
    hSerial = CreateFileA(nombrePuerto,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         nullptr,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         nullptr);
    
    if (hSerial == INVALID_HANDLE_VALUE) {
        printf("Error: No se pudo abrir el puerto %s\n", nombrePuerto);
        return;
    }
    
    conectado = configurarPuerto();
    
    if (!conectado) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
#else
    // Linux/Unix
    fd = open(nombrePuerto, O_RDWR | O_NOCTTY);
    
    if (fd < 0) {
        printf("Error: No se pudo abrir el puerto %s (errno: %d)\n", nombrePuerto, errno);
        return;
    }
    
    conectado = configurarPuerto();
    
    if (!conectado) {
        close(fd);
        fd = -1;
    }
#endif
}

SerialPort::~SerialPort() {
    cerrar();
}

bool SerialPort::configurarPuerto() {
#ifdef _WIN32
    // Configuración para Windows
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        printf("Error: No se pudo obtener el estado del puerto\n");
        return false;
    }
    
    // Configurar parámetros: 9600 baud, 8N1
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        printf("Error: No se pudo configurar el puerto\n");
        return false;
    }
    
    // Configurar timeouts
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        printf("Error: No se pudieron configurar los timeouts\n");
        return false;
    }
    
    return true;
#else
    // Configuración para Linux/Unix
    struct termios newtio;
    
    // Guardar configuración antigua
    tcgetattr(fd, &oldtio);
    
    // Limpiar estructura
    memset(&newtio, 0, sizeof(newtio));
    
    // Configurar parámetros: 9600 baud, 8N1
    newtio.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;
    
    // Timeouts
    newtio.c_cc[VTIME] = 1;  // Timeout de 0.1 segundos
    newtio.c_cc[VMIN] = 0;   // No bloquear lectura
    
    // Limpiar buffer
    tcflush(fd, TCIFLUSH);
    
    // Aplicar configuración
    if (tcsetattr(fd, TCSANOW, &newtio) < 0) {
        printf("Error: No se pudo configurar el puerto\n");
        return false;
    }
    
    return true;
#endif
}

int SerialPort::leerLinea(char* buffer, int longitudMax) {
    if (!conectado || longitudMax <= 0) return -1;
    
    int indice = 0;
    char c;
    
    while (indice < longitudMax - 1) {
        if (!leerCaracter(c)) {
            // Timeout o error
            if (indice > 0) break;  // Si ya leímos algo, terminar la línea
            continue;  // Si no, seguir esperando
        }
        
        // Terminar en nueva línea
        if (c == '\n') {
            break;
        }
        
        // Ignorar retornos de carro
        if (c == '\r') {
            continue;
        }
        
        buffer[indice++] = c;
    }
    
    buffer[indice] = '\0';
    return indice;
}

bool SerialPort::leerCaracter(char& c) {
    if (!conectado) return false;
    
#ifdef _WIN32
    DWORD bytesLeidos;
    if (!ReadFile(hSerial, &c, 1, &bytesLeidos, nullptr)) {
        return false;
    }
    return bytesLeidos == 1;
#else
    int resultado = read(fd, &c, 1);
    return resultado == 1;
#endif
}

void SerialPort::cerrar() {
    if (!conectado) return;
    
#ifdef _WIN32
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
#else
    if (fd >= 0) {
        // Restaurar configuración antigua
        tcsetattr(fd, TCSANOW, &oldtio);
        close(fd);
        fd = -1;
    }
#endif
    
    conectado = false;
}
