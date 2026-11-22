#include <iostream>
#include "Parametros.h"
#include "Fecha.h"

using namespace std;

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
    if (anio >= ANIO_PUBLICACION_MINIMO && anio <= ANIO_PUBLICACION_MAXIMO)
    {
        switch (mes)
        {
        case ENERO:
        case MARZO:
        case MAYO:
        case JULIO:
        case AGOSTO:
        case OCTUBRE:
        case DICIEMBRE:
            valida = dia >= PRIMER_DIA_MES && dia <= DIAS_MES_LARGO;
            break;
        case ABRIL:
        case JUNIO:
        case SEPTIEMBRE:
        case NOVIEMBRE:
            valida = dia >= PRIMER_DIA_MES && dia <= DIAS_MES_CORTO;
            break;
        case FEBRERO:
            if (anio % CICLO_SIGLO_BISIESTO == 0 || (anio % CICLO_BISIESTO == 0 && anio % ANIOS_POR_SIGLO != 0))
                valida = dia >= PRIMER_DIA_MES && dia <= DIAS_FEBRERO_BISIESTO;
            else
                valida = dia >= PRIMER_DIA_MES && dia <= DIAS_FEBRERO_COMUN;
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

void Fecha::imprimir()
{
    cout << dia << '/' << mes << '/' << anio;
}
