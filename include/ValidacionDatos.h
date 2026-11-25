#ifndef VALIDACION_DATOS_H
#define VALIDACION_DATOS_H
#include <string>

// Comprueba longitud permitida y al menos un caracter que no sea espacio, tabulacion o retorno.
bool textoValido(std::string);

// Devuelve la cantidad de digitos decimales de un entero positivo; cero para no positivos.
int cantidadDigitos(long int);

#endif
