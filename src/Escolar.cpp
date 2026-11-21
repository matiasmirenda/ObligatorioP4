#include "Escolar.h"
Escolar::Escolar(long int cod, string tit, string ed, double precio,
                 string mat, Fecha fecha, int anio)
    : Texto(cod, tit, ed, precio, mat, fecha)
{
    anioEscuela = anio;
}

int Escolar::getAnioEscuela() { return (anioEscuela); }
bool Escolar::datosValidos()
{
    return (Texto::datosValidos() && anioEscuela >= 1 && anioEscuela <= 6);
}
double Escolar::calcularPrecioVenta()
{
    double precio = Texto::calcularPrecioVenta();
    if (anioEscuela >= 1 && anioEscuela <= 3)
        precio = precio * 0.90;
    return (precio);
}
TipoLibro Escolar::getTipo() { return (ESCOLAR); }

Escolar::~Escolar()
{
}
