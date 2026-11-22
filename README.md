# Obligatorio P4 · Librería

## 1. Diagrama de análisis

Diagrama conceptual provisto por la letra, expresado en Mermaid.

```mermaid
classDiagram
    direction TB
    class Libro {
        <<abstract>>
        isbn
        titulo
        editorial
        precioBase
        unidadesVendidas
    }
    class Novela {
        genero
    }
    class Texto {
        materia
        fechaPublicado
    }
    class Escolar {
        anioEscuela
    }
    class Autor {
        cedula
        nombre
        apellido
    }
    Libro <|-- Novela
    Libro <|-- Texto
    Texto <|-- Escolar
    Novela "0..*" --> "0..1" Autor : escrita por
```

## 2. Elección de los TAD

```text
Libros = Diccionario (Libro)
Libro = isbn x titulo x editorial x precioBase x unidadesVendidas
Autores = Diccionario (Autor)
Autor = cedula x nombre x apellido
Novela = Libro + (genero x Autor)
Texto = (materia x fechaPublicado) + Libro
Escolar = anioEscuela + Texto
```

`x` combina atributos. `+ Libro` y `+ Texto` representan la herencia de la clase
base. El Autor de una Novela es una referencia opcional a un autor registrado:
puede no estar asociado, y un mismo autor puede estar referenciado por varias novelas.

| Elección | Justificación |
| --- | --- |
| Libros como Diccionario (Libro) | Cada libro se identifica por un ISBN único. Se necesita comprobar pertenencia, insertar sin duplicados y recuperar por esa clave. No se pide conservar el orden de registro. |
| Autores como Diccionario (Autor) | Cada autor se identifica por una cédula única. Se necesita comprobar pertenencia, registrar y buscar por cédula. La cantidad de autores no tiene cota y pueden existir autores sin novelas asociadas. |
| Novela como especialización de Libro | Comparte los atributos comunes del libro y agrega género y referencia opcional a Autor. Define su cálculo particular del precio. |
| Texto como especialización de Libro | Comparte los atributos comunes y agrega materia y fecha de publicación, necesaria para su descuento. |
| Escolar como especialización de Texto | Conserva materia y fecha, agrega año de escuela y amplía la regla de precio del texto común. |
| Recorrido como Secuencia (Objeto) | Los resultados se recorren en el orden en que se cargan. La base Objeto permite guardar referencias a libros o autores en un mismo tipo de iterador, sin duplicar las entidades. |

Libro y Autor heredan de Objeto para reutilizar el recorrido. Eso no significa
que Autor sea un Libro. Libros y Autores se representan en código con las clases
ColeccionLibros y ColeccionAutores.

## 3. Elección de estructuras para los TAD

| TAD | Estructura elegida | Justificación |
| --- | --- | --- |
| Diccionario de libros | Hash con listas simplemente enlazadas por cubeta | Hay aproximadamente 500 títulos y se consulta frecuentemente por ISBN. Con buena dispersión y carga controlada, buscar e insertar tiene costo esperado O(1); el peor caso es O(n). Las listas resuelven colisiones sin fijar un máximo de libros. |
| Diccionario de autores | ABB por cédula | Permite buscar por cédula y obtener el listado ascendente mediante recorrido en orden. Buscar e insertar cuesta O(h); puede llegar a O(n) si el árbol se degenera. El listado completo cuesta O(n). |
| Secuencia de referencias del iterador | LPPF | Agregar al final cuesta O(1) y conserva el orden de carga, especialmente el orden ascendente de los autores. Cada avance cuesta O(1), sin necesitar una capacidad máxima. |

El hash actual tiene 500 cubetas y dispersa con ISBN % 500. Los 500 títulos son
una estimación, no una capacidad máxima. Recorrer todas las cubetas cuesta
O(n + cantidad de cubetas). Las listas de colisiones insertan al principio;
no necesitan un enlace hacia atrás, un puntero al fondo ni circularidad.

AVL sería una alternativa para garantizar búsquedas O(log n), pero la letra no
impone ese costo. Se elige ABB por su implementación más simple. No se agregan
índices por editorial, precio ni prefijo: esas consultas recorren el diccionario
de libros. Una lista circular o doblemente enlazada no aporta operaciones que
necesiten estos recorridos finitos.

En el diagrama, las colecciones tienen los atributos `tabla: HashLibros` y `tabla: ABBAutores`.
Las estructuras se implementan en HashLibros y ABBAutores, separadas de las
colecciones. Cada una encapsula su struct Nodo: el hash contiene un arreglo fijo de 500
punteros a listas; el ABB contiene una raíz con hijos izquierdo y derecho.
El iterador guarda referencias Objeto* en sus propios nodos de LPPF.

Las estructuras son propietarias de libros y autores y los eliminan una sola vez
al finalizar. Novela no elimina al autor compartido y el iterador solo elimina
sus nodos auxiliares. Se destruyen primero libros y después autores.

## 4. Especificación y desglose de requerimientos

Cada requerimiento se describe mediante su entrada, salida y proceso. El menú
valida los datos y solicita las operaciones a la Fachada. La Fachada coordina los
listados y cada entidad imprime sus datos; la colección imprime los totales. Los
mensajes de resultado se muestran desde el módulo MensajesError.

### Requerimiento 1

| Campo | Especificación |
| --- | --- |
| **Descripción** | Registrar un nuevo autor, comprobando que no exista otro con la misma cédula. Si existe, cancelar el registro e informar un error. |
| **Entrada** | Cédula, nombre y apellido del nuevo autor. |
| **Salida** | Autor registrado, o error por cédula repetida. |
| **Proceso** | Consultar si la cédula pertenece al diccionario de autores.<br><strong>Si ya existe entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Informar error: ya existe un autor con esa cédula.<br><strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Crear el autor con los datos recibidos.<br>&nbsp;&nbsp;&nbsp;&nbsp;Insertarlo en el diccionario de autores.<br>&nbsp;&nbsp;&nbsp;&nbsp;Devolver confirmación de registro.<br><strong>Fin</strong> |

### Requerimiento 2

| Campo | Especificación |
| --- | --- |
| **Descripción** | Registrar un nuevo libro, comprobando que no exista otro con el mismo ISBN. Si existe, cancelar el registro e informar un error. Si se registra, inicializar sus unidades vendidas en cero. |
| **Entrada** | ISBN, título, editorial, precio base y tipo de libro. Para novela: género. Para texto: materia y fecha de publicación. Para escolar: además, año de escuela. |
| **Salida** | Libro registrado con cero unidades vendidas, o error por ISBN repetido. |
| **Proceso** | Consultar si el ISBN pertenece al diccionario de libros.<br><strong>Si ya existe entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Informar error: ya existe un libro con ese ISBN.<br><strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Crear una Novela, un Texto o un Escolar según el tipo recibido.<br>&nbsp;&nbsp;&nbsp;&nbsp;Inicializar las unidades vendidas en cero.<br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Si es una novela entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Inicializar su referencia al autor sin asociación.<br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Fin</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Insertar el libro en el diccionario de libros.<br>&nbsp;&nbsp;&nbsp;&nbsp;Devolver confirmación de registro.<br><strong>Fin</strong> |

### Requerimiento 3

| Campo | Especificación |
| --- | --- |
| **Descripción** | Dados el ISBN de una novela y la cédula de un autor, asociar dicho autor a esa novela. Si ya tenía autor, reemplazar la referencia sin eliminar al anterior. |
| **Entrada** | ISBN y cédula con formato válido, comprobado en el menú. |
| **Salida** | Autor asociado, o LIBRO_NO_EXISTE, LIBRO_NO_ES_NOVELA o AUTOR_NO_EXISTE. |
| **Proceso** | Comprobar si el ISBN está registrado.<br>Si no está, informar LIBRO_NO_EXISTE.<br>En otro caso, obtener el libro y comprobar que sea una novela.<br>Si no lo es, informar LIBRO_NO_ES_NOVELA.<br>En otro caso, comprobar la cédula.<br>Si no está registrada, informar AUTOR_NO_EXISTE.<br>En otro caso, obtener el autor y asignar su puntero a la novela.<br>Devolver confirmación sin eliminar ni copiar ningún autor. |

### Requerimiento 4

| Campo | Especificación |
| --- | --- |
| **Descripción** | Dada una frase, obtener un listado básico de los libros cuyo título comienza exactamente con esa frase. Mostrar ISBN, título y tipo, sin un orden particular. Si no hay coincidencias, informar un error. |
| **Entrada** | Frase con la que debe comenzar el título. |
| **Salida** | Listado de ISBN, título y tipo de cada libro coincidente, con los datos de un mismo libro en una línea; o error si no hay resultados. |
| **Proceso** | Obtener del diccionario los libros cuyos títulos comienzan exactamente con la frase.<br><strong>Si no hay libros coincidentes entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Informar error: ningún título comienza con esa frase.<br><strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Listar ISBN, título y tipo de cada libro.<br><strong>Fin</strong> |

### Requerimiento 5

| Campo | Especificación |
| --- | --- |
| **Descripción** | Dado un ISBN, listar todos los datos del libro correspondiente, incluido su tipo. Si es una novela con autor asociado, listar también los datos de ese autor. Si el ISBN no existe, informar un error. |
| **Entrada** | ISBN del libro. |
| **Salida** | Datos completos del libro y de su autor cuando corresponda, o error por ISBN inexistente. |
| **Proceso** | Consultar si el ISBN pertenece al diccionario de libros.<br><strong>Si no existe entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Informar error: no existe un libro con ese ISBN.<br><strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Obtener el libro del diccionario.<br>&nbsp;&nbsp;&nbsp;&nbsp;Preparar ISBN, título, editorial, precio base, unidades vendidas y tipo.<br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Si es una novela entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Agregar el género.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>Si tiene autor asociado entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Obtener su autor y agregar cédula, nombre y apellido.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>Fin</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Agregar materia y fecha de publicación del texto.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>Si es escolar entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Agregar el año de escuela.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>Fin</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Fin</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Listar los datos completos del libro.<br><strong>Fin</strong> |

### Requerimiento 6

| Campo | Especificación |
| --- | --- |
| **Descripción** | Listar todos los datos de los autores registrados, ordenados por cédula de menor a mayor. Si no hay autores registrados, informar un error. |
| **Entrada** | No requiere datos de entrada. |
| **Salida** | Listado de cédula, nombre y apellido de todos los autores en orden ascendente, con un autor por línea; o error si la colección está vacía. |
| **Proceso** | Consultar si el diccionario de autores está vacío.<br><strong>Si está vacío entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Informar error: no hay autores registrados.<br><strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Obtener los autores ordenados por cédula de menor a mayor.<br>&nbsp;&nbsp;&nbsp;&nbsp;Listar cédula, nombre y apellido de cada autor.<br><strong>Fin</strong> |

### Requerimiento 7

| Campo | Especificación |
| --- | --- |
| **Descripción** | Registrar la venta de una unidad del libro e informar su precio de venta. |
| **Entrada** | ISBN del libro vendido. |
| **Salida** | Venta registrada e importe de la unidad, o error por ISBN inexistente. |
| **Proceso** | Consultar si el ISBN pertenece al diccionario de libros.<br><strong>Si no existe entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Informar error: no existe un libro con ese ISBN.<br><strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Obtener el libro del diccionario.<br>&nbsp;&nbsp;&nbsp;&nbsp;Calcular su precio de venta según el tipo de libro y los descuentos que correspondan.<br>&nbsp;&nbsp;&nbsp;&nbsp;Sumar una unidad a la cantidad de unidades vendidas.<br>&nbsp;&nbsp;&nbsp;&nbsp;Informar el importe de la venta.<br><strong>Fin</strong> |

### Requerimiento 8

| Campo | Especificación |
| --- | --- |
| **Descripción** | Dado el nombre de una editorial, calcular el monto total recaudado por las unidades vendidas de libros de dicha editorial. |
| **Entrada** | Nombre de la editorial. |
| **Salida** | Monto total recaudado para la editorial; cero si no hay ventas de libros coincidentes. |
| **Proceso** | Obtener los libros de la editorial recibida.<br>Calcular el total recaudado sumando, para cada libro, sus unidades vendidas multiplicadas por su precio de venta.<br>Informar el total; si no hay ventas coincidentes, informar cero. |

### Requerimiento 9

| Campo | Especificación |
| --- | --- |
| **Descripción** | Dado un precio base, obtener la cantidad de unidades vendidas de los libros que superan dicho precio base, discriminada entre novelas, textos comunes y textos escolares. |
| **Entrada** | Precio base de referencia. |
| **Salida** | Tres cantidades de unidades vendidas: novelas, textos comunes y escolares. Los acumuladores sin ventas coincidentes quedan en cero. |
| **Proceso** | Obtener los libros cuyo precio base supera estrictamente el precio recibido.<br>Sumar sus unidades vendidas, discriminando novelas, textos comunes y escolares.<br>Informar las tres cantidades; si no hay ventas coincidentes, informar cero para el tipo correspondiente. |

### Requerimiento 10

| Campo | Especificación |
| --- | --- |
| **Descripción** | Dada una editorial, listar todos los datos de uno de sus libros con la mayor cantidad de unidades vendidas. Cualquier empate es válido. Si es una novela con autor asociado, incluir sus datos. Si no hay libros de esa editorial, informar un error. |
| **Entrada** | Nombre de la editorial. |
| **Salida** | Datos completos de un libro con ventas máximas de esa editorial y de su autor cuando corresponda, o error si no existen libros de esa editorial. |
| **Proceso** | Obtener del diccionario un libro de la editorial recibida con la mayor cantidad de unidades vendidas.<br><strong>Si no hay libros de esa editorial entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Informar error: no hay libros registrados de esa editorial.<br><strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Obtener los datos completos del libro, incluido su tipo.<br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Si es una novela con autor asociado entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Agregar los datos de su autor.<br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Fin</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Listar los datos obtenidos.<br><strong>Fin</strong> |

### Reglas de precio utilizadas en los requerimientos 7 y 8

| Tipo de libro | Precio de venta por unidad |
| --- | --- |
| Novela | Precio base, salvo género infantil: 80% del precio base. |
| Texto común | Publicado antes del 01/01/2026: 75% del precio base. En otro caso, precio base. |
| Escolar | Se calcula como Texto; de primero a tercero se aplica además 10% de descuento sobre ese resultado. |

## 5. Diagrama de implementación

```mermaid
classDiagram
    direction LR

    Fachada "1" *-- "1" ColeccionLibros
    Fachada "1" *-- "1" ColeccionAutores
    ColeccionLibros "1" *-- "1" HashLibros
    ColeccionAutores "1" *-- "1" ABBAutores
    HashLibros "1" *-- "0..*" Libro
    ABBAutores "1" *-- "0..*" Autor
    Objeto <|-- Autor
    Objeto <|-- Libro
    Libro <|-- Novela
    Libro <|-- Texto
    Texto <|-- Escolar
    Novela "0..*" o-- "0..1" Autor
    Texto "1" *-- "1" Fecha

    class Objeto {
        <<abstract>>
        +~Objeto()
        +getId()* long int
    }
    class Libro {
        <<abstract>>
        -imprimirTipo() void
        -isbn: long int
        -titulo: string
        -editorial: string
        -precioBase: float
        -unidadesVendidas: int
        +Libro(long int, string, string, float)
        +~Libro()
        +getId() long int
        +getTitulo() string
        +getEditorial() string
        +getPrecioBase() float
        +getUnidadesVendidas() int
        +registrarVenta() void
        +comienzaCon(string) bool
        +datosValidos() bool
        +calcularPrecioVenta()* float
        +getTipo()* TipoLibro
        +imprimir() void
        +imprimirBasico() void
        +imprimirPrecioVenta() void
    }
    class Novela {
        -genero: string
        -autor: Autor#42;
        +Novela(long int, string, string, float, string)
        +~Novela()
        +getGenero() string
        +getAutor() Autor #42;
        +asignarAutor(Autor #42;) void
        +datosValidos() bool
        +calcularPrecioVenta() float
        +getTipo() TipoLibro
        +imprimir() void
    }
    class Texto {
        -materia: string
        -fechaPublicado: Fecha
        +Texto(long int, string, string, float, string, Fecha)
        +~Texto()
        +getMateria() string
        +getFechaPublicado() Fecha
        +datosValidos() bool
        +calcularPrecioVenta() float
        +getTipo() TipoLibro
        +imprimir() void
    }
    class Escolar {
        -anioEscuela: int
        +Escolar(long int, string, string, float, string, Fecha, int)
        +~Escolar()
        +getAnioEscuela() int
        +datosValidos() bool
        +calcularPrecioVenta() float
        +getTipo() TipoLibro
        +imprimir() void
    }
    class Autor {
        -cedula: long int
        -nombre: string
        -apellido: string
        +Autor(long int, string, string)
        +~Autor()
        +getId() long int
        +getNombre() string
        +getApellido() string
        +imprimir() void
    }
    class Fecha {
        -dia: int
        -mes: int
        -anio: int
        +Fecha(int, int, int)
        +getDia() int
        +getMes() int
        +getAnio() int
        +esValida() bool
        +operator<(Fecha) bool
        +imprimir() void
    }
    class ColeccionLibros {
        <<diccionario>>
        -tabla: HashLibros
        +ColeccionLibros()
        +~ColeccionLibros()
        +estaVacia() bool
        +existe(long int) bool
        +buscar(long int) Libro#42;
        +insertar(Libro#42;) void
        +cargarIterador(Iterador&) void
        +cargarPorPrefijo(string, Iterador&) void
        +existeEditorial(string) bool
        +calcularRecaudacion(string) float
        +contarVentasPorTipo(float, long int&, long int&, long int&) void
        +masVendido(string) Libro#42;
        +imprimirRecaudacion(string) void
        +imprimirVentasPorTipo(float) void
    }
    class ColeccionAutores {
        <<diccionario>>
        -tabla: ABBAutores
        +ColeccionAutores()
        +~ColeccionAutores()
        +estaVacia() bool
        +existe(long int) bool
        +buscar(long int) Autor#42;
        +insertar(Autor#42;) void
        +cargarIterador(Iterador&) void
    }
    class HashLibros {
        -cubetas: Nodo#42;[CUBETAS_LIBROS]
        -h(long int) int
        -crearLista(Nodo#42;&) void
        -destruirLista(Nodo#42;&) void
        -perteneceLista(Nodo#42;, long int) bool
        -insFrontEnLista(Nodo#42;&, Libro#42;) void
        -obtenerEnLista(Nodo#42;, long int) Libro#42;
        +HashLibros()
        +~HashLibros()
        +estaVacia() bool
        +existe(long int) bool
        +buscar(long int) Libro#42;
        +insertar(Libro#42;) void
        +cargarIterador(Iterador&) void
    }
    class ABBAutores {
        -raiz: Nodo#42;
        -destruirArbol(Nodo#42;&) void
        -insertarEnArbol(Nodo#42;&, Autor#42;) void
        -cargarIterador(Nodo#42;, Iterador&) void
        +ABBAutores()
        +~ABBAutores()
        +estaVacia() bool
        +existe(long int) bool
        +buscar(long int) Autor#42;
        +insertar(Autor#42;) void
        +cargarIterador(Iterador&) void
    }
    class Iterador {
        <<iterator>>
        -primero: Nodo#42;
        -ultimo: Nodo#42;
        -cursor: Nodo#42;
        +Iterador()
        +~Iterador()
        +insertar(Objeto#42;) void
        +reiniciar() void
        +hayActual() bool
        +actual() Objeto#42;
        +avanzar() void
    }
    class TipoLibro {
        <<enumeration>>
        NOVELA
        TEXTO
        ESCOLAR
    }
    class TipoError {
        <<enumeration>>
        SIN_ERROR
        AUTOR_YA_EXISTE
        LIBRO_YA_EXISTE
        LIBRO_NO_EXISTE
        AUTOR_NO_EXISTE
        LIBRO_NO_ES_NOVELA
        SIN_COINCIDENCIAS
        SIN_AUTORES
        SIN_LIBROS_DE_EDITORIAL
        DATOS_INVALIDOS
    }
    class Fachada {
        <<fachada>>
        -autores: ColeccionAutores
        -libros: ColeccionLibros
        +Fachada()
        +~Fachada()
        +registrarAutor(long int, string, string, TipoError&) void
        +registrarLibro(TipoLibro, long int, string, string, float, string, string, int, int, int, int, TipoError&) void
        +asociarAutor(long int, long int, TipoError&) void
        +listarPorPrefijo(string, TipoError&) void
        +consultarLibro(long int, TipoError&) void
        +listarAutores(TipoError&) void
        +registrarVenta(long int, TipoError&) void
        +calcularRecaudacion(string) void
        +contarVentasPorTipo(float) void
        +consultarMasVendido(string, TipoError&) void
    }
```


## 6. Diagrama de módulos

Cada módulo muestra el par **.h / .cpp**. Las flechas representan dependencias a
través del encabezado: **A → B significa que B incluye A.h**. El recorrido va
de arriba hacia abajo y termina en main.cpp, que no tiene encabezado.
Los recuadros verdes son los encabezados compartidos de tipos y constantes,
que no tienen un .cpp asociado.

![Diagrama de módulos](docs/diagramas/modulos.png)

[Versión vectorial del diagrama](docs/diagramas/modulos.svg)

Para quitar un include se revisa el archivo donde está declarado: un include
que está únicamente en un .cpp no se transmite a los demás módulos.
