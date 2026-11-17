#include "ColeccionAutores.h"

// El miembro arbol se construye automaticamente antes del cuerpo.
ColeccionAutores::ColeccionAutores() {}

// El destructor del miembro arbol libera los objetos y nodos una sola vez.
ColeccionAutores::~ColeccionAutores() {}

bool ColeccionAutores::estaVacia() { return arbol.estaVacia(); }
int ColeccionAutores::getCantidad() { return arbol.getCantidad(); }
bool ColeccionAutores::existe(long cedula) { return arbol.existe(cedula); }
Autor* ColeccionAutores::buscar(long cedula) { return arbol.buscar(cedula); }
void ColeccionAutores::insertar(Autor* autor) { arbol.insertar(autor); }
IteradorAutores* ColeccionAutores::crearIterador() { return arbol.crearIterador(); }
