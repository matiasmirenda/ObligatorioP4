#ifndef MENSAJES_ERROR_H
#define MENSAJES_ERROR_H

#include <string>
#include "TipoError.h"

using std::string;

string mensajeError(TipoError);

void mostrarResultado(TipoError);

#endif
