#ifndef NOVELA_H
#define NOVELA_H
#include "Libro.h"
#include "Autor.h"
class Novela : public Libro
{
private:
    string genero;
    Autor *autor; // Referencia prestada al autor registrado; la novela no lo elimina.

public:
    Novela(long int, string, string, double, string);
    ~Novela();
    string getGenero();
    Autor *getAutor();

    // PRECONDICION:
    // 1- El puntero refiere a un autor valido registrado en ColeccionAutores.
    // 2- El autor permanece vivo mientras la novela lo referencia.
    void asignarAutor(Autor *);
    bool datosValidos();
    double calcularPrecioVenta();
    TipoLibro getTipo();
};
#endif
