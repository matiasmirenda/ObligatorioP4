#include <iostream>
#include "ColeccionLibros.h"

using namespace std;

ColeccionLibros::ColeccionLibros()
{
    for (int i = 0; i < CUBETAS_LIBROS; i++)
        crearLista(hash[i]);
}
ColeccionLibros::~ColeccionLibros()
{
    for (int i = 0; i < CUBETAS_LIBROS; i++)
        destruirLista(hash[i]);
}
int ColeccionLibros::h(long int isbn) { return (isbn % CUBETAS_LIBROS); }
bool ColeccionLibros::estaVacia()
{
    int i = 0;
    while (i < CUBETAS_LIBROS && hash[i] == NULL)
        i++;
    return (i == CUBETAS_LIBROS);
}
bool ColeccionLibros::existe(long int isbn)
{
    bool pertenece = false;
    if (isbn > 0)
        pertenece = perteneceLista(hash[h(isbn)], isbn);
    return (pertenece);
}
Libro *ColeccionLibros::buscar(long int isbn)
{
    return (obtenerEnLista(hash[h(isbn)], isbn));
}
void ColeccionLibros::insertar(Libro *libro)
{
    insFrontEnLista(hash[h(libro->getId())], libro);
}
void ColeccionLibros::crearLista(Nodo*& lista)
{
    lista = NULL;
}
void ColeccionLibros::destruirLista(Nodo*& lista)
{
    while (lista != NULL)
    {
        Nodo* siguiente = lista->siguiente;
        delete lista->info;
        delete lista;
        lista = siguiente;
    }
}
bool ColeccionLibros::perteneceLista(Nodo* lista, long int isbn)
{
    while (lista != NULL && lista->info->getId() != isbn)
        lista = lista->siguiente;
    return (lista != NULL);
}
void ColeccionLibros::insFrontEnLista(Nodo*& lista, Libro* libro)
{
    Nodo* nuevo = new Nodo;
    nuevo->info = libro;
    nuevo->siguiente = lista;
    lista = nuevo;
}
Libro* ColeccionLibros::obtenerEnLista(Nodo* lista, long int isbn)
{
    while (lista->info->getId() != isbn)
        lista = lista->siguiente;
    return (lista->info);
}
void ColeccionLibros::cargarIterador(Iterador& iter)
{
    for (int i = 0; i < CUBETAS_LIBROS; i++)
    {
        Nodo *actual = hash[i];
        while (actual != NULL)
        {
            iter.insertar(actual->info);
            actual = actual->siguiente;
        }
    }
}

void ColeccionLibros::cargarPorPrefijo(string frase, Iterador& resultado)
{
    Iterador recorrido;
    cargarIterador(recorrido);
    while (recorrido.hayActual())
    {
        Libro* libro = (Libro*) recorrido.actual();
        if (libro->comienzaCon(frase))
            resultado.insertar(libro);
        recorrido.avanzar();
    }
}

bool ColeccionLibros::existeEditorial(string editorial)
{
    bool encontrado = false;
    Iterador recorrido;
    cargarIterador(recorrido);
    while (recorrido.hayActual() && !encontrado)
    {
        Libro* libro = (Libro*) recorrido.actual();
        if (libro->getEditorial() == editorial)
            encontrado = true;
        recorrido.avanzar();
    }
    return (encontrado);
}

float ColeccionLibros::calcularRecaudacion(string editorial)
{
    float total = 0;
    Iterador recorrido;
    cargarIterador(recorrido);
    while (recorrido.hayActual())
    {
        Libro* libro = (Libro*) recorrido.actual();
        if (libro->getEditorial() == editorial)
            total += libro->getUnidadesVendidas() * libro->calcularPrecioVenta();
        recorrido.avanzar();
    }
    return (total);
}

void ColeccionLibros::contarVentasPorTipo(float precio, long int& novelas, long int& textos, long int& escolares)
{
    novelas = 0;
    textos = 0;
    escolares = 0;
    Iterador recorrido;
    cargarIterador(recorrido);
    while (recorrido.hayActual())
    {
        Libro* libro = (Libro*) recorrido.actual();
        if (libro->getPrecioBase() > precio)
        {
            if (libro->getTipo() == NOVELA)
                novelas += libro->getUnidadesVendidas();
            else if (libro->getTipo() == TEXTO)
                textos += libro->getUnidadesVendidas();
            else
                escolares += libro->getUnidadesVendidas();
        }
        recorrido.avanzar();
    }
}

Libro* ColeccionLibros::masVendido(string editorial)
{
    Libro* mayor = NULL;
    Iterador recorrido;
    cargarIterador(recorrido);
    while (recorrido.hayActual())
    {
        Libro* libro = (Libro*) recorrido.actual();
        if (libro->getEditorial() == editorial)
        {
            if (mayor == NULL || libro->getUnidadesVendidas() > mayor->getUnidadesVendidas())
                mayor = libro;
        }
        recorrido.avanzar();
    }
    return (mayor);
}

void ColeccionLibros::imprimirRecaudacion(string editorial)
{
    cout << "Recaudacion: " << calcularRecaudacion(editorial) << '\n';
}

void ColeccionLibros::imprimirVentasPorTipo(float precio)
{
    long int novelas, textos, escolares;
    contarVentasPorTipo(precio, novelas, textos, escolares);
    cout << "Unidades vendidas: novelas " << novelas << ", textos comunes " << textos
              << ", escolares " << escolares << '\n';
}
