#!/bin/bash
pytest pytest-integration/ --cov=snake --cov-report=term-missing > pytest-integration/output.txt 2>&1
