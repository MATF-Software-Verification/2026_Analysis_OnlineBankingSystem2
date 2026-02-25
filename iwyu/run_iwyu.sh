#!/bin/bash

cd "$(dirname "$0")"

SOURCE_DIR="../OnlineBankingSystem"
OUTPUT_FILE="iwyu_output.txt"

> "$OUTPUT_FILE"

echo "=== Započinjem Include-What-You-Use analizu ===" | tee -a "$OUTPUT_FILE"

for file in "$SOURCE_DIR"/*.cpp; do
    if [ -f "$file" ]; then
        echo "---------------------------------------------------" | tee -a "$OUTPUT_FILE"
        echo "Analiziram zavisnosti za: $file" | tee -a "$OUTPUT_FILE"
        
        include-what-you-use "$file" \
        -std=c++17 -I"$SOURCE_DIR" \
        2>&1 | tee -a "$OUTPUT_FILE"
        
        echo -e "\n" >> "$OUTPUT_FILE"
    fi
done

echo "=== Analiza završena. Izlaz: $OUTPUT_FILE ==="
