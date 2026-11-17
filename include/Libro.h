#ifndef LIBRO_H
#define LIBRO_H
#include "Str.h"

class Libro
{
private:
    long long isbn;
    String titulo;
    String editorial;
    double precioBase;
    int unidadesVendidas;
public:
    // Guarda los datos; comprobar datosValidos() antes de insertar.
    Libro(long long, String, String, double);
    Libro(const Libro&);
    virtual ~Libro();
    long long getIsbn();
    String getTitulo();
    String getEditorial();
    double getPrecioBase();
    int getUnidadesVendidas();
    // PRECONDICION:
    // 1- El nuevo titulo no esta vacio.
    void setTitulo(String);
    // PRECONDICION:
    // 1- La nueva editorial no esta vacia.
    void setEditorial(String);
    // PRECONDICION:
    // 1- El nuevo precio es mayor o igual a cero.
    void setPrecioBase(double);
    // PRECONDICION:
    // 1- Las unidades vendidas son menores que el maximo representable por int.
    void registrarVenta();
    bool comienzaCon(String);
    virtual bool datosValidos();
    // PRECONDICION:
    // 1- Los datos del libro son validos.
    virtual double calcularPrecioVenta() = 0;
    virtual String getTipo() = 0;
};
#endif
