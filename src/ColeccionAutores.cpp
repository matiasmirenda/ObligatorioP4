#include "ColeccionAutores.h"
ColeccionAutores::ColeccionAutores()
{
    raiz = NULL;
}
void ColeccionAutores::destruirArbol(Nodo*& nodo)
{
    if (nodo != NULL)
    {
        destruirArbol(nodo->izquierdo);
        destruirArbol(nodo->derecho);
        delete nodo->info;
        delete nodo;
        nodo = NULL;
    }
}
ColeccionAutores::~ColeccionAutores() { destruirArbol(raiz); }
bool ColeccionAutores::estaVacia() { return (raiz == NULL); }
bool ColeccionAutores::existe(long int cedula)
{
    Nodo* actual = raiz;
    while (actual != NULL && actual->info->getId() != cedula)
    {
        if (cedula < actual->info->getId())
            actual = actual->izquierdo;
        else
            actual = actual->derecho;
    }
    return (actual != NULL);
}
Autor* ColeccionAutores::buscar(long int cedula)
{
    Nodo* actual = raiz;
    while (actual->info->getId() != cedula)
    {
        if (cedula < actual->info->getId())
            actual = actual->izquierdo;
        else
            actual = actual->derecho;
    }
    return (actual->info);
}
void ColeccionAutores::insertarEnArbol(Nodo*& nodo, Autor* autor)
{
    if (nodo == NULL)
    {
        nodo = new Nodo;
        nodo->info = autor;
        nodo->izquierdo = NULL;
        nodo->derecho = NULL;
    }
    else
    {
        if (autor->getId() < nodo->info->getId())
            insertarEnArbol(nodo->izquierdo, autor);
        else
            insertarEnArbol(nodo->derecho, autor);
    }
}
void ColeccionAutores::insertar(Autor* autor)
{
    insertarEnArbol(raiz, autor);
}
void ColeccionAutores::cargarIterador(Nodo* nodo, Iterador& iter)
{
    if (nodo != NULL)
    {
        cargarIterador(nodo->izquierdo, iter);
        iter.insertar(nodo->info);
        cargarIterador(nodo->derecho, iter);
    }
}
void ColeccionAutores::cargarIterador(Iterador& iter)
{
    cargarIterador(raiz, iter);
}
