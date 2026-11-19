#include "ColeccionAutores.h"

ColeccionAutores::ColeccionAutores()
{
}

// Al finalizar, C++ destruye tabla y esta libera sus objetos y nodos.
ColeccionAutores::~ColeccionAutores()
{
}

bool ColeccionAutores::estaVacia()
{
    return (tabla.estaVacia());
}


bool ColeccionAutores::existe(long int cedula)
{
    return (tabla.existe(cedula));
}

Autor* ColeccionAutores::buscar(long int cedula)
{
    return (tabla.buscar(cedula));
}

void ColeccionAutores::insertar(Autor* autor)
{
    tabla.insertar(autor);
}

void ColeccionAutores::cargarIterador(Iterador& iter)
{
    tabla.cargarIterador(iter);
}
