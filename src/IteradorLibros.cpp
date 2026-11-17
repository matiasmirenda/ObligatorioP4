#include "IteradorLibros.h"
#include <cstddef>
IteradorLibros::IteradorLibros()
{
    primero = NULL;
    ultimo = NULL;
    cursor = NULL;
}
IteradorLibros::~IteradorLibros()
{
    while (primero != NULL)
    {
        Nodo* borrar = primero;
        primero = primero->siguiente;
        delete borrar;
    }
}
void IteradorLibros::insertar(Libro* elemento)
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
void IteradorLibros::reiniciar() { cursor = primero; }
bool IteradorLibros::hayActual() { return cursor != NULL; }
Libro* IteradorLibros::actual() { return cursor->info; }
void IteradorLibros::avanzar() { cursor = cursor->siguiente; }
