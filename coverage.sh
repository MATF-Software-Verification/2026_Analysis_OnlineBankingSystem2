#!/bin/bash
pytest pytest-unit/ pytest-integration/ --cov=snake --cov-report=term-missing > coverage_report.txt 2>&1
