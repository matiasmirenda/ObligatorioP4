#include "Menu.h"
#include "Fachada.h"
#include "MensajesError.h"
#include "TipoLibro.h"
#include "Novela.h"
#include "Texto.h"
#include "Escolar.h"
#include <iostream>
#include <string>
#include <cstddef>

using namespace std;

// Se aceptan hasta 9 digitos: cualquier numero de 9 cifras entra en un
// long int (en Windows el maximo es 2147483647), asi se evita el desbordamiento.
const int MAX_DIGITOS = 9;
const long int MAX_ENTERO = 999999999;
// Limite de largo para los textos ingresados (titulo, nombre, editorial, etc.).
const int MAX_LARGO_TEXTO = 100;

// Lee la linea completa hasta el salto de linea, como scan() del Practico 5.
void leerLinea(string& linea)
{
    linea = "";
    char caracter = cin.get();
    while (caracter != '\n')
    {
        linea += caracter;
        caracter = cin.get();
    }
}

void leerTexto(string mensaje, string& valor)
{
    cout << mensaje;
    leerLinea(valor);
    while (valor == "" || (int) valor.size() > MAX_LARGO_TEXTO)
    {
        cout << "Ingrese un texto de 1 a " << MAX_LARGO_TEXTO << " caracteres: ";
        leerLinea(valor);
    }
}

bool esEntero(string linea)
{
    int largo = linea.size();
    bool valido = largo > 0 && largo <= MAX_DIGITOS;
    int i = 0;
    while (i < largo && valido)
    {
        if (linea[i] < '0' || linea[i] > '9')
            valido = false;
        i++;
    }
    return (valido);
}

// PRECONDICION:
// 1- La linea contiene solo digitos.
long int convertirEntero(string linea)
{
    long int numero = 0;
    int largo = linea.size();
    for (int i = 0; i < largo; i++)
        numero = numero * 10 + (linea[i] - '0');
    return (numero);
}

void leerEntero(string mensaje, long int minimo, long int maximo, long int& valor)
{
    string linea;
    bool valido = false;
    cout << mensaje;
    while (!valido)
    {
        leerLinea(linea);
        valido = esEntero(linea);
        if (valido)
        {
            valor = convertirEntero(linea);
            valido = valor >= minimo && valor <= maximo;
        }
        if (!valido)
            cout << "Ingrese un entero entre " << minimo << " y " << maximo << ": ";
    }
}

bool esPrecio(string linea)
{
    int largo = linea.size();
    bool valido = largo > 0;
    bool hayPunto = false;
    bool hayDigito = false;
    int i = 0;
    while (i < largo && valido)
    {
        if (linea[i] == '.' && !hayPunto)
            hayPunto = true;
        else if (linea[i] >= '0' && linea[i] <= '9')
            hayDigito = true;
        else
            valido = false;
        i++;
    }
    return (valido && hayDigito);
}

// PRECONDICION:
// 1- La linea contiene digitos y como maximo un punto.
double convertirPrecio(string linea)
{
    double numero = 0;
    double decimal = 0.1;
    bool hayPunto = false;
    int largo = linea.size();
    for (int i = 0; i < largo; i++)
    {
        if (linea[i] == '.')
            hayPunto = true;
        else if (!hayPunto)
            numero = numero * 10 + (linea[i] - '0');
        else
        {
            numero = numero + (linea[i] - '0') * decimal;
            decimal = decimal / 10;
        }
    }
    return (numero);
}

void leerPrecio(string mensaje, double& valor)
{
    string linea;
    cout << mensaje;
    leerLinea(linea);
    while (!esPrecio(linea))
    {
        cout << "Ingrese un precio valido (use punto para decimales): ";
        leerLinea(linea);
    }
    valor = convertirPrecio(linea);
}

string nombreTipo(TipoLibro tipo)
{
    string nombre;
    switch (tipo)
    {
        case NOVELA:
            nombre = "Novela";
            break;
        case TEXTO:
            nombre = "Texto";
            break;
        case ESCOLAR:
            nombre = "Escolar";
            break;
    }
    return (nombre);
}

void mostrarResultado(TipoError error)
{
    if (error == SIN_ERROR)
        cout << "Operacion realizada." << endl;
    else
        cout << mensajeError(error) << endl;
}

// PRECONDICION:
// 1- El puntero refiere a un libro registrado y vivo.
void mostrarLibro(Libro* libro)
{
    cout << "ISBN: " << libro->getId() << "\nTitulo: " << libro->getTitulo()
         << "\nEditorial: " << libro->getEditorial() << "\nTipo: " << nombreTipo(libro->getTipo())
         << "\nPrecio base: " << libro->getPrecioBase()
         << "\nUnidades vendidas: " << libro->getUnidadesVendidas() << endl;
    if (libro->getTipo() == NOVELA)
    {
        Novela* novela = (Novela*) libro;
        cout << "Genero: " << novela->getGenero() << endl;
        Autor* autor = novela->getAutor();
        if (autor != NULL)
            cout << "Autor: " << autor->getId() << " | " << autor->getNombre()
                 << " | " << autor->getApellido() << endl;
        else
            cout << "Sin autor asociado." << endl;
    }
    else
    {
        Texto* texto = (Texto*) libro;
        Fecha fecha = texto->getFechaPublicado();
        cout << "Materia: " << texto->getMateria() << "\nPublicacion: "
             << fecha.getDia() << '/' << fecha.getMes() << '/' << fecha.getAnio() << endl;
        if (libro->getTipo() == ESCOLAR)
            cout << "Anio escolar: " << ((Escolar*) libro)->getAnioEscuela() << endl;
    }
}

void altaAutor(Fachada& fachada)
{
    long int cedula;
    string nombre, apellido;
    TipoError error;
    leerEntero("Cedula: ", 1, MAX_ENTERO, cedula);
    leerTexto("Nombre: ", nombre);
    leerTexto("Apellido: ", apellido);
    fachada.registrarAutor(cedula, nombre, apellido, error);
    mostrarResultado(error);
}

void altaLibro(Fachada& fachada)
{
    long int opcionTipo, isbn, dia = 0, mes = 0, anio = 0, anioEscuela = 0;
    double precio;
    string titulo, editorial, genero, materia;
    TipoLibro tipo;
    TipoError error;
    leerEntero("Tipo (1 Novela, 2 Texto, 3 Escolar): ", 1, 3, opcionTipo);
    leerEntero("ISBN: ", 1, MAX_ENTERO, isbn);
    leerTexto("Titulo: ", titulo);
    leerTexto("Editorial: ", editorial);
    leerPrecio("Precio base: ", precio);
    if (opcionTipo == 1)
    {
        tipo = NOVELA;
        leerTexto("Genero (infantil para el descuento): ", genero);
    }
    else
    {
        leerTexto("Materia: ", materia);
        leerEntero("Dia: ", 1, 31, dia);
        leerEntero("Mes: ", 1, 12, mes);
        leerEntero("Anio (1900 a 2099): ", 1900, 2099, anio);
        if (opcionTipo == 2)
            tipo = TEXTO;
        else
        {
            tipo = ESCOLAR;
            leerEntero("Anio escolar (1 a 6): ", 1, 6, anioEscuela);
        }
    }
    fachada.registrarLibro(tipo, isbn, titulo, editorial, precio, genero, materia,
                           dia, mes, anio, anioEscuela, error);
    mostrarResultado(error);
}

void ejecutarOpcion(long int opcion, Fachada& fachada)
{
    TipoError error;
    long int isbn, cedula;
    string texto;
    double importe;
    Libro* libro;
    switch (opcion)
    {
        case 1:
            altaAutor(fachada);
            break;
        case 2:
            altaLibro(fachada);
            break;
        case 3:
            leerEntero("ISBN de la novela: ", 1, MAX_ENTERO, isbn);
            leerEntero("Cedula del autor: ", 1, MAX_ENTERO, cedula);
            fachada.asociarAutor(isbn, cedula, error);
            mostrarResultado(error);
            break;
        case 4:
        {
            leerTexto("Comienzo exacto del titulo: ", texto);
            Iterador iter;
            fachada.listarPorPrefijo(texto, iter, error);
            if (error != SIN_ERROR)
                mostrarResultado(error);
            else
                while (iter.hayActual())
                {
                    libro = (Libro*) iter.actual();
                    cout << libro->getId() << " | " << libro->getTitulo() << " | " << nombreTipo(libro->getTipo()) << endl;
                    iter.avanzar();
                }
            break;
        }
        case 5:
            leerEntero("ISBN: ", 1, MAX_ENTERO, isbn);
            fachada.consultarLibro(isbn, libro, error);
            if (error == SIN_ERROR)
                mostrarLibro(libro);
            else
                mostrarResultado(error);
            break;
        case 6:
        {
            Iterador iter;
            fachada.listarAutores(iter, error);
            if (error != SIN_ERROR)
                mostrarResultado(error);
            else
                while (iter.hayActual())
                {
                    Autor* autor = (Autor*) iter.actual();
                    cout << autor->getId() << " | " << autor->getNombre() << " | " << autor->getApellido() << endl;
                    iter.avanzar();
                }
            break;
        }
        case 7:
            leerEntero("ISBN vendido: ", 1, MAX_ENTERO, isbn);
            fachada.registrarVenta(isbn, importe, error);
            if (error == SIN_ERROR)
                cout << "Importe: " << importe << endl;
            else
                mostrarResultado(error);
            break;
        case 8:
            leerTexto("Editorial: ", texto);
            cout << "Recaudacion: " << fachada.calcularRecaudacion(texto) << endl;
            break;
        case 9:
        {
            long int novelas, textos, escolares;
            leerPrecio("Precio base de referencia: ", importe);
            fachada.contarVentasPorTipo(importe, novelas, textos, escolares);
            cout << "Unidades vendidas: novelas " << novelas << ", textos comunes " << textos
                 << ", escolares " << escolares << endl;
            break;
        }
        case 10:
            leerTexto("Editorial: ", texto);
            fachada.consultarMasVendido(texto, libro, error);
            if (error == SIN_ERROR)
                mostrarLibro(libro);
            else
                mostrarResultado(error);
            break;
    }
}

void ejecutarMenu()
{
    Fachada fachada;
    long int opcion;
    bool continuar = true;
    while (continuar)
    {
        cout << "\nLIBRERIA\n1. Registrar autor\n2. Registrar libro\n3. Asociar autor a novela"
             << "\n4. Listar libros por comienzo del titulo\n5. Consultar libro\n6. Listar autores"
             << "\n7. Registrar venta\n8. Recaudacion por editorial\n9. Ventas por tipo y precio base"
             << "\n10. Libro mas vendido de una editorial\n0. Salir\n";
        leerEntero("Opcion: ", 0, 10, opcion);
        if (opcion == 0)
            continuar = false;
        else
            ejecutarOpcion(opcion, fachada);
    }
}
