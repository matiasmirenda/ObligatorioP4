# Obligatorio P4 UDE

Los encabezados estan en include/ y las implementaciones en src/.
Las entidades del dominio siguen vacias. Fecha contiene dia, mes y anio
como enteros, validacion y comparaciones de menor, mayor e igual.
Antes de construir una Fecha se debe comprobar Fecha::esValida(dia, mes, anio).

Para compilar y ejecutar las pruebas desde la terminal de Visual Studio Code:

```sh
mkdir -p build
g++ -std=c++11 -Wall -Wextra -pedantic -Iinclude src/Fecha.cpp tests/main.cpp -o build/p4_tests
./build/p4_tests
```

GitHub Actions compila, ejecuta las pruebas y exige un minimo de 90% de
cobertura de lineas en src/.
