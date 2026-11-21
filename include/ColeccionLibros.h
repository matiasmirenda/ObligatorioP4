#ifndef COLECCION_LIBROS_H
#define COLECCION_LIBROS_H

#include "HashLibros.h"
#include "Iterador.h"

class ColeccionLibros
{
private:
    HashLibros tabla; // Miembro por valor: se construye y destruye con la coleccion.


public:
    ColeccionLibros();
    ~ColeccionLibros();

    bool estaVacia();
    bool existe(long int);

    // PRECONDICION:
    // 1- La clave existe en la coleccion.
    Libro* buscar(long int);

    // PRECONDICION:
    // 1- El puntero no es NULL y los datos del objeto son validos.
    // 2- No existe otro objeto con la misma clave.
    // 3- El llamador transfiere la propiedad del objeto a la estructura.
    void insertar(Libro*);

    // PRECONDICION:
    // 1- El iterador esta vacio y se utiliza mientras exista la coleccion.
    void cargarIterador(Iterador&);
};

#endif
