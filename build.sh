#!/bin/bash

###############################################################################
# Script de compilación para PRT-7 Decoder (Linux/macOS)
###############################################################################

echo "======================================================"
echo "  Compilando PRT-7 Decoder"
echo "======================================================"
echo ""

# Colores para output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Verificar que CMake está instalado
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}Error: CMake no está instalado${NC}"
    echo "Por favor instale CMake:"
    echo "  Ubuntu/Debian: sudo apt-get install cmake"
    echo "  macOS: brew install cmake"
    exit 1
fi

# Crear directorio build si no existe
if [ ! -d "build" ]; then
    echo "Creando directorio build..."
    mkdir build
fi

# Entrar al directorio build
cd build

# Ejecutar CMake
echo "Configurando proyecto con CMake..."
cmake .. || {
    echo -e "${RED}Error en la configuración de CMake${NC}"
    exit 1
}

# Compilar
echo ""
echo "Compilando..."
cmake --build . || {
    echo -e "${RED}Error en la compilación${NC}"
    exit 1
}

echo ""
echo -e "${GREEN}======================================================"
echo "  Compilación exitosa!"
echo "======================================================${NC}"
echo ""
echo "El ejecutable se encuentra en: build/prt7_decoder"
echo ""
echo "Para ejecutar:"
echo "  cd build"
echo "  ./prt7_decoder"
echo ""
