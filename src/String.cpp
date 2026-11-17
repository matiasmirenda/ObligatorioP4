#include "String.h"
#include <string.h>

String::String()
{
    cadena = new char[1];
    cadena[0] = '\0';
}

String::String(const char *texto)
{
    int largo = strlen(texto);
    cadena = new char[largo + 1];
    strcpy(cadena, texto);
}

String::String(const String &otro)
{
    int largo = strlen(otro.cadena);
    cadena = new char[largo + 1];
    strcpy(cadena, otro.cadena);
}

String::~String()
{
    delete[] cadena;
}

String String::operator=(const String &otro)
{
    if (this != &otro)
    {
        int largo = strlen(otro.cadena);
        char *nueva = new char[largo + 1];
        strcpy(nueva, otro.cadena);
        delete[] cadena;
        cadena = nueva;
    }
    return *this;
}

bool String::operator==(String otro)
{
    return strcmp(cadena, otro.cadena) == 0;
}

bool String::comienzaCon(String prefijo)
{
    int i = 0;
    while (prefijo.cadena[i] != '\0')
    {
        if (cadena[i] != prefijo.cadena[i])
            return false;
        i++;
    }
    return true;
}
