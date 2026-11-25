#ifndef ESCOLAR_H
#define ESCOLAR_H
#include "Texto.h"
class Escolar : public Texto
{
private:
    int anioEscuela;

public:
    void imprimir();
    Escolar(long int, string, string, float, string, Fecha, int);
    ~Escolar();
    int getAnioEscuela();
    bool datosValidos();
    // Aplica el descuento de Texto y el adicional de primero a tercero.
    float calcularPrecioVenta();
    TipoLibro getTipo();
};
#endif
