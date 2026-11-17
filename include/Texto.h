#ifndef TEXTO_H
#define TEXTO_H
#include "Libro.h"
#include "Fecha.h"
class Texto : public Libro
{
private:
    String materia;
    Fecha fechaPublicado;
public:
    Texto(long long, String, String, double, String, Fecha);
    Texto(const Texto&);
    ~Texto();
    String getMateria();
    Fecha getFechaPublicado();
    // PRECONDICION:
    // 1- La materia no esta vacia.
    void setMateria(String);
    // PRECONDICION:
    // 1- La nueva fecha es valida.
    void setFechaPublicado(Fecha);
    bool datosValidos();
    double calcularPrecioVenta();
    String getTipo();
};
#endif
