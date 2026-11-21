#include "Texto.h"

Texto::Texto(long int cod, string tit, string ed, double precio, string mat, Fecha fecha)
    : Libro(cod, tit, ed, precio), tipo("Texto"), materia(mat), fechaPublicado(fecha) {}
string Texto::getMateria() { return (materia); }
Fecha Texto::getFechaPublicado() { return (fechaPublicado); }
bool Texto::datosValidos()
{
    return (Libro::datosValidos() && !(materia == "") && fechaPublicado.esValida());
}
double Texto::calcularPrecioVenta()
{
    double precio = getPrecioBase();
    Fecha corte(1, 1, 2026);
    if (fechaPublicado < corte)
        precio = precio * 0.75;
    return (precio);
}
string Texto::getTipo() { return (tipo); }

Texto::~Texto()
{
}
