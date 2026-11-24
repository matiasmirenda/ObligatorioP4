#include "Fachada.h"
#include "Novela.h"
#include "Escolar.h"

Fachada::Fachada() {}
Fachada::~Fachada() {}

void Fachada::registrarAutor(long int cedula, string nombre, string apellido, TipoError& error)
{
    error = SIN_ERROR;
    if (autores.existe(cedula))
        error = AUTOR_YA_EXISTE;
    else
        autores.insertar(new Autor(cedula, nombre, apellido));
}

void Fachada::registrarLibro(TipoLibro tipo, long int isbn, string titulo, string editorial, float precioBase,
                             string genero, string materia, int dia, int mes, int anio, int anioEscuela,
                             TipoError& error)
{
    error = SIN_ERROR;
    if (libros.existe(isbn))
        error = LIBRO_YA_EXISTE;
    else
    {
        Libro* libro = NULL;
        switch (tipo)
        {
            case NOVELA:
                libro = new Novela(isbn, titulo, editorial, precioBase, genero);
                break;
            case TEXTO:
                libro = new Texto(isbn, titulo, editorial, precioBase, materia, Fecha(dia, mes, anio));
                break;
            case ESCOLAR:
                libro = new Escolar(isbn, titulo, editorial, precioBase, materia, Fecha(dia, mes, anio), anioEscuela);
                break;
        }
        libros.insertar(libro);
    }
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

void Fachada::listarPorPrefijo(string frase, TipoError& error)
{
    error = SIN_ERROR;
    Iterador resultado;
    libros.cargarPorPrefijo(frase, resultado);
    if (!resultado.hayActual())
        error = SIN_COINCIDENCIAS;
    else
        while (resultado.hayActual())
        {
            Libro* libro = (Libro*) resultado.actual();
            libro->imprimirBasico();
            resultado.avanzar();
        }
}

void Fachada::consultarLibro(long int isbn, TipoError& error)
{
    error = SIN_ERROR;
    if (!libros.existe(isbn))
        error = LIBRO_NO_EXISTE;
    else
        libros.buscar(isbn)->imprimir();
}

void Fachada::listarAutores(TipoError& error)
{
    error = SIN_ERROR;
    if (autores.estaVacia())
        error = SIN_AUTORES;
    else
    {
        Iterador resultado;
        autores.cargarIterador(resultado);
        while (resultado.hayActual())
        {
            Autor* autor = (Autor*) resultado.actual();
            autor->imprimir();
            resultado.avanzar();
        }
    }
}

void Fachada::registrarVenta(long int isbn, TipoError& error)
{
    error = SIN_ERROR;
    if (!libros.existe(isbn))
        error = LIBRO_NO_EXISTE;
    else
    {
        Libro* libro = libros.buscar(isbn);
        libro->registrarVenta();
        libro->imprimirPrecioVenta();
    }
}

void Fachada::calcularRecaudacion(string editorial)
{
    libros.imprimirRecaudacion(editorial);
}

void Fachada::contarVentasPorTipo(float precio)
{
    libros.imprimirVentasPorTipo(precio);
}

void Fachada::consultarMasVendido(string editorial, TipoError& error)
{
    error = SIN_ERROR;
    if (!libros.existeEditorial(editorial))
        error = SIN_LIBROS_DE_EDITORIAL;
    else
        libros.masVendido(editorial)->imprimir();
}
