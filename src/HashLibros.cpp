#include "HashLibros.h"
#include <cstddef>
HashLibros::HashLibros()
{
    cantidadCubetas = 1009;
    cantidad = 0;
    cubetas = new Nodo*[cantidadCubetas];
    for (int i = 0; i < cantidadCubetas; i++)
        cubetas[i] = NULL;
}
HashLibros::~HashLibros()
{
    for (int i = 0; i < cantidadCubetas; i++)
    {
        Nodo* actual = cubetas[i];
        while (actual != NULL)
        {
            Nodo* siguiente = actual->siguiente;
            delete actual->info;
            delete actual;
            actual = siguiente;
        }
    }
    delete[] cubetas;
}
int HashLibros::h(long long isbn) { return isbn % cantidadCubetas; }
bool HashLibros::estaVacia() { return cantidad == 0; }
int HashLibros::getCantidad() { return cantidad; }
bool HashLibros::existe(long long isbn)
{
    if (isbn <= 0)
        return false;
    Nodo* actual = cubetas[h(isbn)];
    while (actual != NULL && actual->info->getIsbn() != isbn)
        actual = actual->siguiente;
    return actual != NULL;
}
Libro* HashLibros::buscar(long long isbn)
{
    Nodo* actual = cubetas[h(isbn)];
    while (actual->info->getIsbn() != isbn)
        actual = actual->siguiente;
    return actual->info;
}
void HashLibros::insertar(Libro* libro)
{
    int cubeta = h(libro->getIsbn());
    Nodo* nuevo = new Nodo;
    nuevo->info = libro;
    nuevo->siguiente = cubetas[cubeta];
    cubetas[cubeta] = nuevo;
    cantidad++;
}
IteradorLibros* HashLibros::crearIterador()
{
    IteradorLibros* iter = new IteradorLibros;
    for (int i = 0; i < cantidadCubetas; i++)
    {
        Nodo* actual = cubetas[i];
        while (actual != NULL)
        {
            iter->insertar(actual->info);
            actual = actual->siguiente;
        }
    }
    return iter;
}
