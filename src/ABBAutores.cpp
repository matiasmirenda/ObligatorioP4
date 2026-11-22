#include "ABBAutores.h"
ABBAutores::ABBAutores()
{
    raiz = NULL;
}
void ABBAutores::destruirArbol(Nodo*& nodo)
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
ABBAutores::~ABBAutores() { destruirArbol(raiz); }
bool ABBAutores::estaVacia() { return (raiz == NULL); }
bool ABBAutores::existe(long cedula)
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
Autor* ABBAutores::buscar(long cedula)
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
void ABBAutores::insertarEnArbol(Nodo*& nodo, Autor* autor)
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
void ABBAutores::insertar(Autor* autor)
{
    insertarEnArbol(raiz, autor);
}
void ABBAutores::cargarIterador(Nodo* nodo, Iterador& iter)
{
    if (nodo != NULL)
    {
        cargarIterador(nodo->izquierdo, iter);
        iter.insertar(nodo->info);
        cargarIterador(nodo->derecho, iter);
    }
}
void ABBAutores::cargarIterador(Iterador& iter)
{
    cargarIterador(raiz, iter);
}
