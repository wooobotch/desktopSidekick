#!/usr/bin/env bash

set -e

mkdir -p build
cd build

cmake -DCMAKE_BUILD_TYPE=Debug ..

make

echo "Ejecutando mascota..."
./mascota_sdl

