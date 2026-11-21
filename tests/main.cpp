#include "Fecha.h"
#include "Autor.h"
#include "Objeto.h"
#include "Novela.h"
#include "Texto.h"
#include "Escolar.h"
#include "ColeccionLibros.h"
#include "ColeccionAutores.h"
#include "HashLibros.h"
#include "ABBAutores.h"
#include "Iterador.h"
#include "MensajesError.h"
#include "Fachada.h"
#include "Menu.h"
#include <sstream>
#include <limits>
#include "TipoError.h"
#include "Libro.h"
#include <iostream>
#include <cstddef>

int fallos = 0;

void verificar(bool condicion, const char* descripcion)
{
    if (!condicion)
    {
        std::cerr << "FALLO: " << descripcion << '\n';
        fallos++;
    }
    else
        std::cout << "OK: " << descripcion << '\n';
}

bool mismoImporte(double obtenido, double esperado)
{
    double diferencia = obtenido - esperado;
    return (diferencia > -0.000001 && diferencia < 0.000001);
}

// Las marcas permiten comprobar la destruccion por punteros a clases base.
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
    verificar(libro->getId() == 2147483647L, "ISBN maximo documentado (2147483647) como identificador de Libro");
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
        double esperadoViejo = anio <= 3 ? 67.5 : 75;
        double esperadoNuevo = anio <= 3 ? 90 : 100;
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
    // estaVacia tambien debe funcionar con elementos en la ultima o primera cubeta.
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

void probarFachada()
{
    Fachada fachada;
    TipoError error;
    Libro* resultado = NULL;
    double importe = 99;
    long int novelas, textos, escolares;
    fachada.consultarLibro(1, resultado, error);
    verificar(error == LIBRO_NO_EXISTE && resultado == NULL, "consulta inexistente");
    fachada.registrarVenta(1, importe, error);
    verificar(error == LIBRO_NO_EXISTE && importe == 0, "venta inexistente sin efecto");
    fachada.asociarAutor(1, 1, error);
    verificar(error == LIBRO_NO_EXISTE, "asociar libro inexistente");
    {
        Iterador iter;
        fachada.listarAutores(iter, error);
        verificar(error == SIN_AUTORES && !iter.hayActual(), "autores vacios en fachada");
    }
    {
        Iterador iter;
        fachada.listarPorPrefijo("El", iter, error);
        verificar(error == SIN_COINCIDENCIAS && !iter.hayActual(), "prefijo sin libros");
    }
    fachada.consultarMasVendido("E", resultado, error);
    verificar(error == SIN_LIBROS_DE_EDITORIAL && resultado == NULL, "editorial inexistente");
    verificar(fachada.calcularRecaudacion("E") == 0, "recaudacion vacia");
    fachada.contarVentasPorTipo(0, novelas, textos, escolares);
    verificar(novelas == 0 && textos == 0 && escolares == 0, "contadores vacios");
    fachada.registrarAutor(0, "A", "B", error);
    verificar(error == DATOS_INVALIDOS, "cedula invalida");
    fachada.registrarAutor(1, "", "B", error);
    verificar(error == DATOS_INVALIDOS, "nombre invalido");
    fachada.registrarAutor(1, "A", "", error);
    verificar(error == DATOS_INVALIDOS, "apellido invalido");
    fachada.registrarAutor(20, "Ana", "Perez", error);
    verificar(error == SIN_ERROR, "alta autor fachada");
    fachada.registrarAutor(10, "Luis", "Lopez", error);
    verificar(error == SIN_ERROR, "segundo autor fachada");
    fachada.registrarAutor(20, "Otro", "Nombre", error);
    verificar(error == AUTOR_YA_EXISTE, "autor duplicado");
    fachada.registrarLibro(TEXTO, 99, "T", "E", 10, "", "M", 31, 2, 2026, 0, error);
    verificar(error == DATOS_INVALIDOS, "fecha invalida no se registra");
    fachada.registrarLibro(NOVELA, 99, "N", "E", std::numeric_limits<double>::infinity(), "aventura", "", 0, 0, 0, 0, error);
    verificar(error == DATOS_INVALIDOS, "precio infinito rechazado");
    fachada.registrarLibro(ESCOLAR, 99, "N", "E", 10, "", "M", 1, 1, 2026, 7, error);
    verificar(error == DATOS_INVALIDOS, "anio escolar invalido rechazado");
    fachada.registrarLibro(NOVELA, 99, "N", "E", 10, "", "", 0, 0, 0, 0, error);
    verificar(error == DATOS_INVALIDOS, "genero vacio rechazado");
    fachada.consultarLibro(99, resultado, error);
    verificar(error == LIBRO_NO_EXISTE, "altas invalidas no registran el libro");
    fachada.registrarLibro(NOVELA, 1, "El bosque", "E", 100, "infantil", "", 0, 0, 0, 0, error);
    verificar(error == SIN_ERROR, "alta novela fachada");
    fachada.registrarLibro(TEXTO, 2, "El texto", "E", 200, "", "M", 31, 12, 2025, 0, error);
    verificar(error == SIN_ERROR, "alta texto fachada");
    fachada.registrarLibro(ESCOLAR, 3, "Escuela", "E", 100, "", "M", 1, 1, 2026, 2, error);
    verificar(error == SIN_ERROR, "alta escolar fachada");
    fachada.registrarLibro(NOVELA, 4, "Otra", "Otra editorial", 500, "aventura", "", 0, 0, 0, 0, error);
    verificar(error == SIN_ERROR, "alta novela de otra editorial");
    Libro* infantil;
    Libro* texto;
    Libro* escolar;
    fachada.consultarLibro(1, infantil, error);
    fachada.consultarLibro(2, texto, error);
    fachada.consultarLibro(3, escolar, error);
    verificar(infantil->getTipo() == NOVELA && texto->getTipo() == TEXTO && escolar->getTipo() == ESCOLAR,
              "la fachada crea el tipo de libro indicado");
    verificar(infantil->getUnidadesVendidas() == 0 && texto->getUnidadesVendidas() == 0
              && escolar->getUnidadesVendidas() == 0, "alta inicia con cero ventas");
    verificar(((Escolar*) escolar)->getAnioEscuela() == 2 && ((Texto*) texto)->getFechaPublicado().getAnio() == 2025,
              "la fachada arma fecha y anio escolar");
    fachada.registrarLibro(NOVELA, 1, "Duplicado", "E", 10, "aventura", "", 0, 0, 0, 0, error);
    verificar(error == LIBRO_YA_EXISTE, "libro duplicado rechazado");
    fachada.consultarLibro(1, resultado, error);
    verificar(error == SIN_ERROR && resultado == infantil && infantil->getTitulo() == "El bosque", "duplicado no reemplaza al original");
    fachada.asociarAutor(2, 20, error);
    verificar(error == LIBRO_NO_ES_NOVELA, "no convertir texto a novela");
    fachada.asociarAutor(1, 99, error);
    verificar(error == AUTOR_NO_EXISTE && ((Novela*) infantil)->getAutor() == NULL, "autor ausente sin efecto");
    fachada.asociarAutor(1, 20, error);
    verificar(error == SIN_ERROR && ((Novela*) infantil)->getAutor()->getId() == 20, "asociar autor existente");
    fachada.asociarAutor(1, 10, error);
    verificar(error == SIN_ERROR && ((Novela*) infantil)->getAutor()->getId() == 10, "reemplazar asociacion");
    {
        Iterador iter;
        fachada.listarAutores(iter, error);
        verificar(error == SIN_ERROR && iter.actual()->getId() == 10, "autores ordenados primero");
        iter.avanzar();
        verificar(iter.actual()->getId() == 20 && ((Autor*) iter.actual())->getNombre() == "Ana", "autor anterior sigue registrado");
        iter.avanzar();
        verificar(!iter.hayActual(), "sin autores duplicados");
    }
    {
        Iterador iter;
        fachada.listarPorPrefijo("El ", iter, error);
        int cantidad = 0;
        while (iter.hayActual())
        {
            verificar(iter.actual() == infantil || iter.actual() == texto, "prefijo exacto");
            cantidad++;
            iter.avanzar();
        }
        verificar(error == SIN_ERROR && cantidad == 2, "dos coincidencias");
    }
    {
        Iterador iter;
        fachada.listarPorPrefijo("el", iter, error);
        verificar(error == SIN_COINCIDENCIAS, "prefijo distingue mayusculas");
    }
    fachada.consultarMasVendido("E", resultado, error);
    verificar(error == SIN_ERROR && resultado->getUnidadesVendidas() == 0, "maximo valido sin ventas y empate");
    fachada.registrarVenta(1, importe, error);
    verificar(error == SIN_ERROR && mismoImporte(importe, 80), "venta infantil");
    fachada.registrarVenta(2, importe, error);
    verificar(error == SIN_ERROR && mismoImporte(importe, 150), "venta texto antiguo");
    fachada.registrarVenta(3, importe, error);
    verificar(error == SIN_ERROR && mismoImporte(importe, 90), "venta escolar");
    fachada.registrarVenta(1, importe, error);
    verificar(error == SIN_ERROR && infantil->getUnidadesVendidas() == 2, "incrementa exactamente una venta");
    fachada.registrarVenta(4, importe, error);
    verificar(error == SIN_ERROR && mismoImporte(importe, 500), "venta otra editorial");
    verificar(mismoImporte(fachada.calcularRecaudacion("E"), 400), "total editorial con descuentos");
    verificar(fachada.calcularRecaudacion("Ausente") == 0, "editorial ausente devuelve cero");
    fachada.contarVentasPorTipo(99, novelas, textos, escolares);
    verificar(novelas == 3 && textos == 1 && escolares == 1, "ventas discriminadas por tipo");
    fachada.contarVentasPorTipo(100, novelas, textos, escolares);
    verificar(novelas == 1 && textos == 1 && escolares == 0, "precio estrictamente mayor");
    fachada.contarVentasPorTipo(500, novelas, textos, escolares);
    verificar(novelas == 0 && textos == 0 && escolares == 0, "reinicia acumuladores");
    fachada.consultarMasVendido("E", resultado, error);
    verificar(error == SIN_ERROR && resultado == infantil, "maximo por unidades no por precio");
    fachada.consultarLibro(999, resultado, error);
    verificar(error == LIBRO_NO_EXISTE && resultado == NULL, "consulta fallida reinicia resultado");
    fachada.consultarMasVendido("Ausente", resultado, error);
    verificar(error == SIN_LIBROS_DE_EDITORIAL && resultado == NULL, "maximo fallido reinicia resultado");
    verificar(mensajeError(DATOS_INVALIDOS) == "Los datos ingresados no son validos.", "mensaje de datos invalidos");
    verificar(mensajeError(LIMITE_VENTAS) == "No se pueden registrar mas ventas para este libro.", "mensaje limite de ventas");
}

string probarEntradaMenu(string entrada)
{
    std::istringstream datos(entrada);
    std::ostringstream salida;
    std::streambuf* entradaOriginal = std::cin.rdbuf(datos.rdbuf());
    std::streambuf* salidaOriginal = std::cout.rdbuf(salida.rdbuf());
    std::cin.clear();
    ejecutarMenu();
    std::cin.rdbuf(entradaOriginal);
    std::cout.rdbuf(salidaOriginal);
    std::cin.clear();
    return (salida.str());
}

void probarMenu()
{
    string salida = probarEntradaMenu("6\n5\n999\n7\n999\n4\nAusente\n10\nAusente\n1\n20\nAna Maria\nPerez\n1\n10\nLuis\nLopez\n1\n20\nOtro\nAutor\n2\n1\n1\nEl bosque\nE\n100\ninfantil\n2\n2\n2\nEl texto\nE\n200\nMatematica\n31\n12\n2025\n2\n3\n3\nEscuela\nE\n100\nHistoria\n1\n1\n2026\n2\n2\n1\n1\nDuplicado\nE\n10\naventura\n3\n999\n20\n3\n2\n20\n3\n1\n999\n3\n1\n20\n3\n1\n10\n4\nEl \n5\n1\n5\n2\n5\n3\n6\n7\n1\n7\n2\n7\n3\n7\n1\n8\nE\n9\n99\n10\nE\n0\n");
    verificar(salida.find("No hay autores registrados.") != string::npos, "salida del menu");
    verificar(salida.find("No existe un libro con ese ISBN.") != string::npos, "salida del menu");
    verificar(salida.find("Ningun titulo comienza con esa frase.") != string::npos, "salida del menu");
    verificar(salida.find("No hay libros registrados de esa editorial.") != string::npos, "salida del menu");
    verificar(salida.find("Ya existe un autor con esa cedula.") != string::npos, "salida del menu");
    verificar(salida.find("Ya existe un libro con ese ISBN.") != string::npos, "salida del menu");
    verificar(salida.find("El libro indicado no es una novela.") != string::npos, "salida del menu");
    verificar(salida.find("No existe un autor con esa cedula.") != string::npos, "salida del menu");
    verificar(salida.find("1 | El bosque | Novela") != string::npos, "salida del menu");
    verificar(salida.find("2 | El texto | Texto") != string::npos, "salida del menu");
    verificar(salida.find("Autor: 10 | Luis | Lopez") != string::npos, "salida del menu");
    verificar(salida.find("Materia: Matematica") != string::npos, "salida del menu");
    verificar(salida.find("Publicacion: 31/12/2025") != string::npos, "salida del menu");
    verificar(salida.find("Anio escolar: 2") != string::npos, "salida del menu");
    verificar(salida.find("Importe: 80") != string::npos, "salida del menu");
    verificar(salida.find("Importe: 150") != string::npos, "salida del menu");
    verificar(salida.find("Importe: 90") != string::npos, "salida del menu");
    verificar(salida.find("Recaudacion: 400") != string::npos, "salida del menu");
    verificar(salida.find("Unidades vendidas: novelas 2, textos comunes 1, escolares 1") != string::npos, "salida del menu");
    verificar(salida.find("Unidades vendidas: 2") != string::npos, "salida del menu");
    verificar(salida.find("10 | Luis | Lopez") < salida.find("20 | Ana Maria | Perez"), "menu muestra autores ordenados");
    string largo(101, 'A');
    salida = probarEntradaMenu("abc\n11\n1\n0\n1234567890\n1x\n1\n\n" + largo + "\nAna\nPerez\n"
                               "2\n2\n2\nTexto\nE\n-1\nnan\n10x\n10\nM\n31\n2\n2026\n"
                               "2\n2\n2\nTexto\nE\n10\nM\n28\n2\n2026\n5\n2\n0\n");
    verificar(salida.find("Ingrese un entero entre 0 y 10") != string::npos, "menu rechaza opcion invalida");
    verificar(salida.find("Ingrese un entero entre 1 y 999999999") != string::npos, "menu rechaza cedula de 10 digitos");
    verificar(salida.find("Ingrese un texto de 1 a 100 caracteres") != string::npos, "menu rechaza texto vacio o largo");
    verificar(salida.find("Ingrese un precio valido") != string::npos, "menu rechaza precio invalido");
    verificar(salida.find("Los datos ingresados no son validos.") != string::npos, "fachada rechaza fecha inexistente");
    verificar(salida.find("Publicacion: 28/2/2026") != string::npos, "alta valida despues de los errores");
    salida = probarEntradaMenu("9\n1..2\n.50\n9\n12.25\n0\n");
    verificar(salida.find("Ingrese un precio valido") != string::npos, "precio con dos puntos rechazado");
    verificar(salida.find("Unidades vendidas: novelas 0, textos comunes 0, escolares 0") != string::npos,
              "precio decimal aceptado");
}

int main()
{
    probarMenu();
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
        std::cerr << fallos << " comprobacion(es) fallaron\n";
        resultado = 1;
    }
    else
        std::cout << "Todas las pruebas: OK\n";
    return (resultado);
}
