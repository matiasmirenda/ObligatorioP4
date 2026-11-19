#ifndef HASHLIBROS_H
#define HASHLIBROS_H
#include "Libro.h"
#include "Iterador.h"

const int CUBETAS_LIBROS = 500;

class HashLibros
{
private:
    struct Nodo
    {
        Libro *info; // El hash es propietario del libro.
        Nodo *siguiente;
    };
    Nodo* cubetas[CUBETAS_LIBROS];
    int h(long int);
    void crearLista(Nodo*&);
    void destruirLista(Nodo*&);
    bool perteneceLista(Nodo*, long int);
    void insFrontEnLista(Nodo*&, Libro*);
    // PRECONDICION:
    // 1- La clave existe en la lista.
    Libro* obtenerEnLista(Nodo*, long int);

public:
    HashLibros();
    ~HashLibros();
    bool estaVacia();
    bool existe(long int);
    // PRECONDICION:
    // 1- La clave existe en la coleccion.
    Libro *buscar(long int);
    // PRECONDICION:
    // 1- El puntero no es NULL y los datos del objeto son validos.
    // 2- No existe otro objeto con la misma clave.
    // 3- El llamador transfiere la propiedad del objeto a la estructura.
    void insertar(Libro *);
    // PRECONDICION:
    // 1- El iterador esta vacio y se utiliza mientras exista la coleccion.
    void cargarIterador(Iterador&);
};
#endif
