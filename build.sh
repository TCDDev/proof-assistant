#!/usr/bin/env bash

set -euo pipefail

if [[ -z "${VIRTUAL_ENV:-}" ]]; then
    echo "error: activate the project virtual environment first"
    echo "       source .venv/bin/activate"
    exit 1
fi

PYBIND11_DIR="$(python -m pybind11 --cmakedir)"
PYTHON_EXECUTABLE="$(which python)"

cmake \
    --preset clang-debug \
    -DPython_EXECUTABLE="${PYTHON_EXECUTABLE}" \
    -Dpybind11_DIR="${PYBIND11_DIR}"

cmake --build --preset clang-debug

cmake \
    --preset gcc-debug \
    -DPython_EXECUTABLE="${PYTHON_EXECUTABLE}" \
    -Dpybind11_DIR="${PYBIND11_DIR}"

cmake --build --preset gcc-debug