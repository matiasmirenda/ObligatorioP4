#include "Texto.h"
Texto::Texto(long long cod, String tit, String ed, double precio, String mat, Fecha fecha)
    : Libro(cod, tit, ed, precio), materia(mat), fechaPublicado(fecha) {}
Texto::Texto(const Texto& otro)
    : Libro(otro), materia(otro.materia), fechaPublicado(otro.fechaPublicado) {}
String Texto::getMateria() { return materia; }
Fecha Texto::getFechaPublicado() { return fechaPublicado; }
void Texto::setMateria(String nueva) { materia = nueva; }
void Texto::setFechaPublicado(Fecha nueva) { fechaPublicado = nueva; }
bool Texto::datosValidos()
{
    return Libro::datosValidos() && !(materia == "") && fechaPublicado.esValida();
}
double Texto::calcularPrecioVenta()
{
    Fecha corte(1, 1, 2026);
    if (fechaPublicado < corte)
        return getPrecioBase() * 0.75;
    return getPrecioBase();
}
String Texto::getTipo() { return "Texto"; }

Texto::~Texto()
{
}
