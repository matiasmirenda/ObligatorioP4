#ifndef AUTOR_H
#define AUTOR_H

#include <string>
#include "Objeto.h"

using std::string;

class Autor : public Objeto
{
private:
    long int cedula;
    string nombre;
    string apellido;

public:
    void imprimir();
    Autor(long int, string, string);
    ~Autor();

    long int getId();
    string getNombre();
    string getApellido();
};

#endif
