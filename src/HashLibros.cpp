#include "HashLibros.h"

HashLibros::HashLibros()
{
    for (int i = 0; i < CUBETAS_LIBROS; i++)
        crearLista(cubetas[i]);
}
HashLibros::~HashLibros()
{
    for (int i = 0; i < CUBETAS_LIBROS; i++)
        destruirLista(cubetas[i]);
}
int HashLibros::h(long int isbn) { return (isbn % CUBETAS_LIBROS); }
bool HashLibros::estaVacia()
{
    int i = 0;
    while (i < CUBETAS_LIBROS && cubetas[i] == NULL)
        i++;
    return (i == CUBETAS_LIBROS);
}
bool HashLibros::existe(long int isbn)
{
    bool pertenece = false;
    if (isbn > 0)
        pertenece = perteneceLista(cubetas[h(isbn)], isbn);
    return (pertenece);
}
Libro *HashLibros::buscar(long int isbn)
{
    return (obtenerEnLista(cubetas[h(isbn)], isbn));
}
void HashLibros::insertar(Libro *libro)
{
    insFrontEnLista(cubetas[h(libro->getId())], libro);
}
void HashLibros::crearLista(Nodo*& lista)
{
    lista = NULL;
}
void HashLibros::destruirLista(Nodo*& lista)
{
    while (lista != NULL)
    {
        Nodo* siguiente = lista->siguiente;
        delete lista->info;
        delete lista;
        lista = siguiente;
    }
}
bool HashLibros::perteneceLista(Nodo* lista, long int isbn)
{
    while (lista != NULL && lista->info->getId() != isbn)
        lista = lista->siguiente;
    return (lista != NULL);
}
void HashLibros::insFrontEnLista(Nodo*& lista, Libro* libro)
{
    Nodo* nuevo = new Nodo;
    nuevo->info = libro;
    nuevo->siguiente = lista;
    lista = nuevo;
}
Libro* HashLibros::obtenerEnLista(Nodo* lista, long int isbn)
{
    while (lista->info->getId() != isbn)
        lista = lista->siguiente;
    return (lista->info);
}
void HashLibros::cargarIterador(Iterador& iter)
{
    for (int i = 0; i < CUBETAS_LIBROS; i++)
    {
        Nodo *actual = cubetas[i];
        while (actual != NULL)
        {
            iter.insertar(actual->info);
            actual = actual->siguiente;
        }
    }
}
