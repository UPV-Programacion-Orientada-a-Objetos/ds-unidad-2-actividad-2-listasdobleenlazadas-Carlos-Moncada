cmake_minimum_required(VERSION 3.10)
project(PRT7_Decoder VERSION 1.0 LANGUAGES CXX)

# Configuración del estándar C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir directorios de headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/TramaBase.cpp
    src/TramaLoad.cpp
    src/TramaMap.cpp
    src/RotorDeMapeo.cpp
    src/ListaDeCarga.cpp
    src/SerialPort.cpp
)

# Archivos de encabezado
set(HEADERS
    include/TramaBase.h
    include/TramaLoad.h
    include/TramaMap.h
    include/RotorDeMapeo.h
    include/ListaDeCarga.h
    include/SerialPort.h
)

# Crear el ejecutable
add_executable(prt7_decoder ${SOURCES} ${HEADERS})

# Configuración específica por plataforma
if(WIN32)
    # Windows necesita la biblioteca ws2_32 para comunicación serial
    target_link_libraries(prt7_decoder ws2_32)
elseif(UNIX)
    # Linux/Unix necesita pthread
    target_link_libraries(prt7_decoder pthread)
endif()

# Opciones de compilación
if(MSVC)
    target_compile_options(prt7_decoder PRIVATE /W4)
else()
    target_compile_options(prt7_decoder PRIVATE -Wall -Wextra -pedantic)
endif()

# Instalación
install(TARGETS prt7_decoder DESTINATION bin)

# Mensaje de información
message(STATUS "Configurando PRT-7 Decoder v${PROJECT_VERSION}")
message(STATUS "Sistema operativo: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compilador: ${CMAKE_CXX_COMPILER_ID}")
