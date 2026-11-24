#ifndef FECHA_H
#define FECHA_H

class Fecha
{
private:
    int dia;
    int mes;
    int anio;

public:
    void imprimir();
    Fecha(int, int, int);
    int getDia();
    int getMes();
    int getAnio();

    // Comprueba el calendario y el rango de anios 1900..2099.
    bool esValida();

    // PRECONDICION:
    // 1- Ambas fechas son validas.
    bool operator<(Fecha);
};

#endif
