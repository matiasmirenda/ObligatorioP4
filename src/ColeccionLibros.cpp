#include "ColeccionLibros.h"

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
