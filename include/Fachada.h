#ifndef FACHADA_H
#define FACHADA_H

#include <string>
#include "ColeccionAutores.h"
#include "ColeccionLibros.h"
#include "Iterador.h"
#include "TipoError.h"
#include "TipoLibro.h"

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
    // Recibe todos los datos; segun el tipo usa genero, o materia y fecha, o ademas el anio escolar.
    void registrarLibro(TipoLibro, long int, string, string, double, string, string,
                        int, int, int, int, TipoError&);
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
