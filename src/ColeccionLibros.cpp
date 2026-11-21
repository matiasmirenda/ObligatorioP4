#include "ColeccionLibros.h"
#include <cstddef>

ColeccionLibros::ColeccionLibros()
{
}

// Al finalizar, C++ destruye tabla y esta libera sus objetos y nodos.
ColeccionLibros::~ColeccionLibros()
{
}

bool ColeccionLibros::estaVacia()
{
    return (tabla.estaVacia());
}


bool ColeccionLibros::existe(long int isbn)
{
    return (tabla.existe(isbn));
}

Libro* ColeccionLibros::buscar(long int isbn)
{
    return (tabla.buscar(isbn));
}

void ColeccionLibros::insertar(Libro* libro)
{
    tabla.insertar(libro);
}

void ColeccionLibros::cargarIterador(Iterador& iter)
{
    tabla.cargarIterador(iter);
}

void ColeccionLibros::cargarPorPrefijo(string frase, Iterador& resultado)
{
    Iterador recorrido;
    tabla.cargarIterador(recorrido);
    while (recorrido.hayActual())
    {
        Libro* libro = (Libro*) recorrido.actual();
        if (libro->comienzaCon(frase))
            resultado.insertar(libro);
        recorrido.avanzar();
    }
}

bool ColeccionLibros::existeEditorial(string editorial)
{
    bool encontrado = false;
    Iterador recorrido;
    tabla.cargarIterador(recorrido);
    while (recorrido.hayActual() && !encontrado)
    {
        Libro* libro = (Libro*) recorrido.actual();
        if (libro->getEditorial() == editorial)
            encontrado = true;
        recorrido.avanzar();
    }
    return (encontrado);
}

double ColeccionLibros::calcularRecaudacion(string editorial)
{
    double total = 0;
    Iterador recorrido;
    tabla.cargarIterador(recorrido);
    while (recorrido.hayActual())
    {
        Libro* libro = (Libro*) recorrido.actual();
        if (libro->getEditorial() == editorial)
            total += libro->getUnidadesVendidas() * libro->calcularPrecioVenta();
        recorrido.avanzar();
    }
    return (total);
}

void ColeccionLibros::contarVentasPorTipo(double precio, long int& novelas, long int& textos, long int& escolares)
{
    novelas = 0;
    textos = 0;
    escolares = 0;
    Iterador recorrido;
    tabla.cargarIterador(recorrido);
    while (recorrido.hayActual())
    {
        Libro* libro = (Libro*) recorrido.actual();
        if (libro->getPrecioBase() > precio)
        {
            if (libro->getTipo() == NOVELA)
                novelas += libro->getUnidadesVendidas();
            else if (libro->getTipo() == TEXTO)
                textos += libro->getUnidadesVendidas();
            else
                escolares += libro->getUnidadesVendidas();
        }
        recorrido.avanzar();
    }
}

Libro* ColeccionLibros::masVendido(string editorial)
{
    Libro* mayor = NULL;
    Iterador recorrido;
    tabla.cargarIterador(recorrido);
    while (recorrido.hayActual())
    {
        Libro* libro = (Libro*) recorrido.actual();
        if (libro->getEditorial() == editorial)
        {
            if (mayor == NULL || libro->getUnidadesVendidas() > mayor->getUnidadesVendidas())
                mayor = libro;
        }
        recorrido.avanzar();
    }
    return (mayor);
}
