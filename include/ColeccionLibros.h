#ifndef COLECCION_LIBROS_H
#define COLECCION_LIBROS_H
#include "HashLibros.h"
#include "IteradorLibros.h"

class ColeccionLibros
{
private:
    HashLibros tabla;
    // No se permite copiar una coleccion propietaria de objetos dinamicos.
    ColeccionLibros(const ColeccionLibros&);
    ColeccionLibros& operator=(const ColeccionLibros&);
public:
    ColeccionLibros();
    ~ColeccionLibros();
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
    IteradorLibros* crearIterador();//Comentario
};
#endif
