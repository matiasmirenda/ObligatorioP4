#ifndef ABBAUTORES_H
#define ABBAUTORES_H
#include "Autor.h"
#include "IteradorAutores.h"
class ABBAutores
{
private:
    struct Nodo { Autor* info; Nodo* izquierdo; Nodo* derecho; };
    Nodo* raiz;
    int cantidad;
    void destruirArbol(Nodo*);
    void cargarIterador(Nodo*, IteradorAutores*);
    ABBAutores(const ABBAutores&);
    ABBAutores& operator=(const ABBAutores&);
public:
    ABBAutores();
    ~ABBAutores();
    bool estaVacia();
    int getCantidad();
    bool existe(long);
    // PRECONDICION:
    // 1- La clave existe en la coleccion.
    Autor* buscar(long);
    // PRECONDICION:
    // 1- El puntero no es NULL y los datos del objeto son validos.
    // 2- No existe otro objeto con la misma clave.
    // 3- El llamador transfiere la propiedad del objeto a la estructura.
    void insertar(Autor*);
    // El llamador elimina el iterador antes de destruir la coleccion.
    IteradorAutores* crearIterador();
};
#endif
