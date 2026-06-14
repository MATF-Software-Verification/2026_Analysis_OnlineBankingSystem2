#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR/../snake"

pylint "$PROJECT_DIR"/*.py \
    --output-format=text \
    2>&1 | tee -a "$SCRIPT_DIR/results2.txt"
