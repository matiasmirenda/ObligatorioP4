#include "Menu.h"
#include "Fachada.h"
#include "MensajesError.h"
#include "Novela.h"
#include "Texto.h"
#include "Escolar.h"
#include <iostream>
#include <cfloat>
#include <string>
#include <cstddef>
#include <climits>

using namespace std;

// Lee hasta el salto de linea, como String::scan() del Practico 5.
bool leerLinea(string mensaje, string& valor)
{
    cout << mensaje;
    valor = "";
    int caracter = cin.get();
    bool leida = !cin.eof();
    while (caracter != '\n' && !cin.eof())
    {
        valor += (char) caracter;
        caracter = cin.get();
    }
    return (leida);
}

bool esEspacio(char caracter)
{
    return (caracter == ' ' || caracter == '\t' || caracter == '\r');
}

bool leerTexto(string mensaje, string& valor)
{
    bool leido = false;
    bool hayEntrada = true;
    while (!leido && hayEntrada)
    {
        hayEntrada = leerLinea(mensaje, valor);
        long int i = 0;
        while (i < (long int) valor.size() && !leido)
        {
            leido = !esEspacio(valor[i]);
            i++;
        }
        if (hayEntrada && !leido)
            cout << "Ingrese un texto no vacio." << endl;
    }
    return (leido);
}

bool leerEntero(string mensaje, long int minimo, long int maximo, long int& valor)
{
    bool valido = false;
    bool hayEntrada = true;
    while (!valido && hayEntrada)
    {
        string linea;
        hayEntrada = leerLinea(mensaje, linea);
        long int i = 0;
        long int numero = 0;
        bool hayDigitos = false;
        bool correcto = hayEntrada;
        while (i < (long int) linea.size() && esEspacio(linea[i]))
            i++;
        while (i < (long int) linea.size() && linea[i] >= '0' && linea[i] <= '9' && correcto)
        {
            int digito = linea[i] - '0';
            if (numero > (maximo - digito) / 10 || digito > maximo)
                correcto = false;
            else
                numero = numero * 10 + digito;
            hayDigitos = true;
            i++;
        }
        while (i < (long int) linea.size() && esEspacio(linea[i]))
            i++;
        valido = correcto && hayDigitos && i == (long int) linea.size() && numero >= minimo;
        if (valido)
            valor = numero;
        else if (hayEntrada)
            cout << "Ingrese un entero entre " << minimo << " y " << maximo << "." << endl;
    }
    return (valido);
}

bool leerPrecio(string mensaje, double& valor)
{
    bool valido = false;
    bool hayEntrada = true;
    while (!valido && hayEntrada)
    {
        string linea;
        hayEntrada = leerLinea(mensaje, linea);
        long int i = 0;
        double numero = 0;
        double posicionDecimal = 0.1;
        bool decimal = false;
        bool hayDigitos = false;
        bool correcto = hayEntrada;
        while (i < (long int) linea.size() && esEspacio(linea[i]))
            i++;
        while (i < (long int) linea.size() && !esEspacio(linea[i]) && correcto)
        {
            char caracter = linea[i];
            if (caracter == '.' && !decimal)
                decimal = true;
            else if (caracter >= '0' && caracter <= '9')
            {
                int digito = caracter - '0';
                if (decimal)
                {
                    numero += digito * posicionDecimal;
                    posicionDecimal = posicionDecimal * 0.1;
                }
                else
                    numero = numero * 10 + digito;
                correcto = numero <= DBL_MAX;
                hayDigitos = true;
            }
            else
                correcto = false;
            i++;
        }
        while (i < (long int) linea.size() && esEspacio(linea[i]))
            i++;
        valido = correcto && hayDigitos && i == (long int) linea.size();
        if (valido)
            valor = numero;
        else if (hayEntrada)
            cout << "Ingrese un precio no negativo (use punto para decimales)." << endl;
    }
    return (valido);
}

bool leerFecha(int& dia, int& mes, int& anio)
{
    bool valida = false;
    bool hayEntrada = true;
    while (!valida && hayEntrada)
    {
        long int d, m, a;
        hayEntrada = leerEntero("Dia: ", 1, 31, d) && leerEntero("Mes: ", 1, 12, m)
            && leerEntero("Anio (1900 a 2099): ", 1900, 2099, a);
        if (hayEntrada)
        {
            Fecha fecha(d, m, a);
            valida = fecha.esValida();
            if (valida)
            {
                dia = d;
                mes = m;
                anio = a;
            }
            else
                cout << "La fecha no existe. Ingresela nuevamente." << endl;
        }
    }
    return (valida);
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
         << "\nEditorial: " << libro->getEditorial() << "\nTipo: " << libro->getTipo()
         << "\nPrecio base: " << libro->getPrecioBase()
         << "\nUnidades vendidas: " << libro->getUnidadesVendidas() << endl;
    if (libro->getTipo() == "Novela")
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
        if (libro->getTipo() == "Escolar")
            cout << "Anio escolar: " << ((Escolar*) libro)->getAnioEscuela() << endl;
    }
}

bool altaAutor(Fachada& fachada)
{
    long int cedula;
    string nombre, apellido;
    bool leido = leerEntero("Cedula: ", 1, LONG_MAX, cedula)
        && leerTexto("Nombre: ", nombre) && leerTexto("Apellido: ", apellido);
    if (leido)
    {
        TipoError error;
        fachada.registrarAutor(cedula, nombre, apellido, error);
        mostrarResultado(error);
    }
    return (leido);
}

bool altaLibro(Fachada& fachada)
{
    long int tipo, isbn;
    double precio;
    string titulo, editorial;
    bool leido = leerEntero("Tipo (1 Novela, 2 Texto, 3 Escolar): ", 1, 3, tipo)
        && leerEntero("ISBN: ", 1, LONG_MAX, isbn)
        && leerTexto("Titulo: ", titulo) && leerTexto("Editorial: ", editorial)
        && leerPrecio("Precio base: ", precio);
    Libro* libro = NULL;
    if (leido)
    {
        if (tipo == 1)
        {
            string genero;
            leido = leerTexto("Genero (infantil para el descuento): ", genero);
            if (leido)
                libro = new Novela(isbn, titulo, editorial, precio, genero);
        }
        else
        {
            string materia;
            int dia, mes, anio;
            leido = leerTexto("Materia: ", materia) && leerFecha(dia, mes, anio);
            if (leido)
            {
                Fecha fecha(dia, mes, anio);
                if (tipo == 2)
                    libro = new Texto(isbn, titulo, editorial, precio, materia, fecha);
                else
                {
                    long int escolar;
                    leido = leerEntero("Anio escolar (1 a 6): ", 1, 6, escolar);
                    if (leido)
                        libro = new Escolar(isbn, titulo, editorial, precio, materia, fecha, escolar);
                }
            }
        }
    }
    if (leido)
    {
        TipoError error;
        fachada.registrarLibro(libro, error);
        if (error != SIN_ERROR)
            delete libro;
        mostrarResultado(error);
    }
    return (leido);
}

bool ejecutarOpcion(long int opcion, Fachada& fachada)
{
    bool leido = true;
    TipoError error;
    long int isbn, cedula;
    string texto;
    double importe;
    Libro* libro;
    switch (opcion)
    {
        case 1:
            leido = altaAutor(fachada);
            break;
        case 2:
            leido = altaLibro(fachada);
            break;
        case 3:
            leido = leerEntero("ISBN de la novela: ", 1, LONG_MAX, isbn)
                && leerEntero("Cedula del autor: ", 1, LONG_MAX, cedula);
            if (leido)
            {
                fachada.asociarAutor(isbn, cedula, error);
                mostrarResultado(error);
            }
            break;
        case 4:
        {
            leido = leerTexto("Comienzo exacto del titulo: ", texto);
            if (leido)
            {
                Iterador iter;
                fachada.listarPorPrefijo(texto, iter, error);
                if (error != SIN_ERROR)
                    mostrarResultado(error);
                else
                    while (iter.hayActual())
                    {
                        libro = (Libro*) iter.actual();
                        cout << libro->getId() << " | " << libro->getTitulo() << " | " << libro->getTipo() << endl;
                        iter.avanzar();
                    }
            }
            break;
        }
        case 5:
            leido = leerEntero("ISBN: ", 1, LONG_MAX, isbn);
            if (leido)
            {
                fachada.consultarLibro(isbn, libro, error);
                if (error == SIN_ERROR) mostrarLibro(libro);
                else mostrarResultado(error);
            }
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
            leido = leerEntero("ISBN vendido: ", 1, LONG_MAX, isbn);
            if (leido)
            {
                fachada.registrarVenta(isbn, importe, error);
                if (error == SIN_ERROR) cout << "Importe: " << importe << endl;
                else mostrarResultado(error);
            }
            break;
        case 8:
            leido = leerTexto("Editorial: ", texto);
            if (leido)
                cout << "Recaudacion: " << fachada.calcularRecaudacion(texto) << endl;
            break;
        case 9:
        {
            leido = leerPrecio("Precio base de referencia: ", importe);
            if (leido)
            {
                long int novelas, textos, escolares;
                fachada.contarVentasPorTipo(importe, novelas, textos, escolares);
                cout << "Unidades vendidas: novelas " << novelas << ", textos comunes " << textos
                     << ", escolares " << escolares << endl;
            }
            break;
        }
        case 10:
            leido = leerTexto("Editorial: ", texto);
            if (leido)
            {
                fachada.consultarMasVendido(texto, libro, error);
                if (error == SIN_ERROR) mostrarLibro(libro);
                else mostrarResultado(error);
            }
            break;
    }
    return (leido);
}

void ejecutarMenu()
{
    Fachada fachada;
    bool continuar = true;
    while (continuar)
    {
        cout << "\nLIBRERIA\n1. Registrar autor\n2. Registrar libro\n3. Asociar autor a novela"
             << "\n4. Listar libros por comienzo del titulo\n5. Consultar libro\n6. Listar autores"
             << "\n7. Registrar venta\n8. Recaudacion por editorial\n9. Ventas por tipo y precio base"
             << "\n10. Libro mas vendido de una editorial\n0. Salir\n";
        long int opcion;
        if (!leerEntero("Opcion: ", 0, 10, opcion))
            continuar = false;
        else if (opcion == 0)
            continuar = false;
        else
            continuar = ejecutarOpcion(opcion, fachada);
    }
}
