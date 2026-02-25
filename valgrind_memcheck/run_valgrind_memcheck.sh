#!/bin/bash

cd "$(dirname "$0")"

SRC_DIR="../OnlineBankingSystem"
TEMP_EXE="./banking_system_temp"
OUTPUT_FILE="valgrind_report.txt"

echo "=== 1. Kompajliranje programa za Valgrind ==="
g++ -std=c++17 -g "$SRC_DIR"/main.cpp -I"$SRC_DIR" -o "$TEMP_EXE"

if [ $? -ne 0 ]; then
    echo "Greška: Kompajliranje nije uspelo!"
    exit 1
fi

echo "=== 2. Pokretanje Valgrind analize ==="
echo "Napomena: Program će se pokrenuti. Molimo vas da ga završite (exit) da bi Valgrind generisao izveštaj."

valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --log-file="$OUTPUT_FILE" \
         "$TEMP_EXE"

echo "-------------------------------------------------------"
echo "Analiza završena. Rezultati su u fajlu: valgrind_memcheck/$OUTPUT_FILE"

rm -f "$TEMP_EXE"
