#include <iostream>
#include "Iterador.h"

Iterador::Iterador()
{
    primero = NULL;
    ultimo = NULL;
    cursor = NULL;
}

Iterador::~Iterador()
{
    while (primero != NULL)
    {
        Nodo* borrar = primero;
        primero = primero->siguiente;
        delete borrar;
    }
}

void Iterador::insertar(Objeto* elemento)
{
    Nodo* nuevo = new Nodo;
    nuevo->info = elemento;
    nuevo->siguiente = NULL;
    if (primero == NULL)
    {
        primero = nuevo;
        cursor = nuevo;
    }
    else
        ultimo->siguiente = nuevo;
    ultimo = nuevo;
}

void Iterador::reiniciar()
{
    cursor = primero;
}

bool Iterador::hayActual()
{
    return (cursor != NULL);
}

Objeto* Iterador::actual()
{
    return (cursor->info);
}

void Iterador::avanzar()
{
    cursor = cursor->siguiente;
}
