#ifndef MENSAJES_ERROR_H
#define MENSAJES_ERROR_H

#include <string>
#include "TipoError.h"

using std::string;

// Traduce el resultado de la logica para mostrarlo en la presentacion.
string mensajeError(TipoError);

#endif
