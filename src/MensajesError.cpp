#include "MensajesError.h"

string mensajeError(TipoError error)
{
    string mensaje;
    switch (error)
    {
        case DATOS_INVALIDOS:
            mensaje = "Los datos ingresados no son validos.";
            break;
        case LIMITE_VENTAS:
            mensaje = "No se pueden registrar mas ventas para este libro.";
            break;
        case SIN_ERROR:
            mensaje = "Sin error.";
            break;
        case AUTOR_YA_EXISTE:
            mensaje = "Ya existe un autor con esa cedula.";
            break;
        case LIBRO_YA_EXISTE:
            mensaje = "Ya existe un libro con ese ISBN.";
            break;
        case LIBRO_NO_EXISTE:
            mensaje = "No existe un libro con ese ISBN.";
            break;
        case AUTOR_NO_EXISTE:
            mensaje = "No existe un autor con esa cedula.";
            break;
        case LIBRO_NO_ES_NOVELA:
            mensaje = "El libro indicado no es una novela.";
            break;
        case SIN_COINCIDENCIAS:
            mensaje = "Ningun titulo comienza con esa frase.";
            break;
        case SIN_AUTORES:
            mensaje = "No hay autores registrados.";
            break;
        case SIN_LIBROS_DE_EDITORIAL:
            mensaje = "No hay libros registrados de esa editorial.";
            break;
        default:
            mensaje = "Error desconocido.";
            break;
    }
    return (mensaje);
}
