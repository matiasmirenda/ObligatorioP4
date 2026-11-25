#include <iostream>
#include "Parametros.h"
#include "Escolar.h"

using namespace std;
Escolar::Escolar(long int cod, string tit, string ed, float precio,
                 string mat, Fecha fecha, int anio)
    : Texto(cod, tit, ed, precio, mat, fecha)
{
    anioEscuela = anio;
}

int Escolar::getAnioEscuela() { return (anioEscuela); }
bool Escolar::datosValidos()
{
    return (Texto::datosValidos() && anioEscuela >= ANIO_ESCOLAR_MINIMO && anioEscuela <= ANIO_ESCOLAR_MAXIMO);
}
float Escolar::calcularPrecioVenta()
{
    float precio = Texto::calcularPrecioVenta();
    if (anioEscuela >= ANIO_ESCOLAR_MINIMO && anioEscuela <= ULTIMO_ANIO_DESCUENTO_ESCOLAR)
        precio = precio * FACTOR_PRECIO_ESCOLAR_INICIAL;
    return (precio);
}
TipoLibro Escolar::getTipo() { return (ESCOLAR); }

Escolar::~Escolar()
{
}

void Escolar::imprimir()
{
    Texto::imprimir();
    cout << "Anio escolar: " << anioEscuela << '\n';
}
