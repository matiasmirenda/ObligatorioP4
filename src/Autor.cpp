#include "Autor.h"

Autor::Autor(long int ci, string nom, string ape)
    : nombre(nom), apellido(ape)
{
    cedula = ci;
}

long int Autor::getId()
{
    return (cedula);
}

string Autor::getNombre()
{
    return (nombre);
}

string Autor::getApellido()
{
    return (apellido);
}

Autor::~Autor()
{
}
