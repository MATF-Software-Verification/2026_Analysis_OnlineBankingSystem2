#!/bin/bash

cd "$(dirname "$0")"

if ! command -v cppcheck &> /dev/null; then
    echo "Cppcheck nije instaliran. Instalirajte ga: sudo apt install cppcheck"
    exit 1
fi

PROJECT_PATH="../OnlineBankingSystem"
OUTPUT_XML="cppcheck_output.xml"
REPORT_DIR="cppcheck_report"

rm -rf "$OUTPUT_XML" "$REPORT_DIR"

echo "=== 1. Pokrećem Cppcheck analizu ==="
cppcheck --enable=all --inconclusive --xml --xml-version=2 \
         --suppress=missingInclude \
         -I "$PROJECT_PATH" \
         "$PROJECT_PATH" 2> "$OUTPUT_XML"

if [ ! -s "$OUTPUT_XML" ]; then
    echo "Greška: Cppcheck nije generisao podatke."
    exit 1
fi

echo "=== 2. Generišem HTML izveštaj ==="
if command -v cppcheck-htmlreport &> /dev/null; then
    mkdir -p "$REPORT_DIR"
    cppcheck-htmlreport --file="$OUTPUT_XML" --report-dir="$REPORT_DIR" --source-dir="$PROJECT_PATH" --title="Online Banking System - Cppcheck"
    echo "-------------------------------------------------------"
    echo "Gotovo! Izveštaj se nalazi u: cppcheck/$REPORT_DIR/index.html"
else
    echo "Savet: Instalirajte 'cppcheck' paket do kraja da dobijete HTML izveštaje."
    echo "XML izveštaj je sačuvan u: $OUTPUT_XML"
fi
