# Obligatorio P4 UDE

Scaffolding inicial para el obligatorio en C++20. Los archivos de las entidades
del dominio están reservados y siguen vacíos hasta que se definan sus
responsabilidades.

## Estructura

- `include/`: declaraciones (`.h`).
- `src/`: implementaciones (`.cpp`).
- `tests/`: ejecutable de comprobaciones con su propio `main`.
- `.github/workflows/ci.yml`: compilación, pruebas y control de cobertura en PR.

## Compilar y ejecutar las pruebas

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

El módulo `fecha` usa `std::chrono::year_month_day` y ofrece parseo y formato
ISO (`AAAA-MM-DD`). Para medir cobertura local con GCC:

```sh
python -m pip install gcovr
cmake -S . -B build-coverage -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
cmake --build build-coverage
ctest --test-dir build-coverage --output-on-failure
gcovr --root . --filter 'src/' --fail-under-line 90 build-coverage
```

La CI exige al menos 90% de cobertura de líneas sobre los `.cpp` de `src/`.
Los módulos todavía vacíos no aportan líneas ejecutables; las implementaciones
nuevas quedan incluidas automáticamente en el control.
