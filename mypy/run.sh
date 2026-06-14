#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR/../snake"

mypy "$PROJECT_DIR"/*.py \
    --ignore-missing-imports \
    --show-error-codes \
    --pretty \
    2>&1 | tee -a "$SCRIPT_DIR/results.txt"

