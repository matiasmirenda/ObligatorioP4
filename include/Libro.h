#ifndef LIBRO_H
#define LIBRO_H

#include <string>
#include "Objeto.h"
#include "TipoLibro.h"
using std::string;

class Libro : public Objeto
{
private:
    void imprimirTipo();
    long int isbn;
    string titulo;
    string editorial;
    float precioBase;
    int unidadesVendidas;

public:
    Libro(long int, string, string, float);
    virtual ~Libro();
    long int getId();
    string getTitulo();
    string getEditorial();
    float getPrecioBase();
    int getUnidadesVendidas();

    virtual void imprimir();
    void imprimirBasico();
    void imprimirPrecioVenta();
    void registrarVenta();
    // Indica si el titulo comienza exactamente con la frase, distinguiendo mayusculas.
    bool comienzaCon(string);
    // Comprueba ISBN positivo, titulo y editorial con contenido y longitud permitida, y precio no negativo.
    virtual bool datosValidos();
    // Devuelve el precio de la unidad aplicando los descuentos de su tipo.
    // PRECONDICION:
    // 1- Los datos del libro son validos.
    virtual float calcularPrecioVenta() = 0;
    virtual TipoLibro getTipo() = 0;
};
#endif
