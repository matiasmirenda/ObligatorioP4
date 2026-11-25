#include "ValidacionDatos.h"
#include "Parametros.h"
#include "Menu.h"
#include "Fachada.h"
#include "MensajesError.h"
#include "Fecha.h"
#include <iostream>

using namespace std;

void leerLinea(string& linea)
{
    linea = "";
    int caracter = cin.get();
    while (!cin.fail() && caracter != '\n')
    {
        if (linea.size() <= MAX_LARGO_TEXTO)
            linea += (char) caracter;
        caracter = cin.get();
    }
    if (linea.size() > MAX_LARGO_TEXTO)
        linea = "";
}

void leerTexto(string mensaje, string& valor)
{
    valor = "";
    if (!cin.fail())
    {
        cout << mensaje;
        do
        {
            leerLinea(valor);
            if (!cin.fail() && !textoValido(valor))
                cout << "Ingrese un texto de " << MIN_LARGO_TEXTO << " a " << MAX_LARGO_TEXTO << " caracteres con contenido: ";
        } while (!cin.fail() && !textoValido(valor));
    }
}

bool esEntero(string linea)
{
    bool valido = linea.size() > 0 && linea.size() <= DIGITOS_ISBN;
    unsigned int i = 0;
    while (i < linea.size() && valido)
    {
        if (linea[i] < DIGITO_CERO || linea[i] > DIGITO_NUEVE)
            valido = false;
        i++;
    }
    return (valido);
}

long int convertirEntero(string linea)
{
    long int numero = 0;
    for (unsigned int i = 0; i < linea.size(); i++)
        numero = numero * BASE_DECIMAL + (linea[i] - DIGITO_CERO);
    return (numero);
}

void leerEntero(string mensaje, long int minimo, long int maximo, long int& valor)
{
    string linea;
    bool valido = false;
    valor = 0;
    if (!cin.fail())
        cout << mensaje;
    while (!valido && !cin.fail())
    {
        leerLinea(linea);
        valido = esEntero(linea);
        if (valido)
        {
            valor = convertirEntero(linea);
            valido = valor >= minimo && valor <= maximo;
        }
        if (!valido && !cin.fail())
            cout << "Ingrese un entero entre " << minimo << " y " << maximo << ": ";
    }
}

void leerIdentificador(string mensaje, int digitos, long int& valor)
{
    string linea;
    bool valido = false;
    valor = 0;
    if (!cin.fail())
        cout << mensaje;
    while (!valido && !cin.fail())
    {
        leerLinea(linea);
        valido = linea.size() == (unsigned int) digitos && esEntero(linea);
        if (valido)
        {
            valor = convertirEntero(linea);
            valido = cantidadDigitos(valor) == digitos;
        }
        if (!valido && !cin.fail())
            cout << "Ingrese " << digitos << " digitos sin separadores: ";
    }
}

void analizarPrecio(string linea, float& numero, bool& valido)
{
    numero = 0;
    float decimal = VALOR_DECIMA;
    bool hayPunto = false;
    bool hayDigito = false;
    valido = linea.size() > 0;
    unsigned int i = 0;
    while (i < linea.size() && valido)
    {
        char caracter = linea[i];
        if (caracter == SEPARADOR_DECIMAL && !hayPunto)
            hayPunto = true;
        else if (caracter >= DIGITO_CERO && caracter <= DIGITO_NUEVE)
        {
            hayDigito = true;
            int digito = caracter - DIGITO_CERO;
            if (!hayPunto)
                numero = numero * BASE_DECIMAL + digito;
            else
            {
                numero = numero + digito * decimal;
                decimal = decimal / BASE_DECIMAL;
            }
        }
        else
            valido = false;
        i++;
    }
    valido = valido && hayDigito;
    if (!valido)
        numero = 0;
}

bool esPrecio(string linea)
{
    float numero;
    bool valido;
    analizarPrecio(linea, numero, valido);
    return (valido);
}

float convertirPrecio(string linea)
{
    float numero;
    bool valido;
    analizarPrecio(linea, numero, valido);
    return (numero);
}

void leerPrecio(string mensaje, float& valor)
{
    string linea;
    bool valido = false;
    valor = 0;
    if (!cin.fail())
        cout << mensaje;
    while (!valido && !cin.fail())
    {
        leerLinea(linea);
        analizarPrecio(linea, valor, valido);
        if (!valido && !cin.fail())
            cout << "Ingrese un precio valido (use punto para decimales): ";
    }
}

void altaAutor(Fachada& fachada)
{
    long int cedula;
    string nombre, apellido;
    TipoError error;
    leerIdentificador("Cedula: ", DIGITOS_CEDULA, cedula);
    leerTexto("Nombre: ", nombre);
    leerTexto("Apellido: ", apellido);
    if (!cin.fail())
    {
        fachada.registrarAutor(cedula, nombre, apellido, error);
        mostrarResultado(error);
    }
}

void altaLibro(Fachada& fachada)
{
    long int opcionTipo, isbn, dia = 0, mes = 0, anio = 0, anioEscuela = 0;
    float precio;
    string titulo, editorial, genero, materia;
    TipoLibro tipo;
    TipoError error;
    cout << "Tipo (" << OPCION_TIPO_NOVELA << " Novela, " << OPCION_TIPO_TEXTO
         << " Texto, " << OPCION_TIPO_ESCOLAR << " Escolar): ";
    leerEntero("", OPCION_TIPO_NOVELA, OPCION_TIPO_ESCOLAR, opcionTipo);
    leerIdentificador("ISBN: ", DIGITOS_ISBN, isbn);
    leerTexto("Titulo: ", titulo);
    leerTexto("Editorial: ", editorial);
    leerPrecio("Precio base: ", precio);
    if (opcionTipo == OPCION_TIPO_NOVELA)
    {
        tipo = NOVELA;
        leerTexto("Genero (infantil para el descuento): ", genero);
    }
    else
    {
        leerTexto("Materia: ", materia);
        bool fechaValida = false;
        while (!fechaValida && !cin.fail())
        {
            leerEntero("Dia: ", PRIMER_DIA_MES, DIAS_MES_LARGO, dia);
            leerEntero("Mes: ", ENERO, DICIEMBRE, mes);
            leerEntero("Anio de publicacion: ", ANIO_PUBLICACION_MINIMO, ANIO_PUBLICACION_MAXIMO, anio);
            Fecha fecha(dia, mes, anio);
            fechaValida = fecha.esValida();
            if (!fechaValida && !cin.fail())
                cout << "Fecha inexistente. Ingrese nuevamente dia, mes y anio." << '\n';
        }
        if (opcionTipo == OPCION_TIPO_TEXTO)
            tipo = TEXTO;
        else
        {
            tipo = ESCOLAR;
            leerEntero("Anio escolar: ", ANIO_ESCOLAR_MINIMO, ANIO_ESCOLAR_MAXIMO, anioEscuela);
        }
    }
    if (!cin.fail())
    {
        fachada.registrarLibro(tipo, isbn, titulo, editorial, precio, genero, materia,
                               dia, mes, anio, anioEscuela, error);
        mostrarResultado(error);
    }
}

void ejecutarOpcion(long int opcion, Fachada& fachada)
{
    TipoError error;
    long int isbn, cedula;
    string texto;
    float precio;
    switch (opcion)
    {
        case OPCION_REGISTRAR_AUTOR:
            altaAutor(fachada);
            break;
        case OPCION_REGISTRAR_LIBRO:
            altaLibro(fachada);
            break;
        case OPCION_ASOCIAR_AUTOR:
            leerIdentificador("ISBN de la novela: ", DIGITOS_ISBN, isbn);
            leerIdentificador("Cedula del autor: ", DIGITOS_CEDULA, cedula);
            if (!cin.fail())
            {
                fachada.asociarAutor(isbn, cedula, error);
                mostrarResultado(error);
            }
            break;
        case OPCION_LISTAR_POR_PREFIJO:
            leerTexto("Comienzo exacto del titulo: ", texto);
            if (!cin.fail())
            {
                fachada.listarPorPrefijo(texto, error);
                if (error != SIN_ERROR)
                    mostrarResultado(error);
            }
            break;
        case OPCION_CONSULTAR_LIBRO:
            leerIdentificador("ISBN: ", DIGITOS_ISBN, isbn);
            if (!cin.fail())
            {
                fachada.consultarLibro(isbn, error);
                if (error != SIN_ERROR)
                    mostrarResultado(error);
            }
            break;
        case OPCION_LISTAR_AUTORES:
            fachada.listarAutores(error);
            if (error != SIN_ERROR)
                mostrarResultado(error);
            break;
        case OPCION_REGISTRAR_VENTA:
            leerIdentificador("ISBN vendido: ", DIGITOS_ISBN, isbn);
            if (!cin.fail())
            {
                fachada.registrarVenta(isbn, error);
                if (error != SIN_ERROR)
                    mostrarResultado(error);
            }
            break;
        case OPCION_CALCULAR_RECAUDACION:
            leerTexto("Editorial: ", texto);
            if (!cin.fail())
                fachada.calcularRecaudacion(texto);
            break;
        case OPCION_CONTAR_VENTAS_POR_TIPO:
            leerPrecio("Precio base de referencia: ", precio);
            if (!cin.fail())
                fachada.contarVentasPorTipo(precio);
            break;
        case OPCION_CONSULTAR_MAS_VENDIDO:
            leerTexto("Editorial: ", texto);
            if (!cin.fail())
            {
                fachada.consultarMasVendido(texto, error);
                if (error != SIN_ERROR)
                    mostrarResultado(error);
            }
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
        cout << "\nLIBRERIA\n"
             << OPCION_REGISTRAR_AUTOR << ". Registrar autor\n"
             << OPCION_REGISTRAR_LIBRO << ". Registrar libro\n"
             << OPCION_ASOCIAR_AUTOR << ". Asociar autor a novela\n"
             << OPCION_LISTAR_POR_PREFIJO << ". Listar libros por comienzo del titulo\n"
             << OPCION_CONSULTAR_LIBRO << ". Consultar libro\n"
             << OPCION_LISTAR_AUTORES << ". Listar autores\n"
             << OPCION_REGISTRAR_VENTA << ". Registrar venta\n"
             << OPCION_CALCULAR_RECAUDACION << ". Recaudacion por editorial\n"
             << OPCION_CONTAR_VENTAS_POR_TIPO << ". Ventas por tipo y precio base\n"
             << OPCION_CONSULTAR_MAS_VENDIDO << ". Libro mas vendido de una editorial\n"
             << OPCION_SALIR << ". Salir\n";
        leerEntero("Opcion: ", OPCION_SALIR, OPCION_CONSULTAR_MAS_VENDIDO, opcion);
        if (cin.fail() || opcion == OPCION_SALIR)
            continuar = false;
        else
            ejecutarOpcion(opcion, fachada);
    }
}
