#ifndef AUTOR_H
#define AUTOR_H

#include "String.h"

class Autor
{
private:
    long cedula;
    String nombre;
    String apellido;

public:
    // Guarda los datos; comprobar datosValidos() antes de insertar en la coleccion.
    Autor();
    Autor(long, String, String);
    Autor(const Autor &);
    ~Autor();
    Autor operator=(const Autor &);

    long getCedula();
    String getNombre();
    String getApellido();

    // PRECONDICION:
    // 1- El nuevo nombre no esta vacio.
    void setNombre(String);
    // PRECONDICION:
    // 1- El nuevo apellido no esta vacio.
    void setApellido(String);

    // Consulta si la cedula es positiva y ambos textos no estan vacios.
    bool datosValidos();
};

#endif
