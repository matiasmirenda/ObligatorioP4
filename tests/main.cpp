#include "Parametros.h"
#include "Novela.h"
#include "Escolar.h"
#include "MensajesError.h"
#include "Fachada.h"
#include "Menu.h"
#include "ValidacionDatos.h"
#include <sstream>
#include <iostream>

using namespace std;

int fallos = 0;

void verificar(bool condicion, const char* descripcion)
{
    if (!condicion)
    {
        cerr << "FALLO: " << descripcion << '\n';
        fallos++;
    }
    else
        cout << "OK: " << descripcion << '\n';
}

bool mismoImporte(float obtenido, float esperado)
{
    float diferencia = obtenido - esperado;
    return (diferencia > -0.0001f && diferencia < 0.0001f);
}

class NovelaControlada : public Novela
{
private:
    bool* destruida;
public:
    NovelaControlada(long int id, bool* marca)
        : Novela(id, "Control", "Editorial", 100, "aventura")
    {
        destruida = marca;
        *destruida = false;
    }
    ~NovelaControlada() { *destruida = true; }
};

class AutorControlado : public Autor
{
private:
    bool* destruido;
public:
    AutorControlado(long int id, bool* marca) : Autor(id, "Nombre", "Apellido")
    {
        destruido = marca;
        *destruido = false;
    }
    ~AutorControlado() { *destruido = true; }
};

void probarObjeto()
{
    bool libroDestruido = false, autorDestruido = false;
    Objeto* libro = new NovelaControlada(101, &libroDestruido);
    Objeto* autor = new AutorControlado(202, &autorDestruido);
    verificar(libro->getId() == 101 && autor->getId() == 202,
              "getId polimorfico de Libro y Autor mediante Objeto*");
    delete libro;
    delete autor;
    verificar(libroDestruido && autorDestruido,
              "destructor virtual de Objeto ejecuta ambos destructores derivados");
}

void probarFecha()
{
    Fecha fecha(29, 2, 2024);
    verificar(fecha.getDia() == 29 && fecha.getMes() == 2 && fecha.getAnio() == 2024,
              "constructor y selectores de Fecha");
    verificar(fecha.esValida(), "29 de febrero de anio bisiesto");
    verificar(Fecha(29, 2, 2000).esValida(), "siglo divisible por cuatrocientos");
    verificar(!Fecha(29, 2, 1900).esValida(), "siglo no bisiesto");
    verificar(!Fecha(29, 2, 2023).esValida(), "febrero no bisiesto");
    verificar(Fecha(1, 1, 1900).esValida() && Fecha(31, 12, 2099).esValida(),
              "limites incluidos del rango de anios");
    verificar(!Fecha(31, 12, 1899).esValida() && !Fecha(1, 1, 2100).esValida(),
              "anios fuera del rango");
    verificar(!Fecha(1, 0, 2024).esValida() && !Fecha(1, 13, 2024).esValida(),
              "meses fuera de rango");
    verificar(!Fecha(0, 1, 2024).esValida() && !Fecha(-1, 1, 2024).esValida(),
              "dias no positivos");
    int limites[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (int mes = 1; mes <= 12; mes++)
    {
        verificar(Fecha(limites[mes - 1], mes, 2023).esValida(), "ultimo dia de cada mes");
        verificar(!Fecha(limites[mes - 1] + 1, mes, 2023).esValida(), "dia posterior al ultimo del mes");
    }
    verificar(Fecha(31, 12, 2023) < fecha, "orden por anio");
    verificar(Fecha(31, 1, 2024) < fecha, "orden por mes");
    verificar(Fecha(28, 2, 2024) < fecha, "orden por dia");
    verificar(!(fecha < Fecha(1, 1, 2023)) && !(fecha < Fecha(1, 1, 2024)),
              "orden inverso por anio y mes");
    verificar(!(fecha < Fecha(28, 2, 2024)) && !(fecha < Fecha(29, 2, 2024)),
              "orden inverso por dia y fechas iguales");
}

void probarAutor()
{
    Autor autor(12345678, "Ana", "Perez");
    verificar(autor.getId() == 12345678, "identificador de Autor es su cedula");
    verificar(autor.getNombre() == "Ana" && autor.getApellido() == "Perez",
              "constructor y selectores de Autor");
    Objeto* objeto = &autor;
    verificar(objeto->getId() == autor.getId(), "Autor satisface contrato de Objeto");
}

void probarLibro()
{
    Novela novela(2147483647L, "El camino", "Editorial", 100, "aventura");
    Libro* libro = &novela;
    verificar(libro->getId() == 2147483647L, "identificador numerico almacenado por Libro");
    verificar(libro->getTitulo() == "El camino" && libro->getEditorial() == "Editorial",
              "datos comunes del libro");
    verificar(libro->getPrecioBase() == 100 && libro->getUnidadesVendidas() == 0,
              "precio y ventas iniciales");
    libro->registrarVenta();
    libro->registrarVenta();
    verificar(libro->getUnidadesVendidas() == 2, "incremento por cada venta");
    verificar(libro->comienzaCon("El") && libro->comienzaCon("El camino"), "prefijo exacto");
    verificar(!libro->comienzaCon("camino") && !libro->comienzaCon("el"),
              "no busca subcadena y distingue mayusculas");
    verificar(!libro->comienzaCon("El camino largo") && libro->comienzaCon(""),
              "prefijo largo y prefijo vacio");
    verificar(!Novela(0, "T", "E", 1, "g").datosValidos(), "ISBN cero invalido");
    verificar(!Novela(-1, "T", "E", 1, "g").datosValidos(), "ISBN negativo invalido");
    verificar(!Novela(1, "", "E", 1, "g").datosValidos(), "titulo vacio invalido");
    verificar(!Novela(1, "T", "", 1, "g").datosValidos(), "editorial vacia invalida");
    verificar(!Novela(1, "T", "E", -1, "g").datosValidos(), "precio negativo invalido");
    verificar(Novela(1, "T", "E", 0, "g").datosValidos(), "precio cero admitido");
    bool destruida = false;
    Libro* dinamico = new NovelaControlada(45, &destruida);
    delete dinamico;
    verificar(destruida, "destructor virtual mediante Libro*");
}

void probarNovela()
{
    ColeccionAutores autores;
    Autor* primero = new Autor(10, "Ana", "Perez");
    Autor* segundo = new Autor(20, "Juan", "Lopez");
    verificar(!autores.existe(10) && !autores.existe(20), "claves nuevas antes de registrar autores");
    autores.insertar(primero);
    autores.insertar(segundo);
    {
        Novela infantil(1, "Cuentos", "Editorial", 100, "infantil");
        Novela comun(2, "Otra", "E", 100, "aventura");
        Libro* libro = &infantil;
        verificar(libro->getTipo() == NOVELA && infantil.getGenero() == "infantil",
                  "tipo y genero de novela");
        verificar(mismoImporte(libro->calcularPrecioVenta(), 80), "descuento infantil");
        verificar(mismoImporte(comun.calcularPrecioVenta(), 100), "novela comun sin descuento");
        verificar(infantil.getAutor() == NULL, "novela inicialmente sin autor");
        verificar(!Novela(3, "T", "E", 1, "").datosValidos(), "genero vacio invalido");
        infantil.asignarAutor(primero);
        comun.asignarAutor(primero);
        verificar(infantil.getAutor() == comun.getAutor(), "dos novelas comparten autor sin copia");
        infantil.asignarAutor(segundo);
        verificar(infantil.getAutor() == segundo && comun.getAutor() == primero,
                  "reemplazar asociacion no afecta otra novela");
    }
    verificar(autores.existe(10) && autores.buscar(10) == primero && primero->getNombre() == "Ana",
              "destruir novelas no destruye autores de la coleccion");
}

void probarTexto()
{
    Texto anterior(1, "Matematica", "E", 100, "Matematica", Fecha(31, 12, 2025));
    Texto corte(2, "Fisica", "E", 100, "Fisica", Fecha(1, 1, 2026));
    Texto posterior(3, "Quimica", "E", 100, "Quimica", Fecha(2, 1, 2026));
    Libro* libro = &anterior;
    verificar(mismoImporte(libro->calcularPrecioVenta(), 75), "texto anterior al corte tiene descuento");
    verificar(mismoImporte(corte.calcularPrecioVenta(), 100), "corte exacto sin descuento");
    verificar(mismoImporte(posterior.calcularPrecioVenta(), 100), "fecha posterior sin descuento");
    verificar(libro->getTipo() == TEXTO && anterior.getMateria() == "Matematica",
              "tipo y materia de Texto");
    Fecha publicada = anterior.getFechaPublicado();
    verificar(publicada.getDia() == 31 && publicada.getMes() == 12 && publicada.getAnio() == 2025,
              "consulta de Fecha por valor");
    verificar(anterior.datosValidos(), "datos validos de Texto");
    verificar(!Texto(1, "T", "E", 1, "", Fecha(1, 1, 2000)).datosValidos(), "materia vacia invalida");
    verificar(!Texto(1, "T", "E", 1, "M", Fecha(31, 2, 2024)).datosValidos(), "fecha invalida en Texto");
    verificar(!Texto(0, "T", "E", 1, "M", Fecha(1, 1, 2000)).datosValidos(), "datos de Libro invalidos en Texto");
}

void probarEscolar()
{
    for (int anio = 1; anio <= 6; anio++)
    {
        Escolar viejo(1, "Escolar", "E", 100, "M", Fecha(31, 12, 2025), anio);
        Escolar nuevo(2, "Escolar", "E", 100, "M", Fecha(1, 1, 2026), anio);
        float esperadoViejo = anio <= 3 ? 67.5f : 75;
        float esperadoNuevo = anio <= 3 ? 90 : 100;
        Libro* libro = &viejo;
        verificar(viejo.datosValidos() && viejo.getAnioEscuela() == anio, "anio escolar valido");
        verificar(mismoImporte(libro->calcularPrecioVenta(), esperadoViejo), "descuentos escolares acumulados");
        verificar(mismoImporte(nuevo.calcularPrecioVenta(), esperadoNuevo), "escolar desde corte");
        verificar(libro->getTipo() == ESCOLAR, "tipo Escolar mediante Libro*");
    }
    verificar(!Escolar(1, "T", "E", 1, "M", Fecha(1, 1, 2000), 0).datosValidos(), "anio escolar cero");
    verificar(!Escolar(1, "T", "E", 1, "M", Fecha(1, 1, 2000), 7).datosValidos(), "anio escolar mayor a seis");
    verificar(!Escolar(1, "T", "E", 1, "", Fecha(1, 1, 2000), 1).datosValidos(), "datos de Texto invalidos en Escolar");
}

void probarIterador()
{
    Autor autor(10, "Ana", "Perez");
    Novela novela(20, "Libro", "E", 100, "aventura");
    {
        Iterador iter;
        verificar(!iter.hayActual(), "Iterador inicialmente vacio");
        iter.reiniciar();
        verificar(!iter.hayActual(), "Iterador vacio puede reiniciarse");
        iter.insertar(&autor);
        iter.insertar(&novela);
        verificar(iter.hayActual() && iter.actual() == &autor && iter.actual()->getId() == 10,
                  "Iterador unico acepta Autor como Objeto*");
        Autor* actualAutor = (Autor*) iter.actual();
        verificar(actualAutor->getNombre() == "Ana", "cast con origen conocido a Autor*");
        iter.avanzar();
        verificar(iter.hayActual() && iter.actual() == &novela && iter.actual()->getId() == 20,
                  "Iterador unico acepta Libro como Objeto*");
        Libro* actualLibro = (Libro*) iter.actual();
        verificar(actualLibro->getTipo() == NOVELA, "cast con origen conocido a Libro*");
        iter.avanzar();
        verificar(!iter.hayActual(), "fin de recorrido no circular");
        iter.reiniciar();
        verificar(iter.hayActual() && iter.actual() == &autor, "reinicio desde el primer objeto");
    }
    verificar(autor.getNombre() == "Ana" && novela.getTitulo() == "Libro", "Iterador no elimina objetos locales");
    bool libroDestruido = false, autorDestruido = false;
    Objeto* libro = new NovelaControlada(30, &libroDestruido);
    Objeto* otroAutor = new AutorControlado(40, &autorDestruido);
    {
        Iterador iter;
        iter.insertar(libro);
        iter.insertar(otroAutor);
    }
    verificar(!libroDestruido && !autorDestruido, "Iterador no adquiere propiedad del payload dinamico");
    delete libro;
    delete otroAutor;
    verificar(libroDestruido && autorDestruido, "propietarios eliminan payload una sola vez");
}

void probarHashLibros()
{
    bool destruidoUno = false, destruidoDos = false, destruidoTres = false;
    {
        HashLibros hash;
        verificar(hash.estaVacia() && !hash.existe(1), "HashLibros vacio");
        verificar(!hash.existe(0) && !hash.existe(-1), "existe con ID no positivo");
        {
            Iterador vacio;
            hash.cargarIterador(vacio);
            verificar(!vacio.hayActual(), "recorrido de hash vacio");
        }
        Libro* uno = new NovelaControlada(1, &destruidoUno);
        Libro* dos = new NovelaControlada(501, &destruidoDos);
        Libro* tres = new NovelaControlada(1001, &destruidoTres);
        verificar(uno->datosValidos() && !hash.existe(1), "precondiciones primer libro hash");
        hash.insertar(uno);
        verificar(dos->datosValidos() && !hash.existe(501), "precondiciones segundo libro hash");
        hash.insertar(dos);
        verificar(tres->datosValidos() && !hash.existe(1001), "precondiciones tercer libro hash");
        hash.insertar(tres);
        verificar(!hash.estaVacia() && hash.existe(1) && hash.existe(501) && hash.existe(1001), "colisiones en 500 cubetas");
        verificar(hash.buscar(1) == uno && hash.buscar(501) == dos && hash.buscar(1001) == tres,
                  "buscar principio medio y final de cadena");
        verificar(!hash.existe(1501), "ausente dentro de cadena se consulta con existe");
        verificar(hash.existe(uno->getId()), "llamador evita reinsertar mismo puntero");
        Novela* repetida = new Novela(501, "Repetida", "E", 10, "aventura");
        verificar(hash.existe(repetida->getId()), "llamador detecta duplicado sin insertar");
        delete repetida;
        Texto* otraCubeta = new Texto(499, "Texto", "E", 100, "M", Fecha(1, 1, 2026));
        verificar(otraCubeta->datosValidos() && !hash.existe(499), "precondiciones ultima cubeta");
        hash.insertar(otraCubeta);
        verificar(hash.existe(499) && hash.buscar(499) == otraCubeta, "ultima cubeta accesible");
        {
            Iterador iter;
            hash.cargarIterador(iter);
            int vistos = 0;
            bool vistoUno = false, vistoDos = false, vistoTres = false, vistoTexto = false;
            while (iter.hayActual())
        {
                Libro* actual = (Libro*) iter.actual();
                if (actual == uno) vistoUno = true;
                if (actual == dos) vistoDos = true;
                if (actual == tres) vistoTres = true;
                if (actual == otraCubeta) vistoTexto = true;
                vistos++;
                iter.avanzar();
        }
        verificar(vistos == 4 && vistoUno && vistoDos && vistoTres && vistoTexto,
                  "hash entrega cada referencia una vez mediante Iterador");
        iter.reiniciar();
        verificar(iter.hayActual(), "reinicia iterador del hash");
        }
        verificar(!destruidoUno && !destruidoDos && !destruidoTres && hash.buscar(1) == uno,
                  "eliminar recorrido no elimina libros de hash");
    }
    verificar(destruidoUno && destruidoDos && destruidoTres, "HashLibros libera payload por destructor virtual");

    HashLibros extremo;
    extremo.insertar(new Novela(500, "Primera", "E", 10, "aventura"));
    verificar(!extremo.estaVacia() && extremo.existe(500), "cubeta cero con clave positiva");
}

void probarABBAutores()
{
    bool destruido = false;
    {
        ABBAutores arbol;
        verificar(arbol.estaVacia() && !arbol.existe(40), "ABB vacio");
        {
            Iterador vacio;
            arbol.cargarIterador(vacio);
            verificar(!vacio.hayActual(), "recorrido de ABB vacio");
        }
        long int claves[7] = {40, 20, 60, 10, 30, 50, 70};
        for (int i = 0; i < 7; i++)
        {
                Autor* nuevo;
                if (i == 0)
                    nuevo = new AutorControlado(claves[i], &destruido);
                else
                    nuevo = new Autor(claves[i], "Nombre", "Apellido");
                verificar(!arbol.existe(nuevo->getId()), "clave nueva de autor");
                arbol.insertar(nuevo);
        }
        verificar(!arbol.estaVacia(), "ABB no vacio despues de insertar");
        for (int i = 0; i < 7; i++)
            verificar(arbol.existe(claves[i]) && arbol.buscar(claves[i])->getId() == claves[i],
                      "buscar raiz y ambos subarboles");
        verificar(!arbol.existe(5) && !arbol.existe(25) && !arbol.existe(65) && !arbol.existe(80),
                  "faltantes en extremos y entre nodos");
        Autor* repetido = new Autor(20, "Otro", "Autor");
        verificar(arbol.existe(repetido->getId()), "caller detecta cedula duplicada");
        delete repetido;
        Autor* original = arbol.buscar(20);
        verificar(arbol.existe(original->getId()), "no reinsertar puntero ya propio del ABB");
        {
            Iterador iter;
            arbol.cargarIterador(iter);
            long int esperado = 10;
            while (iter.hayActual())
        {
                Autor* actual = (Autor*) iter.actual();
                verificar(actual->getId() == esperado, "ABB genera recorrido ascendente");
                esperado += 10;
                iter.avanzar();
        }
        verificar(esperado == 80, "recorrido contiene siete autores");
        iter.reiniciar();
        verificar(iter.hayActual() && iter.actual()->getId() == 10, "reinicio desde minimo del ABB");
        }
        verificar(!destruido && arbol.buscar(20) == original, "iterador no elimina autores del ABB");
    }
    verificar(destruido, "ABB elimina autor derivado mediante destructor virtual");
    ABBAutores creciente;
    ABBAutores decreciente;
    for (int i = 1; i <= 5; i++)
    {
        creciente.insertar(new Autor(i, "N", "A"));
        decreciente.insertar(new Autor(6 - i, "N", "A"));
    }
    verificar(creciente.existe(5) && decreciente.existe(1), "ABB degenerado admite buscar extremos");
    verificar(creciente.buscar(5)->getId() == 5 && decreciente.buscar(1)->getId() == 1,
              "buscar extremos de ABB degenerados");
}

void probarColeccionLibros()
{
    bool destruido = false;
    {
        ColeccionLibros libros;
        verificar(libros.estaVacia() && !libros.existe(100), "coleccion libros delega vacio y existe");
        Libro* libro = new NovelaControlada(100, &destruido);
        verificar(libro->datosValidos() && !libros.existe(libro->getId()), "precondiciones insertar libro");
        libros.insertar(libro);
        verificar(!libros.estaVacia() && libros.existe(100) && libros.buscar(100) == libro,
                  "coleccion libros delega insertar y buscar al hash");
        {
            Iterador iter;
            libros.cargarIterador(iter);
            verificar(iter.hayActual() && (Libro*) iter.actual() == libro, "coleccion devuelve Iterador comun");
            iter.avanzar();
            verificar(!iter.hayActual(), "un libro en la coleccion");
        }
        verificar(!destruido, "iterador no posee libro de coleccion");
    }
    verificar(destruido, "destruccion de coleccion libera hash y libros");
}

void probarColeccionAutores()
{
    bool destruido = false;
    {
        ColeccionAutores autores;
        verificar(autores.estaVacia() && !autores.existe(40), "coleccion autores delega vacio y existe");
        Autor* mayor = new AutorControlado(40, &destruido);
        Autor* menor = new Autor(20, "Ana", "Perez");
        verificar(!autores.existe(40) && !autores.existe(20), "claves nuevas de coleccion autores");
        autores.insertar(mayor);
        autores.insertar(menor);
        verificar(!autores.estaVacia() && autores.existe(40) && autores.buscar(40) == mayor,
                  "coleccion autores delega insertar y buscar al ABB");
        {
            Iterador iter;
            autores.cargarIterador(iter);
            verificar(iter.hayActual() && (Autor*) iter.actual() == menor, "coleccion autores primero menor cedula");
            iter.avanzar();
            verificar(iter.hayActual() && (Autor*) iter.actual() == mayor, "coleccion autores siguiente cedula");
            iter.avanzar();
            verificar(!iter.hayActual(), "fin recorrido coleccion autores");
        }
        verificar(!destruido && autores.buscar(20) == menor, "iterador no elimina autores de coleccion");
    }
    verificar(destruido, "destruccion de coleccion libera ABB y autores");
}

void probarMensajesError()
{
    verificar(mensajeError(SIN_ERROR) == "Sin error.", "mensaje SIN_ERROR");
    verificar(mensajeError(AUTOR_YA_EXISTE) == "Ya existe un autor con esa cedula.", "mensaje AUTOR_YA_EXISTE");
    verificar(mensajeError(LIBRO_YA_EXISTE) == "Ya existe un libro con ese ISBN.", "mensaje LIBRO_YA_EXISTE");
    verificar(mensajeError(LIBRO_NO_EXISTE) == "No existe un libro con ese ISBN.", "mensaje LIBRO_NO_EXISTE");
    verificar(mensajeError(AUTOR_NO_EXISTE) == "No existe un autor con esa cedula.", "mensaje AUTOR_NO_EXISTE");
    verificar(mensajeError(LIBRO_NO_ES_NOVELA) == "El libro indicado no es una novela.", "mensaje LIBRO_NO_ES_NOVELA");
    verificar(mensajeError(SIN_COINCIDENCIAS) == "Ningun titulo comienza con esa frase.", "mensaje SIN_COINCIDENCIAS");
    verificar(mensajeError(SIN_AUTORES) == "No hay autores registrados.", "mensaje SIN_AUTORES");
    verificar(mensajeError(SIN_LIBROS_DE_EDITORIAL) == "No hay libros registrados de esa editorial.", "mensaje SIN_LIBROS_DE_EDITORIAL");
    verificar(mensajeError((TipoError) -1) == "Error desconocido.", "codigo de error desconocido");
}

string probarSalidaFachada(Fachada& fachada, long int opcion, string texto, long int isbn,
                          float precio, TipoError& error)
{
    ostringstream salida;
    streambuf* original = cout.rdbuf(salida.rdbuf());
    error = SIN_ERROR;
    switch (opcion)
    {
        case OPCION_LISTAR_POR_PREFIJO:
            fachada.listarPorPrefijo(texto, error);
            break;
        case OPCION_CONSULTAR_LIBRO:
            fachada.consultarLibro(isbn, error);
            break;
        case OPCION_LISTAR_AUTORES:
            fachada.listarAutores(error);
            break;
        case OPCION_REGISTRAR_VENTA:
            fachada.registrarVenta(isbn, error);
            break;
        case OPCION_CALCULAR_RECAUDACION:
            fachada.calcularRecaudacion(texto);
            break;
        case OPCION_CONTAR_VENTAS_POR_TIPO:
            fachada.contarVentasPorTipo(precio);
            break;
        case OPCION_CONSULTAR_MAS_VENDIDO:
            fachada.consultarMasVendido(texto, error);
            break;
    }
    cout.rdbuf(original);
    return (salida.str());
}

int contarLineas(string texto)
{
    int cantidad = 0;
    for (unsigned int i = 0; i < texto.size(); i++)
        if (texto[i] == '\n')
            cantidad++;
    return (cantidad);
}

void probarFachada()
{
    Fachada fachada;
    TipoError error;
    string salida = probarSalidaFachada(fachada, OPCION_CONSULTAR_LIBRO, "", 1000000000001L, 0, error);
    verificar(error == LIBRO_NO_EXISTE && salida == "", "consulta inexistente sin imprimir datos");
    salida = probarSalidaFachada(fachada, OPCION_REGISTRAR_VENTA, "", 1000000000001L, 0, error);
    verificar(error == LIBRO_NO_EXISTE && salida == "", "venta inexistente no imprime importe");
    fachada.asociarAutor(1000000000001L, 10000001, error);
    verificar(error == LIBRO_NO_EXISTE, "asociar libro inexistente");
    salida = probarSalidaFachada(fachada, OPCION_LISTAR_AUTORES, "", 0, 0, error);
    verificar(error == SIN_AUTORES && salida == "", "autores vacios sin salida de datos");
    salida = probarSalidaFachada(fachada, OPCION_LISTAR_POR_PREFIJO, "El", 0, 0, error);
    verificar(error == SIN_COINCIDENCIAS && salida == "", "prefijo sin libros sin salida de datos");
    salida = probarSalidaFachada(fachada, OPCION_CONSULTAR_MAS_VENDIDO, "E", 0, 0, error);
    verificar(error == SIN_LIBROS_DE_EDITORIAL && salida == "", "editorial inexistente sin salida de datos");
    salida = probarSalidaFachada(fachada, OPCION_CALCULAR_RECAUDACION, "E", 0, 0, error);
    verificar(salida == "Recaudacion: 0\n", "coleccion imprime recaudacion vacia");
    salida = probarSalidaFachada(fachada, OPCION_CONTAR_VENTAS_POR_TIPO, "", 0, 0, error);
    verificar(salida == "Unidades vendidas: novelas 0, textos comunes 0, escolares 0\n", "coleccion imprime contadores vacios");
    fachada.registrarAutor(10000020, "Ana", "Perez", error);
    verificar(error == SIN_ERROR, "alta autor fachada");
    fachada.registrarAutor(10000010, "Luis", "Lopez", error);
    verificar(error == SIN_ERROR, "segundo autor fachada");
    fachada.registrarAutor(10000020, "Otro", "Nombre", error);
    verificar(error == AUTOR_YA_EXISTE, "autor duplicado");
    fachada.registrarLibro(NOVELA, 1000000000001L, "El bosque", "E", 100, "infantil", "", 0, 0, 0, 0, error);
    verificar(error == SIN_ERROR, "alta novela fachada");
    fachada.registrarLibro(TEXTO, 1000000000002L, "El texto", "E", 200, "", "M", 31, 12, 2025, 0, error);
    verificar(error == SIN_ERROR, "alta texto fachada");
    fachada.registrarLibro(ESCOLAR, 1000000000003L, "Escuela", "E", 100, "", "M", 1, 1, 2026, 2, error);
    verificar(error == SIN_ERROR, "alta escolar fachada");
    fachada.registrarLibro(NOVELA, 1000000000004L, "Otra", "Otra editorial", 500, "aventura", "", 0, 0, 0, 0, error);
    verificar(error == SIN_ERROR, "alta novela de otra editorial");
    salida = probarSalidaFachada(fachada, OPCION_CONSULTAR_LIBRO, "", 1000000000001L, 0, error);
    verificar(error == SIN_ERROR && salida.find("Titulo: El bosque\n") != string::npos
              && salida.find("Tipo: Novela\n") != string::npos && salida.find("Genero: infantil\n") != string::npos,
              "impresion polimorfica de novela desde Libro");
    verificar(salida.find("Unidades vendidas: 0\n") != string::npos
              && salida.find("Sin autor asociado.\n") != string::npos, "alta novela inicia sin ventas ni autor");
    salida = probarSalidaFachada(fachada, OPCION_CONSULTAR_LIBRO, "", 1000000000002L, 0, error);
    verificar(salida.find("Tipo: Texto\n") != string::npos && salida.find("Materia: M\n") != string::npos
              && salida.find("Publicacion: 31/12/2025\n") != string::npos, "texto y fecha imprimen sus datos");
    salida = probarSalidaFachada(fachada, OPCION_CONSULTAR_LIBRO, "", 1000000000003L, 0, error);
    verificar(salida.find("Tipo: Escolar\n") != string::npos && salida.find("Materia: M\n") != string::npos
              && salida.find("Anio escolar: 2\n") != string::npos, "escolar imprime datos heredados y propios");
    fachada.registrarLibro(NOVELA, 1000000000001L, "Duplicado", "E", 10, "aventura", "", 0, 0, 0, 0, error);
    verificar(error == LIBRO_YA_EXISTE, "libro duplicado rechazado");
    salida = probarSalidaFachada(fachada, OPCION_CONSULTAR_LIBRO, "", 1000000000001L, 0, error);
    verificar(salida.find("Titulo: El bosque\n") != string::npos, "duplicado no reemplaza al original");
    fachada.asociarAutor(1000000000002L, 10000020, error);
    verificar(error == LIBRO_NO_ES_NOVELA, "no convertir texto a novela");
    fachada.asociarAutor(1000000000001L, 10000099, error);
    verificar(error == AUTOR_NO_EXISTE, "asociacion con autor ausente rechazada");
    salida = probarSalidaFachada(fachada, OPCION_CONSULTAR_LIBRO, "", 1000000000001L, 0, error);
    verificar(salida.find("Sin autor asociado.\n") != string::npos, "asociacion rechazada no modifica novela");
    fachada.asociarAutor(1000000000001L, 10000020, error);
    verificar(error == SIN_ERROR, "asociar autor existente");
    salida = probarSalidaFachada(fachada, OPCION_CONSULTAR_LIBRO, "", 1000000000001L, 0, error);
    verificar(salida.find("Autor: 10000020 | Ana | Perez\n") != string::npos, "novela llama a imprimir del autor asociado");
    fachada.asociarAutor(1000000000001L, 10000010, error);
    verificar(error == SIN_ERROR, "reemplazar asociacion");
    salida = probarSalidaFachada(fachada, OPCION_CONSULTAR_LIBRO, "", 1000000000001L, 0, error);
    verificar(salida.find("Autor: 10000010 | Luis | Lopez\n") != string::npos
              && salida.find("10000020") == string::npos, "novela imprime solo el autor actual");
    salida = probarSalidaFachada(fachada, OPCION_LISTAR_AUTORES, "", 0, 0, error);
    verificar(error == SIN_ERROR && salida == "10000010 | Luis | Lopez\n10000020 | Ana | Perez\n",
              "autores imprimen una linea cada uno en orden y conservan al autor reemplazado");
    salida = probarSalidaFachada(fachada, OPCION_LISTAR_POR_PREFIJO, "El ", 0, 0, error);
    verificar(error == SIN_ERROR && salida.find("1000000000001 | El bosque | Novela\n") != string::npos
              && salida.find("1000000000002 | El texto | Texto\n") != string::npos,
              "libros imprimen datos basicos para el prefijo exacto");
    verificar(contarLineas(salida) == 2 && salida.find("Genero:") == string::npos,
              "listado basico contiene solo dos filas y no datos completos");
    salida = probarSalidaFachada(fachada, OPCION_LISTAR_POR_PREFIJO, "el", 0, 0, error);
    verificar(error == SIN_COINCIDENCIAS && salida == "", "prefijo distingue mayusculas");
    salida = probarSalidaFachada(fachada, OPCION_CONSULTAR_MAS_VENDIDO, "E", 0, 0, error);
    verificar(error == SIN_ERROR && salida.find("Unidades vendidas: 0\n") != string::npos,
              "maximo admite empate sin ventas");
    salida = probarSalidaFachada(fachada, OPCION_REGISTRAR_VENTA, "", 1000000000001L, 0, error);
    verificar(error == SIN_ERROR && salida == "Importe: 80\n", "libro imprime importe de venta infantil");
    salida = probarSalidaFachada(fachada, OPCION_REGISTRAR_VENTA, "", 1000000000002L, 0, error);
    verificar(error == SIN_ERROR && salida == "Importe: 150\n", "libro imprime importe de texto antiguo");
    salida = probarSalidaFachada(fachada, OPCION_REGISTRAR_VENTA, "", 1000000000003L, 0, error);
    verificar(error == SIN_ERROR && salida == "Importe: 90\n", "libro imprime importe escolar");
    salida = probarSalidaFachada(fachada, OPCION_REGISTRAR_VENTA, "", 1000000000001L, 0, error);
    verificar(error == SIN_ERROR && salida == "Importe: 80\n", "segunda venta imprime importe");
    salida = probarSalidaFachada(fachada, OPCION_CONSULTAR_LIBRO, "", 1000000000001L, 0, error);
    verificar(salida.find("Unidades vendidas: 2\n") != string::npos, "cada venta incrementa exactamente una unidad");
    salida = probarSalidaFachada(fachada, OPCION_REGISTRAR_VENTA, "", 1000000000004L, 0, error);
    verificar(error == SIN_ERROR && salida == "Importe: 500\n", "venta de otra editorial");
    salida = probarSalidaFachada(fachada, OPCION_CALCULAR_RECAUDACION, "E", 0, 0, error);
    verificar(salida == "Recaudacion: 400\n", "coleccion imprime recaudacion con descuentos");
    salida = probarSalidaFachada(fachada, OPCION_CALCULAR_RECAUDACION, "Ausente", 0, 0, error);
    verificar(salida == "Recaudacion: 0\n", "recaudacion de editorial ausente es cero");
    salida = probarSalidaFachada(fachada, OPCION_CONTAR_VENTAS_POR_TIPO, "", 0, 99, error);
    verificar(salida == "Unidades vendidas: novelas 3, textos comunes 1, escolares 1\n", "ventas discriminadas por tipo");
    salida = probarSalidaFachada(fachada, OPCION_CONTAR_VENTAS_POR_TIPO, "", 0, 100, error);
    verificar(salida == "Unidades vendidas: novelas 1, textos comunes 1, escolares 0\n", "precio estrictamente mayor");
    salida = probarSalidaFachada(fachada, OPCION_CONTAR_VENTAS_POR_TIPO, "", 0, 500, error);
    verificar(salida == "Unidades vendidas: novelas 0, textos comunes 0, escolares 0\n", "contadores no conservan consultas anteriores");
    salida = probarSalidaFachada(fachada, OPCION_CONSULTAR_MAS_VENDIDO, "E", 0, 0, error);
    verificar(error == SIN_ERROR && salida.find("ISBN: 1000000000001\n") != string::npos
              && salida.find("Autor: 10000010 | Luis | Lopez\n") != string::npos,
              "maximo por unidades imprime datos completos y autor");
    salida = probarSalidaFachada(fachada, OPCION_CONSULTAR_LIBRO, "", 1000000000999L, 0, error);
    verificar(error == LIBRO_NO_EXISTE && salida == "", "consulta fallida no imprime datos anteriores");
    salida = probarSalidaFachada(fachada, OPCION_CONSULTAR_MAS_VENDIDO, "Ausente", 0, 0, error);
    verificar(error == SIN_LIBROS_DE_EDITORIAL && salida == "", "maximo fallido no imprime datos anteriores");
    verificar(mensajeError(DATOS_INVALIDOS) == "Los datos ingresados no son validos.", "mensaje de datos invalidos");
}

string probarEntradaMenu(string entrada)
{
    istringstream datos(entrada);
    ostringstream salida;
    streambuf* entradaOriginal = cin.rdbuf(datos.rdbuf());
    streambuf* salidaOriginal = cout.rdbuf(salida.rdbuf());
    cin.clear();
    ejecutarMenu();
    cin.rdbuf(entradaOriginal);
    cout.rdbuf(salidaOriginal);
    cin.clear();
    return (salida.str());
}

void probarMenu()
{
    string salida = probarEntradaMenu(
        "6\n5\n1000000000999\n7\n1000000000999\n4\nAusente\n10\nAusente\n"
        "1\n10000020\nAna Maria\nPerez\n1\n10000010\nLuis\nLopez\n1\n10000020\nOtro\nAutor\n"
        "2\n1\n1000000000001\nEl bosque\nE\n100\ninfantil\n"
        "2\n2\n1000000000002\nEl texto\nE\n200\nMatematica\n31\n12\n2025\n"
        "2\n3\n1000000000003\nEscuela\nE\n100\nHistoria\n1\n1\n2026\n2\n"
        "2\n1\n1000000000001\nDuplicado\nE\n10\naventura\n"
        "3\n1000000000999\n10000020\n3\n1000000000002\n10000020\n"
        "3\n1000000000001\n10000999\n3\n1000000000001\n10000020\n3\n1000000000001\n10000010\n"
        "4\nEl \n5\n1000000000001\n5\n1000000000002\n5\n1000000000003\n6\n"
        "7\n1000000000001\n7\n1000000000002\n7\n1000000000003\n7\n1000000000001\n"
        "8\nE\n9\n99\n10\nE\n0\n");
    verificar(salida.find("No hay autores registrados.") != string::npos, "salida del menu");
    verificar(salida.find("No existe un libro con ese ISBN.") != string::npos, "salida del menu");
    verificar(salida.find("Ningun titulo comienza con esa frase.") != string::npos, "salida del menu");
    verificar(salida.find("No hay libros registrados de esa editorial.") != string::npos, "salida del menu");
    verificar(salida.find("Ya existe un autor con esa cedula.") != string::npos, "salida del menu");
    verificar(salida.find("Ya existe un libro con ese ISBN.") != string::npos, "salida del menu");
    verificar(salida.find("El libro indicado no es una novela.") != string::npos, "salida del menu");
    verificar(salida.find("No existe un autor con esa cedula.") != string::npos, "salida del menu");
    verificar(salida.find("1000000000001 | El bosque | Novela") != string::npos, "salida del menu");
    verificar(salida.find("1000000000002 | El texto | Texto") != string::npos, "salida del menu");
    verificar(salida.find("Autor: 10000010 | Luis | Lopez") != string::npos, "salida del menu");
    verificar(salida.find("Materia: Matematica") != string::npos, "salida del menu");
    verificar(salida.find("Publicacion: 31/12/2025") != string::npos, "salida del menu");
    verificar(salida.find("Anio escolar: 2") != string::npos, "salida del menu");
    verificar(salida.find("Importe: 80") != string::npos, "salida del menu");
    verificar(salida.find("Importe: 150") != string::npos, "salida del menu");
    verificar(salida.find("Importe: 90") != string::npos, "salida del menu");
    verificar(salida.find("Recaudacion: 400") != string::npos, "salida del menu");
    verificar(salida.find("Unidades vendidas: novelas 2, textos comunes 1, escolares 1") != string::npos, "salida del menu");
    verificar(salida.find("Unidades vendidas: 2") != string::npos, "salida del menu");
    verificar(salida.find("10000010 | Luis | Lopez") < salida.find("10000020 | Ana Maria | Perez"), "menu muestra autores ordenados");
    string largo(101, 'A');
    salida = probarEntradaMenu("abc\n11\n1\n0\n123456789\n1x\n10000001\n\n" + largo + "\nAna\nPerez\n"
                               "2\n2\n1000000000002\nTexto\nE\n-1\nnan\n10x\n10\nM\n31\n2\n2026\n"
                               "28\n2\n2026\n5\n1000000000002\n0\n");
    verificar(salida.find("Ingrese un entero entre 0 y 10") != string::npos, "menu rechaza opcion invalida");
    verificar(salida.find("Ingrese 8 digitos") != string::npos, "menu exige cedula de ocho digitos");
    verificar(salida.find("Ingrese un texto de 1 a 100 caracteres") != string::npos, "menu rechaza texto vacio o largo");
    verificar(salida.find("Ingrese un precio valido") != string::npos, "menu rechaza precio invalido");
    verificar(salida.find("Fecha inexistente.") != string::npos, "menu vuelve a pedir una fecha inexistente");
    verificar(salida.find("Publicacion: 28/2/2026") != string::npos, "alta valida despues de los errores");
    salida = probarEntradaMenu("9\n1..2\n.50\n9\n12.25\n0\n");
    verificar(salida.find("Ingrese un precio valido") != string::npos, "precio con dos puntos rechazado");
    verificar(salida.find("Unidades vendidas: novelas 0, textos comunes 0, escolares 0") != string::npos,
              "precio decimal aceptado");
}

void probarExtremos()
{
    string maximo = "9999999999999";
    verificar(esEntero(maximo), "entero admite trece digitos");
    verificar(convertirEntero(maximo) == 9999999999999L, "conversion exacta de trece digitos");
    verificar(!esEntero(maximo + "0"), "entero rechaza mas de trece digitos antes de convertir");
    verificar(!esPrecio(".") && !esPrecio("1.2.3") && !esPrecio("nan") && !esPrecio("inf"),
              "precio rechaza formato sin numero y valores especiales");
    verificar(esPrecio("0") && esPrecio(".5") && esPrecio("12.25"), "precio admite cero y decimales");
    verificar(mismoImporte(convertirPrecio("12.25"), 12.25f), "conversion decimal conserva valor");
    verificar(!textoValido("   ") && !textoValido("\t\r "), "texto rechaza solo blancos");
    verificar(textoValido(" Ana Maria "), "texto conserva espacios de contenido valido");
    verificar(textoValido(string(100, 'A')) && !textoValido(string(101, 'A')),
              "texto controla ambos lados del limite de longitud");
    const char* incompletas[] = {"", "0", "abc", "1\n", "1\n10000001\nAna\n",
        "2\n1\n1000000000001\nT\nE\n0\n", "3\n1000000000001\n", "4\n",
        "5\n", "7\n", "8\n", "9\n", "10\n"};
    for (unsigned int i = 0; i < sizeof(incompletas) / sizeof(incompletas[0]); i++)
    {
        string salida = probarEntradaMenu(incompletas[i]);
        verificar(salida.find("Operacion realizada.") == string::npos,
                  "EOF termina sin confirmar una operacion incompleta");
    }
    string salida = probarEntradaMenu("9\n" + string(400, '9') + "\n12.25\n0\n");
    verificar(salida.find("Ingrese un precio valido") != string::npos,
              "menu rechaza una linea demasiado larga");
    verificar(salida.find("Unidades vendidas: novelas 0, textos comunes 0, escolares 0") != string::npos,
              "menu continua despues de una linea demasiado larga");
    salida = probarEntradaMenu("1\n10000001\n   \nAna\nPerez\n6\n0\n");
    verificar(salida.find("caracteres con contenido") != string::npos
              && salida.find("10000001 | Ana | Perez") != string::npos,
              "menu repregunta nombre sin contenido y registra el corregido");
    salida = probarEntradaMenu("2\n1\n123456789\n12345678901234\n1000000000001\nT\nE\n10\naventura\n5\n1000000000001\n0\n");
    verificar(salida.find("Ingrese 13 digitos") != string::npos
              && salida.find("ISBN: 1000000000001") != string::npos,
              "menu exige trece digitos de ISBN y conserva la clave completa");
    salida = probarEntradaMenu("2\n9\n3\n1000000000001\n   \nT\n\t\nE\ninf\n10\n \nM\n31\n4\n2026\n30\n4\n2026\n0\n7\n3\n5\n1000000000001\n0\n");
    verificar(salida.find("Ingrese un entero entre 1 y 3") != string::npos,
              "menu rechaza tipo de libro fuera de opciones");
    verificar(salida.find("Fecha inexistente.") != string::npos
              && salida.find("Publicacion: 30/4/2026") != string::npos,
              "menu corrige fecha inexistente sin cancelar el alta");
    verificar(salida.find("Ingrese un entero entre 1 y 6") != string::npos
              && salida.find("Anio escolar: 3") != string::npos,
              "menu rechaza extremos invalidos de anio escolar");
    verificar(salida.find("Titulo: T") != string::npos && salida.find("Editorial: E") != string::npos
              && salida.find("Materia: M") != string::npos,
              "menu corrige blancos en titulo editorial y materia antes de llamar a fachada");
    salida = probarEntradaMenu("1\n10000001\nAna\n   \nPerez\n6\n0\n");
    verificar(salida.find("10000001 | Ana | Perez") != string::npos,
              "menu corrige apellido sin contenido");
    salida = probarEntradaMenu("2\n1\n1000000000001\nT\nE\n10\n   \naventura\n5\n1000000000001\n0\n");
    verificar(salida.find("Genero: aventura") != string::npos,
              "menu corrige genero sin contenido");

}

int main()
{
    probarMenu();
    probarExtremos();
    probarFachada();
    probarMensajesError();
    probarObjeto();
    probarFecha();
    probarAutor();
    probarLibro();
    probarNovela();
    probarTexto();
    probarEscolar();
    probarIterador();
    probarHashLibros();
    probarABBAutores();
    probarColeccionLibros();
    probarColeccionAutores();
    int resultado = 0;
    if (fallos != 0)
    {
        cerr << fallos << " comprobacion(es) fallaron\n";
        resultado = 1;
    }
    else
        cout << "Todas las pruebas: OK\n";
    return (resultado);
}
