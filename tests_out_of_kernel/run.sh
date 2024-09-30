#!/bin/bash

# Function to clean up
cleanup() {
    echo -e "\nLimpiando archivos generados..."
    make clean
    echo "Proceso completado."
}

trap cleanup SIGINT

chmod +x run.sh
make all
echo "Compilación exitosa."
./test_mm

