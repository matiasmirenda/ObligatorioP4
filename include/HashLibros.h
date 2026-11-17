#ifndef HASHLIBROS_H
#define HASHLIBROS_H
#include "Libro.h"
#include "IteradorLibros.h"
class HashLibros
{
private:
    struct Nodo { Libro* info; Nodo* siguiente; };
    Nodo** cubetas;
    int cantidadCubetas;
    int cantidad;
    int h(long long);
    HashLibros(const HashLibros&);
    HashLibros& operator=(const HashLibros&);
public:
    HashLibros();
    ~HashLibros();
    bool estaVacia();
    int getCantidad();
    bool existe(long long);
    // PRECONDICION:
    // 1- La clave existe en la coleccion.
    Libro* buscar(long long);
    // PRECONDICION:
    // 1- El puntero no es NULL y los datos del objeto son validos.
    // 2- No existe otro objeto con la misma clave.
    // 3- El llamador transfiere la propiedad del objeto a la estructura.
    void insertar(Libro*);
    // El llamador elimina el iterador antes de destruir la coleccion.
    IteradorLibros* crearIterador();
};
#endif
