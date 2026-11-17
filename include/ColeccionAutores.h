#ifndef COLECCION_AUTORES_H
#define COLECCION_AUTORES_H
#include "ABBAutores.h"
#include "IteradorAutores.h"

class ColeccionAutores
{
private:
    ABBAutores arbol;
    // No se permite copiar una coleccion propietaria de objetos dinamicos.
    ColeccionAutores(const ColeccionAutores&);
    ColeccionAutores& operator=(const ColeccionAutores&);
public:
    ColeccionAutores();
    ~ColeccionAutores();
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
