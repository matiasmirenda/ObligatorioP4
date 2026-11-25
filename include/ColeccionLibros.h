#ifndef COLECCION_LIBROS_H
#define COLECCION_LIBROS_H

#include "Libro.h"
#include "Iterador.h"

const int CUBETAS_LIBROS = 500;

using std::string;

class ColeccionLibros
{
private:
    struct Nodo
    {
        Libro *info;
        Nodo *siguiente;
    };
    Nodo* cubetas[CUBETAS_LIBROS];
    // Calcula la cubeta para un ISBN positivo.
    // PRECONDICION:
    // 1- El ISBN es positivo.
    int h(long int);
    void crearLista(Nodo*&);
    // Libera todos los nodos y libros de la lista y la deja vacia.
    void destruirLista(Nodo*&);
    bool perteneceLista(Nodo*, long int);
    // Inserta al principio y adquiere la propiedad del libro.
    void insFrontEnLista(Nodo*&, Libro*);
    // Devuelve el libro prestado correspondiente al ISBN.
    // PRECONDICION:
    // 1- La clave existe en la lista.
    Libro* obtenerEnLista(Nodo*, long int);

public:
    ColeccionLibros();
    ~ColeccionLibros();

    void imprimirRecaudacion(string);
    void imprimirVentasPorTipo(float);
    bool estaVacia();
    bool existe(long int);

    // Devuelve un puntero prestado al objeto de la clave recibida.
    // PRECONDICION:
    // 1- La clave existe en la coleccion.
    Libro* buscar(long int);

    // Agrega el objeto; la estructura adquiere su propiedad.
    // PRECONDICION:
    // 1- El puntero no es NULL y los datos del objeto son validos.
    // 2- No existe otro objeto con la misma clave.
    // 3- El llamador transfiere la propiedad del objeto a la estructura.
    void insertar(Libro*);

    // Carga referencias a los libros, sin orden particular.
    // PRECONDICION:
    // 1- El iterador esta vacio y se utiliza mientras exista la coleccion.
    void cargarIterador(Iterador&);

    // Carga referencias a los libros cuyo titulo comienza con la frase exacta.
    // PRECONDICION:
    // 1- El iterador esta vacio y se utiliza mientras exista la coleccion.
    void cargarPorPrefijo(string, Iterador&);

    bool existeEditorial(string);
    // Suma las ventas de la editorial multiplicadas por el precio de venta de cada libro.
    float calcularRecaudacion(string);
    // Inicializa y carga los tres totales de unidades con precio base estrictamente mayor.
    void contarVentasPorTipo(float, long int&, long int&, long int&);

    // Devuelve un libro prestado de la editorial con ventas maximas; admite cualquier empate.
    // PRECONDICION:
    // 1- Existe al menos un libro de la editorial.
    Libro* masVendido(string);
};

#endif
