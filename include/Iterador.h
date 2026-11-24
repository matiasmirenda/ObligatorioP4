#ifndef ITERADOR_H
#define ITERADOR_H

#include "Objeto.h"

class Iterador
{
private:
    struct Nodo
    {
        Objeto* info;
        Nodo* siguiente;
    };
    Nodo* primero;
    Nodo* ultimo;
    Nodo* cursor;

public:
    Iterador();
    ~Iterador();

    // Agrega una referencia prestada al final sin adquirir la propiedad del objeto.
    // PRECONDICION:
    // 1- El puntero no es NULL y el objeto vive durante todo el recorrido.
    // 2- Los elementos se cargan antes de comenzar el recorrido.
    void insertar(Objeto*);
    void reiniciar();
    bool hayActual();

    // PRECONDICION:
    // 1- hayActual() devuelve verdadero.
    Objeto* actual();

    // PRECONDICION:
    // 1- hayActual() devuelve verdadero.
    void avanzar();
};

#endif
