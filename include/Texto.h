#ifndef TEXTO_H
#define TEXTO_H
#include "Libro.h"
#include "Fecha.h"
class Texto : public Libro
{
private:
    string materia;
    Fecha fechaPublicado;

public:
    void imprimir();
    Texto(long int, string, string, float, string, Fecha);
    ~Texto();
    string getMateria();
    Fecha getFechaPublicado();
    bool datosValidos();
    // Aplica el descuento si la publicacion es anterior a la fecha de corte.
    float calcularPrecioVenta();
    TipoLibro getTipo();
};
#endif
