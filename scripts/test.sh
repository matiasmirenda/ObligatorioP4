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

for tipo in HashLibros ABBAutores Iterador ColeccionLibros ColeccionAutores Fachada; do
    for caso in referencia constructor asignacion; do
        case "$caso" in
            referencia) operacion="$tipo& referencia = original; (void)referencia;" ;;
            constructor) operacion="$tipo copia = original;" ;;
            asignacion) operacion="$tipo copia; copia = original;" ;;
        esac
        fuente="$directorio/copia_${tipo}_${caso}.cpp"
        cat > "$fuente" <<FUENTE
#include "$tipo.h"
int main()
{
    $tipo original;
    $operacion
    return (0);
}
FUENTE
        if "${CXX:-g++}" -std=c++11 -Wall -Wextra -pedantic -Iinclude \
            -fsyntax-only "$fuente" > "$directorio/copia_${tipo}_${caso}.log" 2>&1; then
            if [ "$caso" != referencia ]; then
                echo "FALLO: se permitio copia de $tipo por $caso" >&2
                exit 1
            fi
        else
            if [ "$caso" = referencia ]; then
                cat "$directorio/copia_${tipo}_${caso}.log" >&2
                exit 1
            fi
        fi
    done
done
echo "Referencias permitidas y 12 copias prohibidas: OK"
