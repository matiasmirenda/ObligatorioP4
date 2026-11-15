#include "Fecha.h"

#include <array>
#include <charconv>
#include <system_error>

namespace fecha {
namespace {

bool leerNumero(std::string_view texto, int& valor) {
    const auto resultado = std::from_chars(texto.data(), texto.data() + texto.size(), valor);
    return resultado.ec == std::errc{} && resultado.ptr == texto.data() + texto.size();
}

void escribirDosDigitos(unsigned valor, char* destino) {
    destino[0] = static_cast<char>('0' + (valor / 10U));
    destino[1] = static_cast<char>('0' + (valor % 10U));
}

} // namespace

bool esValida(const Fecha& valor) noexcept {
    return valor.ok() && int(valor.year()) >= 1;
}

std::optional<Fecha> parsearISO(std::string_view texto) {
    if (texto.size() != 10 || texto[4] != '-' || texto[7] != '-') {
        return std::nullopt;
    }

    int anio = 0;
    int mes = 0;
    int dia = 0;
    if (!leerNumero(texto.substr(0, 4), anio) ||
        !leerNumero(texto.substr(5, 2), mes) ||
        !leerNumero(texto.substr(8, 2), dia)) {
        return std::nullopt;
    }

    const Fecha resultado{std::chrono::year{anio}, std::chrono::month{static_cast<unsigned>(mes)},
                          std::chrono::day{static_cast<unsigned>(dia)}};
    if (!esValida(resultado)) {
        return std::nullopt;
    }
    return resultado;
}

std::optional<std::string> aISO(const Fecha& valor) {
    if (!esValida(valor)) {
        return std::nullopt;
    }

    const unsigned anio = static_cast<unsigned>(int(valor.year()));
    const unsigned mes = static_cast<unsigned>(valor.month());
    const unsigned dia = static_cast<unsigned>(valor.day());
    std::array<char, 10> texto{};
    texto[0] = static_cast<char>('0' + (anio / 1000U) % 10U);
    texto[1] = static_cast<char>('0' + (anio / 100U) % 10U);
    texto[2] = static_cast<char>('0' + (anio / 10U) % 10U);
    texto[3] = static_cast<char>('0' + anio % 10U);
    texto[4] = '-';
    escribirDosDigitos(mes, texto.data() + 5);
    texto[7] = '-';
    escribirDosDigitos(dia, texto.data() + 8);
    return std::string(texto.data(), texto.size());
}

} // namespace fecha
