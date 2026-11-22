#ifndef COLECCION_AUTORES_H
#define COLECCION_AUTORES_H

#include "ABBAutores.h"

class ColeccionAutores
{
private:
    ColeccionAutores(const ColeccionAutores&);
    ColeccionAutores& operator=(const ColeccionAutores&);
    ABBAutores tabla;


public:
    ColeccionAutores();
    ~ColeccionAutores();

    bool estaVacia();
    bool existe(long int);

    // Devuelve un puntero prestado al objeto de la clave recibida.
    // PRECONDICION:
    // 1- La clave existe en la coleccion.
    Autor* buscar(long int);

    // Agrega el objeto; la estructura adquiere su propiedad.
    // PRECONDICION:
    // 1- El puntero no es NULL y los datos del objeto son validos.
    // 2- No existe otro objeto con la misma clave.
    // 3- El llamador transfiere la propiedad del objeto a la estructura.
    void insertar(Autor*);

    // Carga referencias a los autores en orden ascendente de cedula.
    // PRECONDICION:
    // 1- El iterador esta vacio y se utiliza mientras exista la coleccion.
    void cargarIterador(Iterador&);
};

#endif
