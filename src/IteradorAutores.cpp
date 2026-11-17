#include "IteradorAutores.h"
#include <cstddef>
IteradorAutores::IteradorAutores()
{
    primero = NULL;
    ultimo = NULL;
    cursor = NULL;
}
IteradorAutores::~IteradorAutores()
{
    while (primero != NULL)
    {
        Nodo* borrar = primero;
        primero = primero->siguiente;
        delete borrar;
    }
}
void IteradorAutores::insertar(Autor* elemento)
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
void IteradorAutores::reiniciar() { cursor = primero; }
bool IteradorAutores::hayActual() { return cursor != NULL; }
Autor* IteradorAutores::actual() { return cursor->info; }
void IteradorAutores::avanzar() { cursor = cursor->siguiente; }
