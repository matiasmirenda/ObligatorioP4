#ifndef NOVELA_H
#define NOVELA_H
#include "Libro.h"
#include "Autor.h"
class Novela : public Libro
{
private:
    String genero;
    Autor* autor;
public:
    Novela(long long, String, String, double, String);
    Novela(const Novela&);
    ~Novela();
    String getGenero();
    Autor* getAutor();
    // PRECONDICION:
    // 1- El nuevo genero no esta vacio. Se usa "infantil" para el descuento.
    void setGenero(String);
    // PRECONDICION:
    // 1- El puntero refiere a un autor valido registrado en ColeccionAutores.
    // 2- El autor permanece vivo mientras la novela lo referencia.
    void asignarAutor(Autor*);
    bool datosValidos();
    double calcularPrecioVenta();
    String getTipo();
};
#endif
