#include <iostream>
#include "ValidacionDatos.h"
#include "Parametros.h"
#include "Texto.h"

using namespace std;

Texto::Texto(long int cod, string tit, string ed, float precio, string mat, Fecha fecha)
    : Libro(cod, tit, ed, precio), materia(mat), fechaPublicado(fecha) {}
string Texto::getMateria() { return (materia); }
Fecha Texto::getFechaPublicado() { return (fechaPublicado); }
bool Texto::datosValidos()
{
    return (Libro::datosValidos() && textoValido(materia) && fechaPublicado.esValida());
}
float Texto::calcularPrecioVenta()
{
    float precio = getPrecioBase();
    Fecha corte(DIA_CORTE_DESCUENTO, MES_CORTE_DESCUENTO, ANIO_CORTE_DESCUENTO);
    if (fechaPublicado < corte)
        precio = precio * FACTOR_PRECIO_TEXTO_ANTIGUO;
    return (precio);
}
TipoLibro Texto::getTipo() { return (TEXTO); }

Texto::~Texto()
{
}

void Texto::imprimir()
{
    Libro::imprimir();
    cout << "Materia: " << materia << "\nPublicacion: ";
    fechaPublicado.imprimir();
    cout << '\n';
}
