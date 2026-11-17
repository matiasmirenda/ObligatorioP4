#include "Autor.h"

Autor::Autor()
{
    cedula = 0;
}


Autor::Autor(long ci, String nom, String ape)
    : nombre(nom), apellido(ape)
{
    cedula = ci;
}

Autor::Autor(const Autor& otro)
    : nombre(otro.nombre), apellido(otro.apellido)
{
    cedula = otro.cedula;
}

long Autor::getCedula() {
    return cedula;
}

String Autor::getNombre() {
    return nombre;
}

String Autor::getApellido() {
    return apellido;
}

void Autor::setNombre(String nuevoNombre) {
    nombre = nuevoNombre;
}

void Autor::setApellido(String nuevoApellido) {
    apellido = nuevoApellido;
}

bool Autor::datosValidos() {
    return cedula > 0 && !(nombre == "") && !(apellido == "");
}

Autor Autor::operator=(const Autor& otro)
{
    cedula = otro.cedula;
    nombre = otro.nombre;
    apellido = otro.apellido;
    return *this;
}

Autor::~Autor()
{
}
