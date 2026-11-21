#ifndef ESCOLAR_H
#define ESCOLAR_H
#include "Texto.h"
class Escolar : public Texto
{
private:
    int anioEscuela;

public:
    Escolar(long int, string, string, double, string, Fecha, int);
    ~Escolar();
    int getAnioEscuela();
    bool datosValidos();
    double calcularPrecioVenta();
    TipoLibro getTipo();
};
#endif
