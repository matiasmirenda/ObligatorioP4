#include "Novela.h"
#include <cstddef>

Novela::Novela(long int cod, string tit, string ed, double precio, string gen)
    : Libro(cod, tit, ed, precio), genero(gen)
{
    autor = NULL;
}
string Novela::getGenero() { return (genero); }
Autor *Novela::getAutor() { return (autor); }
void Novela::asignarAutor(Autor *nuevo) { autor = nuevo; }
bool Novela::datosValidos() { return (Libro::datosValidos() && !(genero == "")); }
double Novela::calcularPrecioVenta()
{
    double precio = getPrecioBase();
    if (genero == "infantil")
        precio = precio * 0.80;
    return (precio);
}
TipoLibro Novela::getTipo() { return (NOVELA); }

Novela::~Novela()
{
}
