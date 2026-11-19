#include "Fachada.h"
#include "Novela.h"
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

void Fachada::registrarLibro(Libro* libro, TipoError& error)
{
    error = SIN_ERROR;
    if (libro == NULL || !libro->datosValidos() || libro->getUnidadesVendidas() != 0)
        error = DATOS_INVALIDOS;
    else if (libros.existe(libro->getId()))
        error = LIBRO_YA_EXISTE;
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
        if (libro->getTipo() != "Novela")
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
    Iterador recorrido;
    libros.cargarIterador(recorrido);
    while (recorrido.hayActual())
    {
        Libro* libro = (Libro*) recorrido.actual();
        if (libro->comienzaCon(frase))
            resultado.insertar(libro);
        recorrido.avanzar();
    }
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
    double total = 0;
    Iterador recorrido;
    libros.cargarIterador(recorrido);
    while (recorrido.hayActual())
    {
        Libro* libro = (Libro*) recorrido.actual();
        if (libro->getEditorial() == editorial)
            total += libro->getUnidadesVendidas() * libro->calcularPrecioVenta();
        recorrido.avanzar();
    }
    return (total);
}

void Fachada::contarVentasPorTipo(double precio, long int& novelas, long int& textos, long int& escolares)
{
    novelas = 0;
    textos = 0;
    escolares = 0;
    Iterador recorrido;
    libros.cargarIterador(recorrido);
    while (recorrido.hayActual())
    {
        Libro* libro = (Libro*) recorrido.actual();
        if (libro->getPrecioBase() > precio)
        {
            if (libro->getTipo() == "Novela")
                novelas += libro->getUnidadesVendidas();
            else if (libro->getTipo() == "Texto")
                textos += libro->getUnidadesVendidas();
            else
                escolares += libro->getUnidadesVendidas();
        }
        recorrido.avanzar();
    }
}

void Fachada::consultarMasVendido(string editorial, Libro*& resultado, TipoError& error)
{
    error = SIN_ERROR;
    resultado = NULL;
    Iterador recorrido;
    libros.cargarIterador(recorrido);
    while (recorrido.hayActual())
    {
        Libro* libro = (Libro*) recorrido.actual();
        if (libro->getEditorial() == editorial)
        {
            if (resultado == NULL || libro->getUnidadesVendidas() > resultado->getUnidadesVendidas())
                resultado = libro;
        }
        recorrido.avanzar();
    }
    if (resultado == NULL)
        error = SIN_LIBROS_DE_EDITORIAL;
}
