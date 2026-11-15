#ifndef FECHA_H
#define FECHA_H

class Fecha {
private:
    int dia;
    int mes;
    int anio;

public:
    // PRECONDICION:
    // 1- Los valores deben formar una fecha valida (anio entre 1 y 9999).
    Fecha(int dia, int mes, int anio);

    // PRECONDICION: Ninguna.
    int getDia() const;
    // PRECONDICION: Ninguna.
    int getMes() const;
    // PRECONDICION: Ninguna.
    int getAnio() const;

    // Comprueba los dias de cada mes y los anios bisiestos.
    // PRECONDICION: Ninguna.
    static bool esValida(int dia, int mes, int anio);

    // PRECONDICION:
    // 1- Ambas fechas deben ser validas.
    bool esMenor(const Fecha& otra) const;
    // PRECONDICION:
    // 1- Ambas fechas deben ser validas.
    bool esMayor(const Fecha& otra) const;
    // PRECONDICION:
    // 1- Ambas fechas deben ser validas.
    bool esIgual(const Fecha& otra) const;
};

#endif
