#ifndef LIBRO_H
#define LIBRO_H

#include <string>
#include "Objeto.h"
using std::string;

class Libro : public Objeto
{
private:
    long int isbn;
    string titulo;
    string editorial;
    double precioBase;
    int unidadesVendidas;

public:
    // Guarda los datos; comprobar datosValidos() antes de insertar.
    Libro(long int, string, string, double);
    virtual ~Libro();
    long int getId();
    string getTitulo();
    string getEditorial();
    double getPrecioBase();
    int getUnidadesVendidas();

    // PRECONDICION:
    // 1- Las unidades vendidas son menores que el maximo representable por int.
    void registrarVenta();
    bool comienzaCon(string);
    virtual bool datosValidos();
    // PRECONDICION:
    // 1- Los datos del libro son validos.
    virtual double calcularPrecioVenta() = 0;
    virtual string getTipo() = 0;
};
#endif
