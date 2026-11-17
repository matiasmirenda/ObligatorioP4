#include "Fecha.h"
#include "Autor.h"
#include "Novela.h"
#include "Texto.h"
#include "Escolar.h"
#include "ColeccionLibros.h"
#include "ColeccionAutores.h"
#include "HashLibros.h"
#include "ABBAutores.h"
#include <iostream>
#include <cstddef>

int fallos = 0;
void verificar(bool condicion, const char* descripcion) {
    if (!condicion) {
        std::cerr << "FALLO: " << descripcion << '\n';
        ++fallos;
    }
}

void probarFecha() {
    Fecha inicial;
    verificar(inicial == Fecha(1, 1, 2000), "constructor por defecto");
    Fecha copia(inicial);
    verificar(copia == inicial, "constructor de copia");
    Fecha asignada(2, 1, 2000);
    asignada = inicial;
    verificar(asignada == inicial, "asignacion de componentes por valor");
    verificar(!Fecha(31, 12, 1899).esValida(), "anio anterior al limite");
    Fecha fecha(29, 2, 2024);
    verificar(fecha.getDia() == 29 && fecha.getMes() == 2 && fecha.getAnio() == 2024,
              "consulta de componentes");
    verificar(Fecha(29, 2, 2024).esValida(), "bisiesto divisible por cuatro");
    verificar(Fecha(29, 2, 2000).esValida(), "bisiesto divisible por cuatrocientos");
    verificar(!Fecha(29, 2, 1900).esValida(), "siglo no bisiesto");
    verificar(!Fecha(29, 2, 2023).esValida(), "febrero no bisiesto");
    verificar(Fecha(30, 4, 2024).esValida(), "mes de treinta dias");
    verificar(!Fecha(31, 4, 2024).esValida(), "dia fuera del mes");
    verificar(Fecha(31, 12, 2099).esValida(), "limite superior");
    verificar(!Fecha(1, 1, 0).esValida(), "anio cero");
    verificar(!Fecha(1, 1, 2100).esValida(), "anio fuera de rango");
    verificar(!Fecha(1, 0, 2024).esValida(), "mes cero");
    verificar(!Fecha(1, 13, 2024).esValida(), "mes fuera de rango");
    verificar(!Fecha(0, 1, 2024).esValida(), "dia cero");
    verificar((Fecha(31, 12, 2023) < fecha), "compara anios");
    verificar((Fecha(31, 1, 2024) < fecha), "compara meses");
    verificar((Fecha(28, 2, 2024) < fecha), "compara dias");
    verificar((fecha > Fecha(28, 2, 2024)), "fecha mayor");
    verificar(!(fecha > fecha) && !(fecha < fecha), "igualdad no es mayor ni menor");
    verificar((fecha == Fecha(29, 2, 2024)), "fechas iguales");
    verificar(!(fecha == Fecha(28, 2, 2024)), "fechas diferentes");
    verificar(Fecha(1, 1, 1900).esValida(), "limite inferior de anio");
    verificar(!Fecha(-1, 1, 2024).esValida(), "dia negativo");
    verificar(!Fecha(32, 1, 2024).esValida(), "dia superior a treinta y uno");
    verificar(!Fecha(1, -1, 2024).esValida(), "mes negativo");
    const int limites[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (int mes = 1; mes <= 12; ++mes) {
        verificar(Fecha(limites[mes - 1], mes, 2023).esValida(), "ultimo dia de cada mes");
        verificar(!Fecha(limites[mes - 1] + 1, mes, 2023).esValida(), "dia posterior al fin de mes");
    }
    verificar(!(fecha < Fecha(1, 1, 2023)), "orden inverso por anio");
    verificar(!(fecha < Fecha(1, 1, 2024)), "orden inverso por mes");
    verificar(!(fecha == Fecha(29, 3, 2024)), "diferencia de mes");
    verificar(!(fecha == Fecha(29, 2, 2028)), "diferencia de anio");
}

void probarAutor() {
    Autor vacio;
    verificar(vacio.getCedula() == 0 && vacio.getNombre() == "" &&
              vacio.getApellido() == "" && !vacio.datosValidos(), "autor por defecto");
    verificar(Autor(12345678, "Ana", "Perez").datosValidos(), "datos validos");
    verificar(!Autor(0, "Ana", "Perez").datosValidos(), "cedula cero");
    verificar(!Autor(-1, "Ana", "Perez").datosValidos(), "cedula negativa");
    verificar(!Autor(12345678, "", "Perez").datosValidos(), "nombre vacio");
    verificar(!Autor(12345678, "Ana", "").datosValidos(), "apellido vacio");

    Autor original(12345678, "Ana", "Perez");
    verificar(original.getCedula() == 12345678, "consulta de cedula");
    verificar(original.getNombre() == "Ana", "nombre inicial");
    verificar(original.getApellido() == "Perez", "apellido inicial");

    Autor copia(original);
    Autor asignado(87654321, "Juan", "Garcia");
    asignado = original;
    asignado.setNombre("Otro");
    verificar(original.getNombre() == "Ana", "asignacion de string independiente");
    copia.setNombre("Maria");
    copia.setApellido("Lopez");
    verificar(copia.getNombre() == "Maria" && copia.getApellido() == "Lopez",
              "modificadores");
    verificar(copia.getCedula() == original.getCedula(), "clave estable");
    verificar(original.getNombre() == "Ana" && original.getApellido() == "Perez",
              "copia por valor independiente");

    Autor* propietario = new Autor(87654321, "Juan", "Garcia");
    Autor* referencia = propietario;
    referencia->setNombre("Pedro");
    verificar(propietario->getNombre() == "Pedro", "dos punteros al mismo autor");
    delete propietario;
    // referencia ya no puede utilizarse: no se elimina por segunda vez.
    referencia = NULL;
    propietario = NULL;

}


bool mismoImporte(double obtenido, double esperado)
{
    double diferencia = obtenido - esperado;
    return diferencia > -0.000001 && diferencia < 0.000001;
}

// Permite observar que delete mediante Libro* ejecuta el destructor derivado.
class NovelaControlada : public Novela
{
private:
    bool* destruida;
public:
    NovelaControlada(long long isbn, bool* marca)
        : Novela(isbn, "Control", "Editorial", 100, "aventura")
    {
        destruida = marca;
        *destruida = false;
    }
    ~NovelaControlada() { *destruida = true; }
};

void probarLibro()
{
    Novela novela(1234567890123LL, "El camino", "Editorial", 100, "aventura");
    Libro* libro = &novela;
    verificar(libro->getIsbn() == 1234567890123LL, "ISBN de trece digitos");
    verificar(libro->getTitulo() == "El camino" && libro->getEditorial() == "Editorial",
              "datos comunes del libro");
    verificar(libro->getUnidadesVendidas() == 0, "libro nuevo sin ventas");
    libro->registrarVenta();
    libro->registrarVenta();
    verificar(libro->getUnidadesVendidas() == 2, "incrementa una unidad por venta");
    verificar(libro->comienzaCon("El") && libro->comienzaCon("El camino"), "prefijo exacto");
    verificar(!libro->comienzaCon("camino") && !libro->comienzaCon("el"),
              "prefijo no es subcadena y distingue mayusculas");
    verificar(!libro->comienzaCon("El camino largo") && libro->comienzaCon(""),
              "prefijo largo y prefijo vacio");
    libro->setTitulo("Otro titulo");
    libro->setEditorial("Otra editorial");
    libro->setPrecioBase(0);
    verificar(libro->getTitulo() == "Otro titulo" && libro->getEditorial() == "Otra editorial",
              "modificadores comunes");
    verificar(libro->datosValidos() && libro->getPrecioBase() == 0, "precio cero permitido");
    verificar(!Novela(0, "T", "E", 1, "g").datosValidos(), "ISBN cero invalido");
    verificar(!Novela(-1, "T", "E", 1, "g").datosValidos(), "ISBN negativo invalido");
    verificar(!Novela(1, "", "E", 1, "g").datosValidos(), "titulo vacio invalido");
    verificar(!Novela(1, "T", "", 1, "g").datosValidos(), "editorial vacia invalida");
    verificar(!Novela(1, "T", "E", -1, "g").datosValidos(), "precio negativo invalido");
    bool destruida = false;
    Libro* dinamico = new NovelaControlada(45, &destruida);
    delete dinamico;
    verificar(destruida, "destructor virtual mediante Libro*");
}

void probarNovela()
{
    ColeccionAutores autores;
    Novela infantil(1, "Cuentos", "Editorial", 100, "infantil");
    Libro* libro = &infantil;
    verificar(libro->getTipo() == "Novela" && infantil.getGenero() == "infantil",
              "tipo y genero de novela");
    verificar(mismoImporte(libro->calcularPrecioVenta(), 80), "descuento infantil veinte por ciento");
    verificar(infantil.getAutor() == NULL, "novela sin autor inicial");
    infantil.setGenero("aventura");
    verificar(mismoImporte(libro->calcularPrecioVenta(), 100), "novela comun sin descuento");
    verificar(!Novela(2, "T", "E", 10, "").datosValidos(), "genero vacio invalido");
    Autor* primero = new Autor(10, "Ana", "Perez");
    Autor* segundo = new Autor(20, "Juan", "Lopez");
    verificar(primero->datosValidos() && !autores.existe(10), "precondiciones primer autor");
    autores.insertar(primero);
    verificar(segundo->datosValidos() && !autores.existe(20), "precondiciones segundo autor");
    autores.insertar(segundo);
    verificar(autores.getCantidad() == 2, "autores registrados");
    {
        Novela otra(3, "Otra", "E", 10, "aventura");
        infantil.asignarAutor(primero);
        otra.asignarAutor(primero);
        verificar(infantil.getAutor() == otra.getAutor(), "dos novelas comparten autor");
        infantil.asignarAutor(segundo);
        verificar(infantil.getAutor() == segundo && otra.getAutor() == primero,
                  "reemplazo de asociacion independiente");
        Novela copia(otra);
        verificar(copia.getAutor() == primero && copia.getGenero() == otra.getGenero(),
                  "copia de novela conserva referencia prestada");
        otra.registrarVenta();
        Novela conVentas(otra);
        verificar(conVentas.getUnidadesVendidas() == 1, "copia conserva ventas");
        copia.setTitulo("Copia");
        verificar(otra.getTitulo() == "Otra", "copia no modifica titulo original");
    }
    verificar(autores.buscar(10) == primero && primero->getNombre() == "Ana",
              "destruir novelas no destruye autor");
}

void probarTexto()
{
    Texto anterior(1, "Matematica", "E", 100, "Matematica", Fecha(31, 12, 2025));
    Texto corte(2, "Fisica", "E", 100, "Fisica", Fecha(1, 1, 2026));
    Texto posterior(3, "Quimica", "E", 100, "Quimica", Fecha(2, 1, 2026));
    Libro* libro = &anterior;
    verificar(mismoImporte(libro->calcularPrecioVenta(), 75), "texto anterior tiene descuento");
    verificar(mismoImporte(corte.calcularPrecioVenta(), 100), "fecha de corte no tiene descuento");
    verificar(mismoImporte(posterior.calcularPrecioVenta(), 100), "texto posterior sin descuento");
    verificar(libro->getTipo() == "Texto" && anterior.getMateria() == "Matematica",
              "tipo y materia de texto");
    verificar(anterior.getFechaPublicado() == Fecha(31, 12, 2025), "consulta fecha publicada");
    anterior.setMateria("Historia");
    anterior.setFechaPublicado(Fecha(1, 1, 2026));
    Texto copia(anterior);
    verificar(copia.getMateria() == "Historia" && copia.getFechaPublicado() == Fecha(1, 1, 2026),
              "copia y modificadores de texto");
    verificar(copia.datosValidos(), "texto valido");
    verificar(!Texto(1, "T", "E", 1, "", Fecha()).datosValidos(), "materia vacia invalida");
    verificar(!Texto(1, "T", "E", 1, "M", Fecha(31, 2, 2024)).datosValidos(),
              "fecha de publicacion invalida");
    verificar(!Texto(0, "T", "E", 1, "M", Fecha()).datosValidos(), "texto con ISBN invalido");
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
        verificar(mismoImporte(libro->calcularPrecioVenta(), esperadoViejo), "descuentos escolares combinados");
        verificar(mismoImporte(nuevo.calcularPrecioVenta(), esperadoNuevo), "descuento escolar desde corte");
        verificar(libro->getTipo() == "Escolar", "tipo escolar por polimorfismo");
    }
    Escolar escolar(3, "Escolar", "E", 100, "M", Fecha(), 1);
    escolar.setAnioEscuela(6);
    Escolar copia(escolar);
    verificar(copia.getAnioEscuela() == 6 && copia.getMateria() == "M", "copia y modificador escolar");
    verificar(!Escolar(1, "T", "E", 1, "M", Fecha(), 0).datosValidos(), "anio escolar cero");
    verificar(!Escolar(1, "T", "E", 1, "M", Fecha(), 7).datosValidos(), "anio escolar superior a seis");
    verificar(!Escolar(1, "T", "E", 1, "", Fecha(), 1).datosValidos(), "escolar con datos de texto invalidos");
}

void probarColeccionLibros()
{
    ColeccionLibros libros;
    verificar(libros.estaVacia() && libros.getCantidad() == 0, "coleccion de libros inicialmente vacia");
    verificar(!libros.existe(99) && !libros.existe(0) && !libros.existe(-1), "consulta de claves ausentes");
    Libro* nulo = NULL;
    bool puedeInsertar = nulo != NULL;
    verificar(!puedeInsertar, "llamador descarta puntero nulo sin invocar insertar");
    Novela* invalida = new Novela(9, "", "E", 1, "aventura");
    puedeInsertar = invalida != NULL && invalida->datosValidos() && !libros.existe(invalida->getIsbn());
    verificar(!puedeInsertar && libros.getCantidad() == 0, "llamador descarta datos invalidos");
    delete invalida;
    bool destruido1 = false, destruido2 = false, destruido3 = false;
    {
        ColeccionLibros colisiones;
        Libro* uno = new NovelaControlada(1, &destruido1);
        Libro* dos = new NovelaControlada(1010, &destruido2);
        Libro* tres = new NovelaControlada(2019, &destruido3);
        verificar(uno->datosValidos() && !colisiones.existe(uno->getIsbn()), "precondiciones libro uno");
        colisiones.insertar(uno);
        verificar(dos->datosValidos() && !colisiones.existe(dos->getIsbn()), "precondiciones libro dos");
        colisiones.insertar(dos);
        verificar(tres->datosValidos() && !colisiones.existe(tres->getIsbn()), "precondiciones libro tres");
        colisiones.insertar(tres);
        verificar(!colisiones.estaVacia() && colisiones.getCantidad() == 3, "cantidad en hash");
        verificar(colisiones.existe(1) && colisiones.existe(1010) && colisiones.existe(2019), "claves de cadena existen");
        verificar(colisiones.buscar(1) == uno && colisiones.buscar(1010) == dos &&
                  colisiones.buscar(2019) == tres, "busca primero medio y ultimo de cadena");
        verificar(!colisiones.existe(3028), "faltante con colision se consulta con existe");
        verificar(colisiones.existe(uno->getIsbn()), "llamador evita reinsertar el mismo puntero");
        // uno ya es propiedad de la coleccion: no volver a insertarlo ni eliminarlo.
        Novela* repetida = new Novela(1010, "Repetido", "E", 1, "aventura");
        puedeInsertar = repetida != NULL && repetida->datosValidos() && !colisiones.existe(repetida->getIsbn());
        verificar(!puedeInsertar && colisiones.getCantidad() == 3, "llamador detecta ISBN repetido");
        verificar(repetida->getTitulo() == "Repetido", "candidato no insertado sigue siendo del llamador");
        delete repetida;
        IteradorLibros* iter = colisiones.crearIterador();
        int vistos = 0;
        bool visto1 = false, visto2 = false, visto3 = false;
        while (iter->hayActual())
        {
            Libro* actual = iter->actual();
            if (actual == uno) visto1 = true;
            if (actual == dos) visto2 = true;
            if (actual == tres) visto3 = true;
            vistos++;
            iter->avanzar();
        }
        verificar(vistos == 3 && visto1 && visto2 && visto3, "iterador visita cada libro de cadena");
        iter->reiniciar();
        verificar(iter->hayActual(), "reiniciar iterador no vacio");
        delete iter;
        verificar(!destruido1 && !destruido2 && !destruido3, "iterador no elimina libros");
        verificar(colisiones.buscar(1) == uno, "libro sigue consultable al eliminar iterador");
    }
    verificar(destruido1 && destruido2 && destruido3, "coleccion elimina libros mediante destructor virtual");
}

void probarColeccionAutores()
{
    ColeccionAutores autores;
    verificar(autores.estaVacia() && autores.getCantidad() == 0, "coleccion autores inicialmente vacia");
    verificar(!autores.existe(10), "autor inexistente se consulta con existe");
    Autor* nulo = NULL;
    verificar(nulo == NULL, "llamador descarta autor nulo sin insertar");
    Autor* invalido = new Autor(0, "Ana", "Perez");
    verificar(!invalido->datosValidos(), "llamador detecta datos de autor invalidos");
    delete invalido;
    long cedulas[7] = {40, 20, 60, 10, 30, 50, 70};
    for (int i = 0; i < 7; i++)
    {
        Autor* candidato = new Autor(cedulas[i], "Nombre", "Apellido");
        verificar(candidato->datosValidos() && !autores.existe(cedulas[i]), "precondiciones para insertar autor");
        autores.insertar(candidato);
    }
    verificar(!autores.estaVacia() && autores.getCantidad() == 7, "cantidad de autores");
    for (int i = 0; i < 7; i++)
        verificar(autores.existe(cedulas[i]) && autores.buscar(cedulas[i])->getCedula() == cedulas[i],
                  "busca raiz y descendientes");
    verificar(!autores.existe(5) && !autores.existe(65) && !autores.existe(80), "faltantes a izquierda y derecha");
    Autor* original = autores.buscar(20);
    verificar(autores.existe(original->getCedula()), "no reinsertar autor que ya pertenece a la coleccion");
    Autor* duplicado = new Autor(20, "Otro", "Apellido");
    bool puedeInsertar = duplicado != NULL && duplicado->datosValidos() && !autores.existe(duplicado->getCedula());
    verificar(!puedeInsertar && autores.buscar(20) == original && autores.getCantidad() == 7,
              "llamador detecta cedula repetida sin cambiar coleccion");
    verificar(duplicado->getNombre() == "Otro", "candidato no insertado conserva propiedad del llamador");
    delete duplicado;
    IteradorAutores* iter = autores.crearIterador();
    long esperado = 10;
    while (iter->hayActual())
    {
        verificar(iter->actual()->getCedula() == esperado, "recorrido por cedula ascendente");
        esperado += 10;
        iter->avanzar();
    }
    verificar(esperado == 80, "recorrido visita siete autores");
    iter->reiniciar();
    verificar(iter->hayActual() && iter->actual()->getCedula() == 10, "reinicia desde menor cedula");
    delete iter;
    verificar(autores.buscar(20) == original && original->getNombre() == "Nombre", "iterador no elimina autores");
    ColeccionAutores creciente;
    ColeccionAutores decreciente;
    for (int i = 1; i <= 4; i++)
    {
        creciente.insertar(new Autor(i, "N", "A"));
        decreciente.insertar(new Autor(5 - i, "N", "A"));
    }
    verificar(creciente.existe(4) && decreciente.existe(1), "existen extremos de ABB degenerados");
    verificar(creciente.buscar(4)->getCedula() == 4 && decreciente.buscar(1)->getCedula() == 1,
              "busca extremos de ABB degenerados");
}





void probarHashLibros()
{
    bool destruidoPrimero = false, destruidoSegundo = false, destruidoTercero = false;
    {
        HashLibros hash;
        verificar(hash.estaVacia() && hash.getCantidad() == 0, "HashLibros directo inicia vacio");
        verificar(!hash.existe(1) && !hash.existe(0) && !hash.existe(-1009), "HashLibros existe admite claves ausentes");
        IteradorLibros* vacio = hash.crearIterador();
        verificar(!vacio->hayActual(), "HashLibros crea recorrido vacio");
        vacio->reiniciar();
        verificar(!vacio->hayActual(), "HashLibros reinicia recorrido vacio");
        delete vacio;
        Libro* nulo = NULL;
        verificar(nulo == NULL, "llamador descarta NULL antes de insertar en hash");
        Novela* invalida = new Novela(5, "", "E", 10, "aventura");
        verificar(!invalida->datosValidos() && hash.getCantidad() == 0, "datos invalidos no se insertan");
        invalida->setTitulo("Reparado");
        verificar(invalida->datosValidos(), "candidato no transferido sigue vivo");
        delete invalida;
        Libro* primero = new NovelaControlada(1, &destruidoPrimero);
        Libro* segundo = new NovelaControlada(1010, &destruidoSegundo);
        Libro* tercero = new NovelaControlada(2019, &destruidoTercero);
        verificar(primero->datosValidos() && !hash.existe(1), "precondiciones primer ISBN hash");
        hash.insertar(primero);
        verificar(segundo->datosValidos() && !hash.existe(1010), "precondiciones segundo ISBN hash");
        hash.insertar(segundo);
        verificar(tercero->datosValidos() && !hash.existe(2019), "precondiciones tercer ISBN hash");
        hash.insertar(tercero);
        Libro* distinto = new Texto(500, "Fisica", "E", 100, "Fisica", Fecha(1, 1, 2026));
        verificar(distinto->datosValidos() && !hash.existe(500), "precondiciones libro en otra cubeta");
        hash.insertar(distinto);
        verificar(!hash.estaVacia() && hash.getCantidad() == 4, "HashLibros cantidad tras inserciones");
        verificar(hash.existe(1) && hash.existe(1010) && hash.existe(2019) && hash.existe(500), "HashLibros claves insertadas");
        verificar(hash.buscar(1) == primero && hash.buscar(1010) == segundo &&
                  hash.buscar(2019) == tercero && hash.buscar(500) == distinto, "HashLibros recupera cada puntero");
        verificar(!hash.existe(3028), "HashLibros faltante en cadena se verifica con existe");
        verificar(hash.existe(primero->getIsbn()) && !destruidoPrimero, "mismo puntero no debe reinsertarse");
        Novela* duplicada = new Novela(1010, "Duplicada", "E", 20, "aventura");
        bool puedeInsertar = duplicada != NULL && duplicada->datosValidos() && !hash.existe(duplicada->getIsbn());
        verificar(!puedeInsertar && hash.getCantidad() == 4 && hash.buscar(1010) == segundo,
                  "llamador detecta duplicado antes de insertar");
        duplicada->setTitulo("Sigue viva");
        verificar(duplicada->getTitulo() == "Sigue viva", "candidato no transferido pertenece al llamador");
        delete duplicada;
        IteradorLibros* iter = hash.crearIterador();
        primero->setTitulo("Actualizado");
        int vistos = 0;
        int vecesPrimero = 0, vecesSegundo = 0, vecesTercero = 0, vecesDistinto = 0;
        while (iter->hayActual())
        {
            Libro* actual = iter->actual();
            if (actual == primero)
            {
                vecesPrimero++;
                verificar(actual->getTitulo() == "Actualizado", "snapshot hash guarda referencias");
            }
            if (actual == segundo) vecesSegundo++;
            if (actual == tercero) vecesTercero++;
            if (actual == distinto) vecesDistinto++;
            vistos++;
            iter->avanzar();
        }
        verificar(vistos == 4 && vecesPrimero == 1 && vecesSegundo == 1 &&
                  vecesTercero == 1 && vecesDistinto == 1, "HashLibros visita cada objeto una vez");
        iter->reiniciar();
        verificar(iter->hayActual(), "HashLibros recorrido puede repetirse");
        delete iter;
        verificar(!destruidoPrimero && !destruidoSegundo && !destruidoTercero, "iterador no elimina libros de hash");
        verificar(hash.buscar(1) == primero && primero->getTitulo() == "Actualizado", "hash sigue funcionando tras eliminar iterador");
    }
    verificar(destruidoPrimero && destruidoSegundo && destruidoTercero, "HashLibros destruye libros con destructor virtual");
}

void probarABBAutores()
{
    ABBAutores arbol;
    verificar(arbol.estaVacia() && arbol.getCantidad() == 0, "ABBAutores directo inicia vacio");
    verificar(!arbol.existe(40), "ABBAutores inexistente se consulta con existe");
    IteradorAutores* vacio = arbol.crearIterador();
    verificar(!vacio->hayActual(), "ABBAutores recorrido vacio");
    vacio->reiniciar();
    verificar(!vacio->hayActual(), "ABBAutores reinicia recorrido vacio");
    delete vacio;
    Autor* invalido = new Autor(0, "Ana", "Perez");
    bool puedeInsertar = invalido != NULL && invalido->datosValidos() && !arbol.existe(invalido->getCedula());
    verificar(!puedeInsertar && arbol.getCantidad() == 0, "llamador descarta datos invalidos antes del ABB");
    verificar(invalido->getNombre() == "Ana", "candidato no insertado sigue vivo");
    delete invalido;
    long cedulas[7] = {40, 20, 60, 10, 30, 50, 70};
    Autor* registrados[7];
    for (int i = 0; i < 7; i++)
    {
        registrados[i] = new Autor(cedulas[i], "Nombre", "Apellido");
        verificar(registrados[i]->datosValidos() && !arbol.existe(cedulas[i]), "precondiciones insertar en ABB");
        arbol.insertar(registrados[i]);
    }
    verificar(!arbol.estaVacia() && arbol.getCantidad() == 7, "ABBAutores cantidad de nodos");
    for (int i = 0; i < 7; i++)
        verificar(arbol.existe(cedulas[i]) && arbol.buscar(cedulas[i]) == registrados[i], "ABBAutores recupera referencias");
    verificar(!arbol.existe(5) && !arbol.existe(25) && !arbol.existe(65) &&
              !arbol.existe(80) && !arbol.existe(-1), "ABBAutores faltantes en extremos y entre nodos");
    verificar(arbol.existe(registrados[1]->getCedula()) && arbol.buscar(20) == registrados[1],
              "llamador evita reinsertar el mismo autor");
    Autor* repetido = new Autor(20, "Otro", "Autor");
    puedeInsertar = repetido != NULL && repetido->datosValidos() && !arbol.existe(repetido->getCedula());
    verificar(!puedeInsertar && arbol.getCantidad() == 7 && arbol.buscar(20) == registrados[1],
              "llamador detecta cedula duplicada sin invocar insertar");
    repetido->setNombre("Rechazado vivo");
    verificar(repetido->getNombre() == "Rechazado vivo", "candidato no transferido sigue siendo del llamador");
    delete repetido;
    IteradorAutores* iter = arbol.crearIterador();
    registrados[0]->setNombre("Actualizado");
    long esperado = 10;
    int vistos = 0;
    while (iter->hayActual())
    {
        Autor* actual = iter->actual();
        verificar(actual->getCedula() == esperado, "ABBAutores carga recorrido ascendente");
        if (actual->getCedula() == 40)
            verificar(actual == registrados[0] && actual->getNombre() == "Actualizado", "snapshot ABB guarda referencias");
        esperado += 10;
        vistos++;
        iter->avanzar();
    }
    verificar(vistos == 7 && esperado == 80, "ABBAutores recorre todos los autores");
    iter->reiniciar();
    verificar(iter->hayActual() && iter->actual() == registrados[3], "ABBAutores reinicia desde minimo");
    delete iter;
    verificar(arbol.buscar(40) == registrados[0] && registrados[0]->getNombre() == "Actualizado", "iterador no elimina autores");
    {
        Novela asociada(123, "Prueba", "E", 50, "aventura");
        asociada.asignarAutor(arbol.buscar(40));
        verificar(asociada.getAutor() == registrados[0], "ABBAutores comparte autor sin copiar");
    }
    verificar(arbol.buscar(40)->getNombre() == "Actualizado", "novela no libera autor del ABB");
    ABBAutores creciente;
    ABBAutores decreciente;
    for (int i = 1; i <= 5; i++)
    {
        creciente.insertar(new Autor(i, "N", "A"));
        decreciente.insertar(new Autor(6 - i, "N", "A"));
    }
    verificar(creciente.existe(5) && decreciente.existe(1), "ABBAutores extremos de cadenas existen");
    verificar(creciente.buscar(5)->getCedula() == 5 && decreciente.buscar(1)->getCedula() == 1,
              "ABBAutores busca extremos aun si se degenera");
    IteradorAutores* ordenados = decreciente.crearIterador();
    for (int i = 1; i <= 5; i++)
    {
        verificar(ordenados->hayActual(), "ABBAutores degenerado conserva todos los nodos");
        if (ordenados->hayActual())
        {
            verificar(ordenados->actual()->getCedula() == i, "ABBAutores degenerado recorre en orden");
            ordenados->avanzar();
        }
    }
    verificar(!ordenados->hayActual(), "ABBAutores degenerado fin de recorrido");
    delete ordenados;
}

void probarIteradorLibros()
{
    ColeccionLibros vacia;
    IteradorLibros* desdeColeccion = vacia.crearIterador();
    verificar(!desdeColeccion->hayActual(), "iterador de coleccion de libros vacia");
    desdeColeccion->reiniciar();
    verificar(!desdeColeccion->hayActual(), "reiniciar recorrido de coleccion de libros vacia");
    delete desdeColeccion;
    Novela primero(1, "Primero", "E", 10, "aventura");
    Texto segundo(2, "Segundo", "E", 20, "M", Fecha());
    {
        IteradorLibros iter;
        verificar(!iter.hayActual(), "IteradorLibros inicia vacio");
        iter.reiniciar();
        verificar(!iter.hayActual(), "IteradorLibros reinicia vacio");
        iter.insertar(&primero);
        iter.insertar(&segundo);
        verificar(iter.hayActual() && iter.actual() == &primero,
                  "IteradorLibros conserva orden de carga");
        primero.setTitulo("Cambio");
        verificar(iter.actual()->getTitulo() == "Cambio", "IteradorLibros conserva referencia al objeto");
        iter.avanzar();
        verificar(iter.hayActual() && iter.actual() == &segundo,
                  "IteradorLibros avanza al segundo");
        iter.avanzar();
        verificar(!iter.hayActual(), "IteradorLibros termina sin recorrido circular");
        iter.reiniciar();
        verificar(iter.hayActual() && iter.actual() == &primero,
                  "IteradorLibros reinicia desde primero");
    }
    verificar(primero.getTitulo() == "Cambio" && segundo.getTitulo() == "Segundo",
              "IteradorLibros destruye nodos propios, no libros prestados");
    bool destruida = false;
    Libro* dinamico = new NovelaControlada(10, &destruida);
    {
        IteradorLibros iter;
        iter.insertar(dinamico);
    }
    verificar(!destruida, "IteradorLibros no elimina libro dinamico");
    delete dinamico;
    verificar(destruida, "propietario original puede eliminar libro una sola vez");
}

void probarIteradorAutores()
{
    ColeccionAutores vacia;
    IteradorAutores* desdeColeccion = vacia.crearIterador();
    verificar(!desdeColeccion->hayActual(), "iterador de coleccion de autores vacia");
    desdeColeccion->reiniciar();
    verificar(!desdeColeccion->hayActual(), "reiniciar recorrido de coleccion de autores vacia");
    delete desdeColeccion;
    Autor primero(10, "Primero", "A");
    Autor segundo(20, "Segundo", "A");
    {
        IteradorAutores iter;
        verificar(!iter.hayActual(), "IteradorAutores inicia vacio");
        iter.reiniciar();
        verificar(!iter.hayActual(), "IteradorAutores reinicia vacio");
        iter.insertar(&primero);
        iter.insertar(&segundo);
        verificar(iter.hayActual() && iter.actual() == &primero,
                  "IteradorAutores conserva orden de carga");
        primero.setNombre("Cambio");
        verificar(iter.actual()->getNombre() == "Cambio", "IteradorAutores mantiene referencia");
        iter.avanzar();
        verificar(iter.hayActual() && iter.actual() == &segundo,
                  "IteradorAutores avanza al segundo");
        iter.avanzar();
        verificar(!iter.hayActual(), "IteradorAutores termina recorrido");
        iter.reiniciar();
        verificar(iter.hayActual() && iter.actual() == &primero,
                  "IteradorAutores reinicia desde primero");
    }
    verificar(primero.getNombre() == "Cambio" && segundo.getNombre() == "Segundo",
              "IteradorAutores no elimina autores prestados");
    Autor* dinamico = new Autor(30, "Dinamico", "A");
    {
        IteradorAutores iter;
        iter.insertar(dinamico);
    }
    verificar(dinamico->getNombre() == "Dinamico", "IteradorAutores no elimina autor dinamico");
    delete dinamico;
}

int main() {
    probarFecha();
    probarAutor();
    probarLibro();
    probarNovela();
    probarTexto();
    probarEscolar();
    probarIteradorLibros();
    probarIteradorAutores();
    probarHashLibros();
    probarABBAutores();
    probarColeccionLibros();
    probarColeccionAutores();
    if (fallos != 0) {
        std::cerr << fallos << " comprobacion(es) fallaron\n";
        return 1;
    }
    std::cout << "Todas las pruebas de entidades, colecciones e iteradores: OK\n";
    return 0;
}
