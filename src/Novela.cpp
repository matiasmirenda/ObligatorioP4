#include <iostream>
#include "ValidacionDatos.h"
#include "Parametros.h"
#include "Novela.h"

using namespace std;

Novela::Novela(long int cod, string tit, string ed, float precio, string gen)
    : Libro(cod, tit, ed, precio), genero(gen)
{
    autor = NULL;
}
string Novela::getGenero() { return (genero); }
Autor *Novela::getAutor() { return (autor); }
void Novela::asignarAutor(Autor *nuevo) { autor = nuevo; }
bool Novela::datosValidos() { return (Libro::datosValidos() && textoValido(genero)); }
float Novela::calcularPrecioVenta()
{
    float precio = getPrecioBase();
    if (genero == "infantil")
        precio = precio * FACTOR_PRECIO_INFANTIL;
    return (precio);
}
TipoLibro Novela::getTipo() { return (NOVELA); }

Novela::~Novela()
{
}

void Novela::imprimir()
{
    Libro::imprimir();
    cout << "Genero: " << genero << '\n';
    if (autor != NULL)
    {
        cout << "Autor: ";
        autor->imprimir();
    }
    else
        cout << "Sin autor asociado.\n";
}
