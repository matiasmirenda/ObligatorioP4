#include <iostream>
#include "ValidacionDatos.h"
#include "Libro.h"

using namespace std;

Libro::Libro(long int cod, string tit, string ed, float precio)
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
float Libro::getPrecioBase() { return (precioBase); }
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
    return (isbn > 0 && textoValido(titulo) && textoValido(editorial) && precioBase >= 0);
}

void Libro::imprimirTipo()
{
    switch (getTipo())
    {
        case NOVELA:
            cout << "Novela";
            break;
        case TEXTO:
            cout << "Texto";
            break;
        case ESCOLAR:
            cout << "Escolar";
            break;
    }
}

void Libro::imprimir()
{
    cout << "ISBN: " << isbn << "\nTitulo: " << titulo
              << "\nEditorial: " << editorial << "\nTipo: ";
    imprimirTipo();
    cout << "\nPrecio base: " << precioBase
              << "\nUnidades vendidas: " << unidadesVendidas << '\n';
}

void Libro::imprimirBasico()
{
    cout << isbn << " | " << titulo << " | ";
    imprimirTipo();
    cout << '\n';
}

void Libro::imprimirPrecioVenta()
{
    cout << "Importe: " << calcularPrecioVenta() << '\n';
}
