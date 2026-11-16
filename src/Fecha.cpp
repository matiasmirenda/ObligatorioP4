#include "Fecha.h"

Fecha::Fecha(int dia, int mes, int anio) : dia(dia), mes(mes), anio(anio) {}

int Fecha::getDia() const { return dia; }
int Fecha::getMes() const { return mes; }
int Fecha::getAnio() const { return anio; }

bool Fecha::esValida(int dia, int mes, int anio) {
    if (anio < 1 || anio > 9999 || mes < 1 || mes > 12 || dia < 1) {
        return false;
    }
    int diasDelMes = 31;
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        diasDelMes = 30;
    } else if (mes == 2) {
        bool bisiesto = anio % 400 == 0 || (anio % 4 == 0 && anio % 100 != 0);
        diasDelMes = bisiesto ? 29 : 28;
    }
    return dia <= diasDelMes;
}

bool Fecha::esMenor(const Fecha& otra) const {
    if (anio != otra.anio) {
        return anio < otra.anio;
    }
    if (mes != otra.mes) {
        return mes < otra.mes;
    }
    return dia < otra.dia;
}

bool Fecha::esMayor(const Fecha& otra) const {
    return otra.esMenor(*this);
}

bool Fecha::esIgual(const Fecha& otra) const {
    return dia == otra.dia && mes == otra.mes && anio == otra.anio;
}
