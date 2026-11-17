#include "Libro.h"
Libro::Libro(long long cod, String tit, String ed, double precio)
    : titulo(tit), editorial(ed)
{
    isbn = cod;
    precioBase = precio;
    unidadesVendidas = 0;
}
Libro::Libro(const Libro& otro) : titulo(otro.titulo), editorial(otro.editorial)
{
    isbn = otro.isbn;
    precioBase = otro.precioBase;
    unidadesVendidas = otro.unidadesVendidas;
}
Libro::~Libro() {}
long long Libro::getIsbn() { return isbn; }
String Libro::getTitulo() { return titulo; }
String Libro::getEditorial() { return editorial; }
double Libro::getPrecioBase() { return precioBase; }
int Libro::getUnidadesVendidas() { return unidadesVendidas; }
void Libro::setTitulo(String nuevo) { titulo = nuevo; }
void Libro::setEditorial(String nueva) { editorial = nueva; }
void Libro::setPrecioBase(double nuevo) { precioBase = nuevo; }
void Libro::registrarVenta() { unidadesVendidas++; }
bool Libro::comienzaCon(String frase)
{
    return titulo.comienzaCon(frase);
}
bool Libro::datosValidos()
{
    return isbn > 0 && !(titulo == "") && !(editorial == "") && precioBase >= 0;
}
