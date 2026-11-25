#include "ValidacionDatos.h"
#include "Parametros.h"

using namespace std;

bool textoValido(string texto)
{
    bool contenido = false;
    unsigned int i = 0;
    while (i < texto.size() && !contenido)
    {
        contenido = texto[i] != ' ' && texto[i] != '\t' && texto[i] != '\r';
        i++;
    }
    return (contenido && texto.size() <= MAX_LARGO_TEXTO);
}

int cantidadDigitos(long int numero)
{
    int cantidad = 0;
    while (numero > 0)
    {
        cantidad++;
        numero = numero / BASE_DECIMAL;
    }
    return (cantidad);
}
