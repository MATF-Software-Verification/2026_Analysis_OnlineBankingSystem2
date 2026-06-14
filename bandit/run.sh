#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR/../snake"

bandit -r "$PROJECT_DIR" \
    --format txt \
    --verbose \
    2>&1 | tee -a "$SCRIPT_DIR/results.txt"
