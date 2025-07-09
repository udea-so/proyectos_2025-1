#!/bin/bash

echo "[🎬] Iniciando demostración del proyecto con GPIOs virtuales"
echo "-------------------------------------------------------------"

cd "$(dirname "$0")"

make clean
make

echo ""
echo "[📡] Mostrando mensajes del kernel (Ctrl+C para salir)..."
echo ""
sudo dmesg -w

