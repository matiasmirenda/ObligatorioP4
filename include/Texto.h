#ifndef TEXTO_H
#define TEXTO_H
#include "Libro.h"
#include "Fecha.h"
class Texto : public Libro
{
private:
    string materia;
    Fecha fechaPublicado; // Valor propio del texto, sin new ni delete.

public:
    Texto(long int, string, string, double, string, Fecha);
    ~Texto();
    string getMateria();
    Fecha getFechaPublicado();
    bool datosValidos();
    double calcularPrecioVenta();
    TipoLibro getTipo();
};
#endif
