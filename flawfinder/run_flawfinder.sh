#!/bin/bash

cd "$(dirname "$0")"

if ! command -v flawfinder &> /dev/null; then
    echo "Flawfinder nije instaliran. Instalirajte ga: sudo apt install flawfinder"
    exit 1
fi

SOURCE_DIR="../OnlineBankingSystem"
OUTPUT_FILE="flawfinder_report.txt"
OUTPUT_HTML="flawfinder_report.html"

echo "=== Započinjem Flawfinder analizu ==="

flawfinder --minlevel=1 --columns "$SOURCE_DIR" > "$OUTPUT_FILE"

flawfinder --html --minlevel=1 "$SOURCE_DIR" > "$OUTPUT_HTML"

echo "-------------------------------------------------------"
echo "Analiza završena!"
echo "Tekstualni izveštaj: flawfinder/$OUTPUT_FILE"
echo "HTML izveštaj: flawfinder/$OUTPUT_HTML"
