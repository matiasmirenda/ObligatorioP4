#ifndef FECHA_H
#define FECHA_H

#include <chrono>
#include <optional>
#include <string>
#include <string_view>

namespace fecha {

using Fecha = std::chrono::year_month_day;

// Interpreta una fecha ISO estricta (AAAA-MM-DD). Devuelve nullopt si no es válida.
std::optional<Fecha> parsearISO(std::string_view texto);

// Comprueba que el día exista en el calendario gregoriano y que el año sea positivo.
bool esValida(const Fecha& valor) noexcept;

// Devuelve la fecha en formato AAAA-MM-DD o nullopt si el valor no es válido.
std::optional<std::string> aISO(const Fecha& valor);

} // namespace fecha

#endif // FECHA_H
