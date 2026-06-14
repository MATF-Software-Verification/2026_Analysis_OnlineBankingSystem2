#!/bin/bash
pytest pytest-unit/ --cov=snake --cov-report=term-missing > pytest-unit/output.txt 2>&1
