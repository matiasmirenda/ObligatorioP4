#ifndef ITERADOR_AUTORES_H
#define ITERADOR_AUTORES_H
#include "Autor.h"
class IteradorAutores
{
private:
    struct Nodo { Autor* info; Nodo* siguiente; };
    Nodo* primero;
    Nodo* ultimo;
    Nodo* cursor;
    // Evita copiar los nodos propietarios del iterador.
    IteradorAutores(const IteradorAutores&);
    IteradorAutores& operator=(const IteradorAutores&);
public:
    IteradorAutores();
    ~IteradorAutores();
    // PRECONDICION:
    // 1- El puntero no es NULL y el objeto vive mientras se usa el iterador.
    // 2- La carga se realiza antes de empezar el recorrido.
    void insertar(Autor*);
    void reiniciar();
    bool hayActual();
    // PRECONDICION:
    // 1- hayActual() devuelve verdadero.
    Autor* actual();
    // PRECONDICION:
    // 1- hayActual() devuelve verdadero.
    void avanzar();
};
#endif
