#include "ABBAutores.h"
#include <cstddef>
ABBAutores::ABBAutores()
{
    raiz = NULL;
    cantidad = 0;
}
void ABBAutores::destruirArbol(Nodo* nodo)
{
    if (nodo != NULL)
    {
        destruirArbol(nodo->izquierdo);
        destruirArbol(nodo->derecho);
        delete nodo->info;
        delete nodo;
    }
}
ABBAutores::~ABBAutores() { destruirArbol(raiz); }
bool ABBAutores::estaVacia() { return cantidad == 0; }
int ABBAutores::getCantidad() { return cantidad; }
bool ABBAutores::existe(long cedula)
{
    Nodo* actual = raiz;
    while (actual != NULL && actual->info->getCedula() != cedula)
    {
        if (cedula < actual->info->getCedula())
            actual = actual->izquierdo;
        else
            actual = actual->derecho;
    }
    return actual != NULL;
}
Autor* ABBAutores::buscar(long cedula)
{
    Nodo* actual = raiz;
    while (actual->info->getCedula() != cedula)
    {
        if (cedula < actual->info->getCedula())
            actual = actual->izquierdo;
        else
            actual = actual->derecho;
    }
    return actual->info;
}
void ABBAutores::insertar(Autor* autor)
{
    Nodo* nuevo = new Nodo;
    nuevo->info = autor;
    nuevo->izquierdo = NULL;
    nuevo->derecho = NULL;
    if (raiz == NULL)
        raiz = nuevo;
    else
    {
        Nodo* actual = raiz;
        bool insertado = false;
        while (!insertado)
        {
            if (autor->getCedula() < actual->info->getCedula())
            {
                if (actual->izquierdo == NULL)
                {
                    actual->izquierdo = nuevo;
                    insertado = true;
                }
                else
                    actual = actual->izquierdo;
            }
            else
            {
                if (actual->derecho == NULL)
                {
                    actual->derecho = nuevo;
                    insertado = true;
                }
                else
                    actual = actual->derecho;
            }
        }
    }
    cantidad++;
}
void ABBAutores::cargarIterador(Nodo* nodo, IteradorAutores* iter)
{
    if (nodo != NULL)
    {
        cargarIterador(nodo->izquierdo, iter);
        iter->insertar(nodo->info);
        cargarIterador(nodo->derecho, iter);
    }
}
IteradorAutores* ABBAutores::crearIterador()
{
    IteradorAutores* iter = new IteradorAutores;
    cargarIterador(raiz, iter);
    return iter;
}
