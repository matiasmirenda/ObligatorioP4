#ifndef FACHADA_H
#define FACHADA_H

#include "ColeccionAutores.h"
#include "ColeccionLibros.h"
#include "TipoError.h"

using std::string;

class Fachada
{
private:
    ColeccionAutores autores;
    ColeccionLibros libros;

public:
    Fachada();
    ~Fachada();
    // Registra el autor o informa que su cedula ya existe.
    // PRECONDICION:
    // 1- La cedula tiene ocho digitos y nombre y apellido cumplen textoValido().
    void registrarAutor(long int, string, string, TipoError&);
    // Crea el subtipo con cero ventas o informa que el ISBN ya existe.
    // PRECONDICION:
    // 1- El ISBN tiene trece digitos y los datos del subtipo son validos.
    // 2- El precio es no negativo y los textos usados cumplen textoValido().
    // 3- El tipo es NOVELA, TEXTO o ESCOLAR; la fecha y el anio escolar son validos si corresponden.
    void registrarLibro(TipoLibro, long int, string, string, float, string, string,
                        int, int, int, int, TipoError&);
    // Controla existencia y tipo novela antes de asociar o reemplazar el autor.
    // PRECONDICION:
    // 1- El ISBN tiene trece digitos y la cedula ocho.
    void asociarAutor(long int, long int, TipoError&);
    // Lista los datos basicos de las coincidencias exactas o informa SIN_COINCIDENCIAS.
    // PRECONDICION:
    // 1- La frase cumple textoValido().
    void listarPorPrefijo(string, TipoError&);
    // Lista los datos completos del libro o informa que el ISBN no existe.
    // PRECONDICION:
    // 1- El ISBN tiene trece digitos.
    void consultarLibro(long int, TipoError&);
    // Lista los autores por cedula ascendente o informa SIN_AUTORES.
    void listarAutores(TipoError&);
    // Registra una venta e informa su importe, o devuelve error si el libro no existe.
    // PRECONDICION:
    // 1- El ISBN tiene trece digitos.
    void registrarVenta(long int, TipoError&);
    // Calcula y muestra la recaudacion de la editorial.
    // PRECONDICION:
    // 1- La editorial cumple textoValido().
    void calcularRecaudacion(string);
    // Muestra las unidades vendidas por tipo con precio base estrictamente mayor.
    // PRECONDICION:
    // 1- El precio de referencia es no negativo.
    void contarVentasPorTipo(float);
    // Lista los datos de un libro con ventas maximas de la editorial, o informa error.
    // PRECONDICION:
    // 1- La editorial cumple textoValido().
    void consultarMasVendido(string, TipoError&);
};

#endif
