#ifndef ESCOLAR_H
#define ESCOLAR_H
#include "Texto.h"
class Escolar : public Texto
{
private:
    const string tipo;
    int anioEscuela;

public:
    Escolar(long int, string, string, double, string, Fecha, int);
    ~Escolar();
    int getAnioEscuela();
    bool datosValidos();
    double calcularPrecioVenta();
    string getTipo();
};
#endif
