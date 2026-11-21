#include "Fachada.h"
#include "Novela.h"
#include "Texto.h"
#include "Escolar.h"
#include <cstddef>
#include <climits>

Fachada::Fachada() {}
Fachada::~Fachada() {}

void Fachada::registrarAutor(long int cedula, string nombre, string apellido, TipoError& error)
{
    error = SIN_ERROR;
    if (cedula <= 0 || nombre == "" || apellido == "")
        error = DATOS_INVALIDOS;
    else if (autores.existe(cedula))
        error = AUTOR_YA_EXISTE;
    else
        autores.insertar(new Autor(cedula, nombre, apellido));
}

void Fachada::registrarLibro(TipoLibro tipo, long int isbn, string titulo, string editorial, double precioBase,
                             string genero, string materia, int dia, int mes, int anio, int anioEscuela,
                             TipoError& error)
{
    error = SIN_ERROR;
    Libro* libro;
    if (tipo == NOVELA)
        libro = new Novela(isbn, titulo, editorial, precioBase, genero);
    else if (tipo == TEXTO)
        libro = new Texto(isbn, titulo, editorial, precioBase, materia, Fecha(dia, mes, anio));
    else
        libro = new Escolar(isbn, titulo, editorial, precioBase, materia, Fecha(dia, mes, anio), anioEscuela);
    if (!libro->datosValidos())
    {
        error = DATOS_INVALIDOS;
        delete libro;
    }
    else if (libros.existe(isbn))
    {
        error = LIBRO_YA_EXISTE;
        delete libro;
    }
    else
        libros.insertar(libro);
}

void Fachada::asociarAutor(long int isbn, long int cedula, TipoError& error)
{
    error = SIN_ERROR;
    if (!libros.existe(isbn))
        error = LIBRO_NO_EXISTE;
    else
    {
        Libro* libro = libros.buscar(isbn);
        if (libro->getTipo() != NOVELA)
            error = LIBRO_NO_ES_NOVELA;
        else if (!autores.existe(cedula))
            error = AUTOR_NO_EXISTE;
        else
        {
            Novela* novela = (Novela*) libro;
            novela->asignarAutor(autores.buscar(cedula));
        }
    }
}

void Fachada::listarPorPrefijo(string frase, Iterador& resultado, TipoError& error)
{
    error = SIN_ERROR;
    libros.cargarPorPrefijo(frase, resultado);
    if (!resultado.hayActual())
        error = SIN_COINCIDENCIAS;
}

void Fachada::consultarLibro(long int isbn, Libro*& libro, TipoError& error)
{
    error = SIN_ERROR;
    libro = NULL;
    if (!libros.existe(isbn))
        error = LIBRO_NO_EXISTE;
    else
        libro = libros.buscar(isbn);
}

void Fachada::listarAutores(Iterador& resultado, TipoError& error)
{
    error = SIN_ERROR;
    if (autores.estaVacia())
        error = SIN_AUTORES;
    else
        autores.cargarIterador(resultado);
}

void Fachada::registrarVenta(long int isbn, double& importe, TipoError& error)
{
    error = SIN_ERROR;
    importe = 0;
    if (!libros.existe(isbn))
        error = LIBRO_NO_EXISTE;
    else
    {
        Libro* libro = libros.buscar(isbn);
        if (libro->getUnidadesVendidas() == INT_MAX)
            error = LIMITE_VENTAS;
        else
        {
            importe = libro->calcularPrecioVenta();
            libro->registrarVenta();
        }
    }
}

double Fachada::calcularRecaudacion(string editorial)
{
    return (libros.calcularRecaudacion(editorial));
}

void Fachada::contarVentasPorTipo(double precio, long int& novelas, long int& textos, long int& escolares)
{
    libros.contarVentasPorTipo(precio, novelas, textos, escolares);
}

void Fachada::consultarMasVendido(string editorial, Libro*& resultado, TipoError& error)
{
    error = SIN_ERROR;
    resultado = NULL;
    if (!libros.existeEditorial(editorial))
        error = SIN_LIBROS_DE_EDITORIAL;
    else
        resultado = libros.masVendido(editorial);
}
