#ifndef FECHA_H
#define FECHA_H

class Fecha
{
private:
    int dia;
    int mes;
    int anio;

public:
    Fecha();
    // Guarda los componentes; comprobar esValida() antes de comparar.
    Fecha(int, int, int);
    Fecha(const Fecha&);
    Fecha operator=(const Fecha&);

    int getDia();
    int getMes();
    int getAnio();

    // Valida el calendario y limita el anio a 1900..2099.
    bool esValida();

    // PRECONDICION:
    // 1- Ambas fechas deben ser validas.
    bool operator<(Fecha);
    // PRECONDICION:
    // 1- Ambas fechas deben ser validas.
    bool operator>(Fecha);
    // PRECONDICION:
    // 1- Ambas fechas deben ser validas.
    bool operator==(Fecha);
};

#endif
