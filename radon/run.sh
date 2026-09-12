#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR/../snake"

{
    echo "=== radon cc (ciklicka kompleksnost, po funkciji/metodi) ==="
    radon cc "$PROJECT_DIR"/*.py -s -a

    echo
    echo "=== radon mi (indeks odrzivosti, po fajlu) ==="
    radon mi "$PROJECT_DIR"/*.py -s

    echo
    echo "=== radon raw (sirove metrike: LOC, SLOC, komentari) ==="
    radon raw "$PROJECT_DIR"/*.py
} 2>&1 | tee "$SCRIPT_DIR/results.txt"
