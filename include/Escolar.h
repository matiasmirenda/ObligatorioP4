#ifndef ESCOLAR_H
#define ESCOLAR_H
#include "Texto.h"
class Escolar : public Texto
{
private:
    int anioEscuela;
public:
    Escolar(long long, String, String, double, String, Fecha, int);
    Escolar(const Escolar&);
    ~Escolar();
    int getAnioEscuela();
    // PRECONDICION:
    // 1- El anio escolar esta entre 1 y 6.
    void setAnioEscuela(int);
    bool datosValidos();
    double calcularPrecioVenta();
    String getTipo();
};
#endif
