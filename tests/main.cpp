#include "Fecha.h"

#include <iostream>
#include <string_view>

namespace {

int fallos = 0;

void verificar(bool condicion, std::string_view descripcion) {
    if (!condicion) {
        std::cerr << "FALLO: " << descripcion << '\n';
        ++fallos;
    }
}

void probarParseoISO() {
    const auto fecha = fecha::parsearISO("2024-02-29");
    verificar(fecha.has_value(), "acepta un día bisiesto válido");
    verificar(fecha && int(fecha->year()) == 2024 && unsigned(fecha->month()) == 2 &&
                  unsigned(fecha->day()) == 29,
              "interpreta año, mes y día");
}

void probarFechasInvalidas() {
    verificar(!fecha::parsearISO("2023-02-29"), "rechaza un día bisiesto inexistente");
    verificar(!fecha::parsearISO("2024-13-01"), "rechaza un mes fuera de rango");
    verificar(!fecha::parsearISO("2024-04-31"), "rechaza un día fuera del mes");
    verificar(!fecha::parsearISO("0000-01-01"), "rechaza el año cero");
}

void probarFormatoEstricto() {
    verificar(!fecha::parsearISO(""), "rechaza texto vacío");
    verificar(!fecha::parsearISO("2024/02/29"), "rechaza separadores distintos");
    verificar(!fecha::parsearISO("2024-2-09"), "rechaza componentes sin ancho fijo");
    verificar(!fecha::parsearISO("abcd-02-09"), "rechaza componentes no numéricos");
}

void probarFormatoISO() {
    const auto valor = fecha::parsearISO("2001-09-08");
    verificar(valor.has_value(), "prepara fecha para formato");
    if (valor) {
        const auto texto = fecha::aISO(*valor);
        verificar(texto && *texto == "2001-09-08", "formatea con ceros a la izquierda");
    }

    const fecha::Fecha invalida{std::chrono::year{2024}, std::chrono::month{2},
                                std::chrono::day{30}};
    verificar(!fecha::aISO(invalida), "no formatea una fecha inválida");
}

} // namespace

int main() {
    probarParseoISO();
    probarFechasInvalidas();
    probarFormatoEstricto();
    probarFormatoISO();

    if (fallos != 0) {
        std::cerr << fallos << " comprobación(es) fallaron\n";
        return 1;
    }
    std::cout << "Todas las comprobaciones pasaron\n";
    return 0;
}
