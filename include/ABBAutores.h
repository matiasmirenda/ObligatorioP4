#ifndef ABBAUTORES_H
#define ABBAUTORES_H
#include "Autor.h"
#include "Iterador.h"
class ABBAutores
{
private:
    struct Nodo
    {
        Autor* info; // El ABB es propietario del autor.
        Nodo* izquierdo;
        Nodo* derecho;
    };
    Nodo* raiz;
    void destruirArbol(Nodo*&);
    // PRECONDICION:
    // 1- El autor es valido y su clave no existe en el arbol.
    void insertarEnArbol(Nodo*&, Autor*);
    void cargarIterador(Nodo*, Iterador&);
public:
    ABBAutores();
    ~ABBAutores();
    bool estaVacia();
    bool existe(long);
    // PRECONDICION:
    // 1- La clave existe en la coleccion.
    Autor* buscar(long);
    // PRECONDICION:
    // 1- El puntero no es NULL y los datos del objeto son validos.
    // 2- No existe otro objeto con la misma clave.
    // 3- El llamador transfiere la propiedad del objeto a la estructura.
    void insertar(Autor*);
    // PRECONDICION:
    // 1- El iterador esta vacio y se utiliza mientras exista la coleccion.
    void cargarIterador(Iterador&);
};
#endif
