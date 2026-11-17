#include "Novela.h"
#include <cstddef>
Novela::Novela(long long cod, String tit, String ed, double precio, String gen)
    : Libro(cod, tit, ed, precio), genero(gen)
{
    autor = NULL;
}
Novela::Novela(const Novela& otra) : Libro(otra), genero(otra.genero)
{
    autor = otra.autor;
}
String Novela::getGenero() { return genero; }
Autor* Novela::getAutor() { return autor; }
void Novela::setGenero(String nuevo) { genero = nuevo; }
void Novela::asignarAutor(Autor* nuevo) { autor = nuevo; }
bool Novela::datosValidos() { return Libro::datosValidos() && !(genero == ""); }
double Novela::calcularPrecioVenta()
{
    if (genero == "infantil")
        return getPrecioBase() * 0.80;
    return getPrecioBase();
}
String Novela::getTipo() { return "Novela"; }

Novela::~Novela()
{
}
