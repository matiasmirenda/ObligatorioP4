#ifndef FACHADA_H
#define FACHADA_H

#include <string>
#include "ColeccionAutores.h"
#include "ColeccionLibros.h"
#include "Iterador.h"
#include "TipoError.h"

using std::string;

class Fachada
{
private:
    ColeccionAutores autores;
    ColeccionLibros libros; // Se destruye antes que autores.

public:
    Fachada();
    ~Fachada();
    void registrarAutor(long int, string, string, TipoError&);
    // Si falla, el llamador conserva la propiedad del libro.
    void registrarLibro(Libro*, TipoError&);
    void asociarAutor(long int, long int, TipoError&);
    // PRECONDICION:
    // 1- El iterador esta vacio y se usa mientras exista la Fachada.
    void listarPorPrefijo(string, Iterador&, TipoError&);
    // Devuelve una referencia prestada, o NULL si falla.
    void consultarLibro(long int, Libro*&, TipoError&);
    // PRECONDICION:
    // 1- El iterador esta vacio y se usa mientras exista la Fachada.
    void listarAutores(Iterador&, TipoError&);
    void registrarVenta(long int, double&, TipoError&);
    double calcularRecaudacion(string);
    void contarVentasPorTipo(double, long int&, long int&, long int&);
    // Devuelve una referencia prestada, o NULL si falla.
    void consultarMasVendido(string, Libro*&, TipoError&);
};

#endif
