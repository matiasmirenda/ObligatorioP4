#include "ColeccionLibros.h"

// El miembro tabla se construye automaticamente antes del cuerpo.
ColeccionLibros::ColeccionLibros() {}

// El destructor del miembro tabla libera los objetos y nodos una sola vez.
ColeccionLibros::~ColeccionLibros() {}

bool ColeccionLibros::estaVacia() { return tabla.estaVacia(); }
int ColeccionLibros::getCantidad() { return tabla.getCantidad(); }
bool ColeccionLibros::existe(long long isbn) { return tabla.existe(isbn); }
Libro* ColeccionLibros::buscar(long long isbn) { return tabla.buscar(isbn); }
void ColeccionLibros::insertar(Libro* libro) { tabla.insertar(libro); }
IteradorLibros* ColeccionLibros::crearIterador() { return tabla.crearIterador(); }
