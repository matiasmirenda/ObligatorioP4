#ifndef NOVELA_H
#define NOVELA_H
#include "Libro.h"
#include "Autor.h"
class Novela : public Libro
{
private:
    string genero;
    Autor *autor;

public:
    void imprimir();
    Novela(long int, string, string, float, string);
    ~Novela();
    string getGenero();
    Autor *getAutor();

    // Asocia el autor recibido, reemplazando la referencia anterior sin eliminarla.
    // PRECONDICION:
    // 1- El puntero refiere a un autor valido registrado en ColeccionAutores.
    // 2- El autor permanece vivo mientras la novela lo referencia.
    void asignarAutor(Autor *);
    bool datosValidos();
    // Aplica el descuento infantil al precio base cuando corresponde.
    float calcularPrecioVenta();
    TipoLibro getTipo();
};
#endif
