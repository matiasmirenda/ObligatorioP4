#include "Escolar.h"
Escolar::Escolar(long long cod, String tit, String ed, double precio,
                 String mat, Fecha fecha, int anio)
    : Texto(cod, tit, ed, precio, mat, fecha)
{
    anioEscuela = anio;
}
Escolar::Escolar(const Escolar& otro) : Texto(otro)
{
    anioEscuela = otro.anioEscuela;
}
int Escolar::getAnioEscuela() { return anioEscuela; }
void Escolar::setAnioEscuela(int nuevo) { anioEscuela = nuevo; }
bool Escolar::datosValidos()
{
    return Texto::datosValidos() && anioEscuela >= 1 && anioEscuela <= 6;
}
double Escolar::calcularPrecioVenta()
{
    double precio = Texto::calcularPrecioVenta();
    if (anioEscuela >= 1 && anioEscuela <= 3)
        precio = precio * 0.90;
    return precio;
}
String Escolar::getTipo() { return "Escolar"; }

Escolar::~Escolar()
{
}
