#ifndef COLECCION_AUTORES_H
#define COLECCION_AUTORES_H

#include "Autor.h"
#include "Iterador.h"

class ColeccionAutores
{
private:
    struct Nodo
    {
        Autor* info;
        Nodo* izquierdo;
        Nodo* derecho;
    };
    Nodo* abb;
    // Libera los nodos del subarbol y sus autores, dejando el arbol vacio.
    void destruirArbol(Nodo*&);
    // Inserta el autor conservando el orden del ABB y adquiere su propiedad.
    // PRECONDICION:
    // 1- El autor es valido y su clave no existe en el arbol.
    void insertarEnArbol(Nodo*&, Autor*);
    // Carga referencias a los autores en orden ascendente de cedula.
    void cargarIterador(Nodo*, Iterador&);

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
