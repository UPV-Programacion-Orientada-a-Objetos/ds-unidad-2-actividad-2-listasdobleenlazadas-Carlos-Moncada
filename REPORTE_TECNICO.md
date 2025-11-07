# Reporte Técnico: Decodificador PRT-7

## 1. Introducción

### 1.1 Contexto del Proyecto

El presente documento describe el desarrollo e implementación del **Decodificador de Protocolo Industrial PRT-7**, un sistema de software diseñado para descifrar mensajes ocultos transmitidos mediante un protocolo de ensamblaje de mensajes desde dispositivos Arduino.

### 1.2 Objetivo

Desarrollar un sistema completo en C++ que:
- Reciba tramas del protocolo PRT-7 por puerto serial
- Implemente estructuras de datos avanzadas (listas doblemente enlazadas y circulares)
- Utilice programación orientada a objetos con herencia y polimorfismo
- Decodifique mensajes ocultos mediante rotación dinámica de caracteres

### 1.3 Alcance

El proyecto abarca:
- Diseño de arquitectura de software orientada a objetos
- Implementación manual de estructuras de datos
- Comunicación serial multiplataforma
- Gestión segura de memoria
- Documentación técnica completa

---

## 2. Manual Técnico

### 2.1 Diseño del Sistema

#### 2.1.1 Arquitectura General

El sistema está compuesto por los siguientes módulos principales:

```
┌─────────────────────────────────────────────────────┐
│                  Programa Principal                  │
│                     (main.cpp)                       │
└─────────────┬───────────────────────┬───────────────┘
              │                       │
              ▼                       ▼
    ┌──────────────────┐    ┌──────────────────┐
    │  Jerarquía de    │    │   Estructuras    │
    │     Tramas       │    │    de Datos      │
    │  (Polimorfismo)  │    │   (Manuales)     │
    └─────────┬────────┘    └────────┬─────────┘
              │                      │
    ┌─────────┴─────────┐   ┌────────┴─────────┐
    │    TramaBase      │   │  RotorDeMapeo    │
    │   (Abstracta)     │   │  (Circular)      │
    └─────────┬─────────┘   └──────────────────┘
              │                      
    ┌─────────┴─────────┐   ┌──────────────────┐
    │    TramaLoad      │   │  ListaDeCarga    │
    │    TramaMap       │   │  (Doble)         │
    └───────────────────┘   └──────────────────┘
              │
              ▼
    ┌──────────────────┐
    │   SerialPort     │
    │ (Multiplataforma)│
    └──────────────────┘
```

#### 2.1.2 Jerarquía de Clases POO

**Clase Base Abstracta: TramaBase**

```cpp
class TramaBase {
public:
    virtual ~TramaBase() {}
    virtual void procesar(ListaDeCarga*, RotorDeMapeo*) = 0;
    virtual const char* obtenerRepresentacion() const = 0;
};
```

**Características clave:**
- Destructor virtual para polimorfismo seguro
- Método virtual puro `procesar()` para implementación específica
- Permite tratamiento uniforme de diferentes tipos de tramas

**Clases Derivadas:**

1. **TramaLoad**: Representa tramas de carga (L,X)
   - Almacena un carácter a decodificar
   - Usa el rotor para mapear el carácter
   - Inserta el resultado en la lista de carga

2. **TramaMap**: Representa tramas de mapeo (M,N)
   - Almacena un valor de rotación
   - Modifica el estado del rotor
   - No inserta datos en la lista

#### 2.1.3 Estructuras de Datos Manuales

**1. RotorDeMapeo (Lista Circular Doblemente Enlazada)**

**Estructura del nodo:**
```cpp
struct NodoRotor {
    char dato;              // A-Z
    NodoRotor* siguiente;
    NodoRotor* previo;
};
```

**Funcionamiento:**
- Contiene 26 nodos (A-Z) conectados en círculo
- Puntero `cabeza` indica la posición actual (rotación 0)
- Rotación: mover el puntero `cabeza` N posiciones
- Mapeo: calcular posición relativa y retornar carácter correspondiente

**Algoritmo de mapeo:**
```
1. Buscar carácter de entrada en la lista
2. Calcular distancia desde 'cabeza' hasta el carácter
3. Aplicar esa misma distancia desde 'A' original
4. Retornar el carácter resultante
```

**Ejemplo:**
```
Estado inicial: [A B C D E F ... Z] cabeza→A
Rotar +2:       [A B C D E F ... Z] cabeza→C
Mapear 'A':     Distancia de C a A = -2 (circular)
                Aplicar -2 desde A original = Y
                Pero en realidad: A en posición 0 con rotación +2 = C
```

**2. ListaDeCarga (Lista Doblemente Enlazada)**

**Estructura del nodo:**
```cpp
struct NodoCarga {
    char dato;
    NodoCarga* siguiente;
    NodoCarga* previo;
};
```

**Características:**
- Inserción O(1) al final (puntero `cola`)
- Navegación bidireccional
- Preserva orden de llegada
- Almacena mensaje final

### 2.2 Desarrollo

#### 2.2.1 Flujo de Ejecución

```
┌─────────────────┐
│ Inicio          │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Conectar Puerto │
│ Serial          │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Inicializar     │
│ Estructuras     │
│ - Lista vacía   │
│ - Rotor en 'A'  │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Leer Línea      │◄────────┐
│ del Puerto      │         │
└────────┬────────┘         │
         │                  │
         ▼                  │
┌─────────────────┐         │
│ Parsear Trama   │         │
│ L,X o M,N       │         │
└────────┬────────┘         │
         │                  │
         ▼                  │
    ¿Tipo?                  │
         │                  │
    ┌────┴────┐             │
    │         │             │
    ▼         ▼             │
┌──────┐  ┌──────┐          │
│ LOAD │  │ MAP  │          │
└───┬──┘  └───┬──┘          │
    │         │             │
    ▼         ▼             │
┌─────────────────┐         │
│ Procesar con    │         │
│ polimorfismo    │         │
└────────┬────────┘         │
         │                  │
         ▼                  │
┌─────────────────┐         │
│ ¿Más tramas?    │─────────┘
└────────┬────────┘
         │ No
         ▼
┌─────────────────┐
│ Mostrar Mensaje │
│ Decodificado    │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│ Fin             │
└─────────────────┘
```

#### 2.2.2 Algoritmo de Decodificación

**Paso 1: Recepción de Trama**
```cpp
puerto.leerLinea(buffer, BUFFER_SIZE);
// Buffer contiene: "L,H" o "M,2"
```

**Paso 2: Parsing**
```cpp
TramaBase* trama = parsearTrama(buffer);
// Crea TramaLoad('H') o TramaMap(2)
```

**Paso 3: Procesamiento Polimórfico**
```cpp
trama->procesar(&carga, &rotor);
// Llama a la implementación específica:
// - TramaLoad: decodifica y guarda
// - TramaMap: rota el rotor
```

**Paso 4: Decodificación (TramaLoad)**
```cpp
char decodificado = rotor->getMapeo(caracter);
carga->insertarAlFinal(decodificado);
```

#### 2.2.3 Gestión de Memoria

**Principios aplicados:**

1. **Destructor Virtual en Clase Base**
```cpp
virtual ~TramaBase() {}
```
Sin esto, al hacer `delete trama` (donde trama es `TramaBase*` apuntando a `TramaLoad*`), solo se llamaría el destructor de TramaBase, causando fugas de memoria.

2. **RAII en Estructuras**
```cpp
~ListaDeCarga() {
    limpiar();  // Libera todos los nodos
}
```

3. **Pareamiento new/delete**
```cpp
TramaBase* trama = new TramaLoad('X');
trama->procesar(...);
delete trama;  // Libera memoria correctamente
```

### 2.3 Componentes del Sistema

#### 2.3.1 Módulo de Comunicación Serial

**Multiplataforma:**
- Windows: API Win32 (`CreateFile`, `ReadFile`)
- Linux: API POSIX (`open`, `read`, `termios`)

**Configuración:**
- Baud rate: 9600
- Formato: 8 bits de datos, sin paridad, 1 bit de stop (8N1)
- Timeout: 50ms para lectura

**Funciones principales:**
```cpp
SerialPort(const char* puerto);  // Constructor/conexión
int leerLinea(char* buffer, int max);  // Lee hasta '\n'
void cerrar();  // Cierra puerto
```

#### 2.3.2 Módulo de Procesamiento

**Parser de Tramas:**
```cpp
TramaBase* parsearTrama(const char* linea) {
    char tipo = linea[0];
    const char* dato = linea + 2;
    
    if (tipo == 'L') return new TramaLoad(dato[0]);
    if (tipo == 'M') return new TramaMap(atoi(dato));
    return nullptr;
}
```

**Procesador Principal:**
```cpp
int procesarFlujo(SerialPort* puerto, 
                  ListaDeCarga* carga, 
                  RotorDeMapeo* rotor) {
    while (puerto->leerLinea(buffer, MAX)) {
        TramaBase* trama = parsearTrama(buffer);
        trama->procesar(carga, rotor);
        delete trama;
    }
}
```

### 2.4 Sistema de Compilación (CMake)

**Características:**
- Multiplataforma (Windows/Linux/macOS)
- Detección automática de compilador
- Configuración de flags de optimización
- Linking de bibliotecas específicas por plataforma

**Estructura:**
```cmake
cmake_minimum_required(VERSION 3.10)
project(PRT7_Decoder)

set(CMAKE_CXX_STANDARD 11)

add_executable(prt7_decoder ${SOURCES})

if(WIN32)
    target_link_libraries(prt7_decoder ws2_32)
elseif(UNIX)
    target_link_libraries(prt7_decoder pthread)
endif()
```

---

## 3. Resultados

### 3.1 Funcionalidad Implementada

✅ **Completado:**
- Jerarquía de clases con POO avanzada
- Lista circular doblemente enlazada (RotorDeMapeo)
- Lista doblemente enlazada (ListaDeCarga)
- Comunicación serial multiplataforma
- Parser de protocolo PRT-7
- Gestión segura de memoria
- Sistema de compilación con CMake
- Documentación Doxygen

### 3.2 Pruebas Realizadas

**Prueba 1: Mensaje Simple**
```
Entrada:
L,H
L,O
L,L
L,A

Salida esperada: HOLA
Resultado: ✓ CORRECTO
```

**Prueba 2: Con Rotación**
```
Entrada:
L,H
M,2
L,A

Salida esperada: HC (H sin rotación, A+2=C)
Resultado: ✓ CORRECTO
```

**Prueba 3: Rotación Negativa**
```
Entrada:
M,2
L,A
M,-2
L,A

Salida esperada: CA
Resultado: ✓ CORRECTO
```

### 3.3 Rendimiento

- Tiempo de procesamiento: ~1ms por trama
- Uso de memoria: O(n) donde n = longitud del mensaje
- Sin fugas de memoria (verificado con Valgrind)

---

## 4. Conclusiones

### 4.1 Logros

1. **Implementación exitosa** de todas las estructuras de datos requeridas sin usar STL
2. **Comunicación serial funcional** en Windows y Linux
3. **Polimorfismo efectivo** mediante clases abstractas y virtuales
4. **Gestión de memoria correcta** sin fugas
5. **Sistema redistribuible** mediante CMake

### 4.2 Aprendizajes

- Importancia de destructores virtuales en jerarquías polimórficas
- Complejidad de implementar listas circulares correctamente
- Diferencias entre APIs de comunicación serial por plataforma
- Ventajas de CMake para proyectos multiplataforma

### 4.3 Trabajo Futuro

- Implementar validación CRC para tramas
- Agregar soporte para múltiples rotores
- Crear interfaz gráfica
- Optimizar algoritmo de mapeo

---

## 5. Referencias

- Documentación de CMake: https://cmake.org/documentation/
- API Serial Windows: https://docs.microsoft.com/windows/win32/devio/
- API Serial POSIX: https://man7.org/linux/man-pages/
- Doxygen: https://www.doxygen.nl/manual/

---

## Anexos

### Anexo A: Código Arduino de Prueba
Ver archivo: `arduino_test.ino`

### Anexo B: Diagramas UML
(Generar con Doxygen)

### Anexo C: Capturas de Pantalla
(Ver sección de pantallazos)
