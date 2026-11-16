#include "Fecha.h"
#include <iostream>

int fallos = 0;
void verificar(bool condicion, const char* descripcion) {
    if (!condicion) {
        std::cerr << "FALLO: " << descripcion << '\n';
        ++fallos;
    }
}

int main() {
    Fecha fecha(29, 2, 2024);
    verificar(fecha.getDia() == 29 && fecha.getMes() == 2 && fecha.getAnio() == 2024,
              "consulta de componentes");
    verificar(Fecha::esValida(29, 2, 2024), "bisiesto divisible por cuatro");
    verificar(Fecha::esValida(29, 2, 2000), "bisiesto divisible por cuatrocientos");
    verificar(!Fecha::esValida(29, 2, 1900), "siglo no bisiesto");
    verificar(!Fecha::esValida(29, 2, 2023), "febrero no bisiesto");
    verificar(Fecha::esValida(30, 4, 2024), "mes de treinta dias");
    verificar(!Fecha::esValida(31, 4, 2024), "dia fuera del mes");
    verificar(Fecha::esValida(31, 12, 9999), "limite superior");
    verificar(!Fecha::esValida(1, 1, 0), "anio cero");
    verificar(!Fecha::esValida(1, 1, 10000), "anio fuera de rango");
    verificar(!Fecha::esValida(1, 0, 2024), "mes cero");
    verificar(!Fecha::esValida(1, 13, 2024), "mes fuera de rango");
    verificar(!Fecha::esValida(0, 1, 2024), "dia cero");
    verificar(Fecha(31, 12, 2023).esMenor(fecha), "compara anios");
    verificar(Fecha(31, 1, 2024).esMenor(fecha), "compara meses");
    verificar(Fecha(28, 2, 2024).esMenor(fecha), "compara dias");
    verificar(fecha.esMayor(Fecha(28, 2, 2024)), "fecha mayor");
    verificar(!fecha.esMayor(fecha) && !fecha.esMenor(fecha), "igualdad no es mayor ni menor");
    verificar(fecha.esIgual(Fecha(29, 2, 2024)), "fechas iguales");
    verificar(!fecha.esIgual(Fecha(28, 2, 2024)), "fechas diferentes");
    if (fallos != 0) return 1;
    std::cout << "Todas las comprobaciones pasaron\n";
    return 0;
}
