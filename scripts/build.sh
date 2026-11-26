#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
mkdir -p build
"${CXX:-g++}" -std=c++11 -Wall -Wextra -pedantic -Iinclude src/*.cpp -o build/libreria
