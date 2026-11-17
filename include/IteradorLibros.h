#ifndef ITERADOR_LIBROS_H
#define ITERADOR_LIBROS_H
#include "Libro.h"
class IteradorLibros
{
private:
    struct Nodo { Libro* info; Nodo* siguiente; };
    Nodo* primero;
    Nodo* ultimo;
    Nodo* cursor;
    // Evita copiar los nodos propietarios del iterador.
    IteradorLibros(const IteradorLibros&);
    IteradorLibros& operator=(const IteradorLibros&);
public:
    IteradorLibros();
    ~IteradorLibros();
    // PRECONDICION:
    // 1- El puntero no es NULL y el objeto vive mientras se usa el iterador.
    // 2- La carga se realiza antes de empezar el recorrido.
    void insertar(Libro*);
    void reiniciar();
    bool hayActual();
    // PRECONDICION:
    // 1- hayActual() devuelve verdadero.
    Libro* actual();
    // PRECONDICION:
    // 1- hayActual() devuelve verdadero.
    void avanzar();
};
#endif
