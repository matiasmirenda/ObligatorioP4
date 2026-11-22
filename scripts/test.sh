#!/bin/sh
set -eu

cd "$(dirname "$0")/.."

case "${1:-}" in
    "")
        directorio=build
        set --
        ;;
    --coverage)
        directorio=build-coverage
        set -- --coverage -O0 -g
        ;;
    *)
        echo "Uso: sh scripts/test.sh [--coverage]" >&2
        exit 2
        ;;
esac

mkdir -p "$directorio"
# Evita mezclar mediciones de una compilacion anterior.
if [ "$directorio" = build-coverage ]; then
    find "$directorio" -maxdepth 1 \( -name 'p4_tests-*.gcda' -o -name 'p4_tests-*.gcno' \) -delete
fi
for fuente in src/*.cpp; do
    case "$fuente" in
        src/main.cpp) ;;
        *) set -- "$@" "$fuente" ;;
    esac
done

"${CXX:-g++}" -std=c++11 -Wall -Wextra -pedantic -Iinclude \
    "$@" tests/main.cpp -o "$directorio/p4_tests"
"./$directorio/p4_tests"
