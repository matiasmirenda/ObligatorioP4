#include "Fecha.h"

Fecha::Fecha(int d, int m, int a)
{
    dia = d;
    mes = m;
    anio = a;
}

int Fecha::getDia() { return (dia); }
int Fecha::getMes() { return (mes); }
int Fecha::getAnio() { return (anio); }

bool Fecha::esValida()
{
    bool valida = false;
    if (anio >= 1900 && anio <= 2099)
    {
        switch (mes)
        {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            valida = dia >= 1 && dia <= 31;
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            valida = dia >= 1 && dia <= 30;
            break;
        case 2:
            if (anio % 400 == 0 || (anio % 4 == 0 && anio % 100 != 0))
                valida = dia >= 1 && dia <= 29;
            else
                valida = dia >= 1 && dia <= 28;
            break;
        default:
            valida = false;
        }
    }
    return (valida);
}

bool Fecha::operator<(Fecha otra)
{
    bool menor;
    if (anio != otra.anio)
        menor = anio < otra.anio;
    else if (mes != otra.mes)
        menor = mes < otra.mes;
    else
        menor = dia < otra.dia;
    return (menor);
}
