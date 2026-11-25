#ifndef MENU_H
#define MENU_H

#include <string>
class Fachada;

// Lee una linea acotada; descarta el exceso y conserva en cin el estado de fin de entrada.
void leerLinea(std::string&);
// Solicita texto con contenido y longitud permitida hasta obtenerlo o finalizar la entrada.
void leerTexto(std::string, std::string&);
// Comprueba que la cadena contenga entre uno y nueve digitos.
bool esEntero(std::string);
// PRECONDICION:
// 1- esEntero(linea) devuelve verdadero.
long int convertirEntero(std::string);
// Solicita un entero en el intervalo indicado, o cancela ante fin de entrada.
void leerEntero(std::string, long int, long int, long int&);
// Solicita la cantidad exacta de digitos sin separadores.
void leerIdentificador(std::string, int, long int&);
// Comprueba el formato decimal no negativo; deja cero y falso si no es valido.
void analizarPrecio(std::string, float&, bool&);
// Comprueba formato decimal no negativo.
bool esPrecio(std::string);
// Convierte el decimal validado, o devuelve cero si no es valido.
float convertirPrecio(std::string);
// Solicita un precio decimal no negativo, o cancela ante fin de entrada.
void leerPrecio(std::string, float&);
void altaAutor(Fachada&);
void altaLibro(Fachada&);
// PRECONDICION:
// 1- La opcion esta entre 1 y 10.
void ejecutarOpcion(long int, Fachada&);
// Muestra las opciones y ejecuta requerimientos hasta elegir salir o finalizar la entrada.
void ejecutarMenu();

#endif
