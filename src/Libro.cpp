#include "Libro.h"
#include <cfloat>

Libro::Libro(long int cod, string tit, string ed, double precio)
    : titulo(tit), editorial(ed)
{
    isbn = cod;
    precioBase = precio;
    unidadesVendidas = 0;
}

Libro::~Libro() {}
long int Libro::getId() { return (isbn); }
string Libro::getTitulo() { return (titulo); }
string Libro::getEditorial() { return (editorial); }
double Libro::getPrecioBase() { return (precioBase); }
int Libro::getUnidadesVendidas() { return (unidadesVendidas); }

void Libro::registrarVenta() { unidadesVendidas++; }
bool Libro::comienzaCon(string frase)
{
    bool ok = true;
    long int largoFrase = frase.size();
    long int largoTitulo = titulo.size();
    if (largoFrase > largoTitulo)
        ok = false;
    else
    {
        long int i = 0;
        while (i != largoFrase && ok)
        {
            if (titulo[i] != frase[i])
                ok = false;
            else
                i++;
        }
    }

    return (ok);
}
bool Libro::datosValidos()
{
    return (isbn > 0 && !(titulo == "") && !(editorial == "") && precioBase <= DBL_MAX && precioBase >= 0);
}
