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

## 2. Elección del diseño y de los TAD

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

En el diagrama, las colecciones tienen los atributos `tabla: Hash` y `tabla: ABB`.
Las estructuras se implementan en HashLibros y ABBAutores, separadas de las
colecciones. Cada una encapsula su struct Nodo: el hash contiene un arreglo fijo de 500
punteros a listas; el ABB contiene una raíz con hijos izquierdo y derecho.
El iterador guarda referencias Objeto* en sus propios nodos de LPPF.

Las estructuras son propietarias de libros y autores y los eliminan una sola vez
al finalizar. Novela no elimina al autor compartido y el iterador solo elimina
sus nodos auxiliares. Se destruyen primero libros y después autores.

## 4. Especificación y desglose de requerimientos

Los requerimientos se toman de la letra. Cada ficha presenta entrada, salida y
proceso. La Fachada coordina los chequeos y devuelve los resultados; la capa de
presentación muestra los mensajes y listados. Los recorridos no duplican objetos.
Las fichas describen los requerimientos con entradas válidas según su dominio.
Antes de ejecutarlos, el menú valida el formato y los rangos de entrada, incluida
la validez de las fechas. La Fachada también rechaza altas con datos inválidos.
El desglose conserva los chequeos de existencia, duplicados y resultados vacíos
pedidos por la letra.

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
| **Descripción** | Dados el ISBN de una novela y la cédula de un autor, asociar dicho autor a esa novela. Si alguno no existe, informar un error. Si ya tenía un autor asociado, reemplazarlo. |
| **Entrada** | ISBN de la novela y cédula del autor. |
| **Salida** | Autor asociado a la novela, o error por ISBN o cédula inexistentes. |
| **Proceso** | Consultar si el ISBN pertenece al diccionario de libros.<br><strong>Si no existe entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Informar error: no existe una novela con ese ISBN.<br><strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Consultar si la cédula pertenece al diccionario de autores.<br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Si no existe entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Informar error: no existe un autor con esa cédula.<br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Obtener la novela del diccionario de libros.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Obtener el autor del diccionario de autores.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Asignar el autor obtenido a la novela.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Devolver confirmación de la asociación.<br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Fin</strong><br><strong>Fin</strong> |

### Requerimiento 4

| Campo | Especificación |
| --- | --- |
| **Descripción** | Dada una frase, obtener un listado básico de los libros cuyo título comienza exactamente con esa frase. Mostrar ISBN, título y tipo, sin un orden particular. Si no hay coincidencias, informar un error. |
| **Entrada** | Frase con la que debe comenzar el título. |
| **Salida** | Listado de ISBN, título y tipo de cada libro coincidente, con los datos de un mismo libro en una línea; o error si no hay resultados. |
| **Proceso** | Obtener del diccionario los libros cuyos títulos comienzan exactamente con la frase.<br><strong>Si no hay libros coincidentes entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Informar error: ningún título comienza con esa frase.<br><strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Devolver el listado básico con ISBN, título y tipo de cada libro.<br><strong>Fin</strong> |

### Requerimiento 5

| Campo | Especificación |
| --- | --- |
| **Descripción** | Dado un ISBN, listar todos los datos del libro correspondiente, incluido su tipo. Si es una novela con autor asociado, listar también los datos de ese autor. Si el ISBN no existe, informar un error. |
| **Entrada** | ISBN del libro. |
| **Salida** | Datos completos del libro y de su autor cuando corresponda, o error por ISBN inexistente. |
| **Proceso** | Consultar si el ISBN pertenece al diccionario de libros.<br><strong>Si no existe entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Informar error: no existe un libro con ese ISBN.<br><strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Obtener el libro del diccionario.<br>&nbsp;&nbsp;&nbsp;&nbsp;Preparar ISBN, título, editorial, precio base, unidades vendidas y tipo.<br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Si es una novela entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Agregar el género.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>Si tiene autor asociado entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Obtener su autor y agregar cédula, nombre y apellido.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>Fin</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Agregar materia y fecha de publicación del texto.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>Si es escolar entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Agregar el año de escuela.<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<strong>Fin</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Fin</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Devolver los datos para que la presentación los muestre.<br><strong>Fin</strong> |

### Requerimiento 6

| Campo | Especificación |
| --- | --- |
| **Descripción** | Listar todos los datos de los autores registrados, ordenados por cédula de menor a mayor. Si no hay autores registrados, informar un error. |
| **Entrada** | No requiere datos de entrada. |
| **Salida** | Listado de cédula, nombre y apellido de todos los autores en orden ascendente, con un autor por línea; o error si la colección está vacía. |
| **Proceso** | Consultar si el diccionario de autores está vacío.<br><strong>Si está vacío entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Informar error: no hay autores registrados.<br><strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Obtener los autores ordenados por cédula de menor a mayor.<br>&nbsp;&nbsp;&nbsp;&nbsp;Devolver el listado con cédula, nombre y apellido de cada autor.<br><strong>Fin</strong> |

### Requerimiento 7

| Campo | Especificación |
| --- | --- |
| **Descripción** | Dado el ISBN de un libro, registrar la venta de una nueva unidad incrementando sus unidades vendidas en uno. Si no existe, cancelar e informar un error. En otro caso, informar el precio de venta de la unidad. |
| **Entrada** | ISBN del libro vendido. |
| **Salida** | Venta registrada e importe de la unidad, o error por ISBN inexistente. |
| **Proceso** | Consultar si el ISBN pertenece al diccionario de libros.<br><strong>Si no existe entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Informar error: no existe un libro con ese ISBN.<br><strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Obtener el libro del diccionario.<br>&nbsp;&nbsp;&nbsp;&nbsp;Obtener el precio de venta invocando el método polimórfico del libro.<br>&nbsp;&nbsp;&nbsp;&nbsp;Incrementar sus unidades vendidas exactamente en uno.<br>&nbsp;&nbsp;&nbsp;&nbsp;Devolver el importe para que la presentación lo muestre.<br><strong>Fin</strong> |

### Requerimiento 8

| Campo | Especificación |
| --- | --- |
| **Descripción** | Dado el nombre de una editorial, calcular el monto total recaudado por las unidades vendidas de libros de dicha editorial. |
| **Entrada** | Nombre de la editorial. |
| **Salida** | Monto total recaudado para la editorial; cero si no hay ventas de libros coincidentes. |
| **Proceso** | Obtener los libros de la editorial recibida.<br>Calcular el total recaudado sumando, para cada libro, sus unidades vendidas multiplicadas por su precio de venta.<br>Devolver el total; si no hay ventas coincidentes, devolver cero. |

### Requerimiento 9

| Campo | Especificación |
| --- | --- |
| **Descripción** | Dado un precio base, obtener la cantidad de unidades vendidas de los libros que superan dicho precio base, discriminada entre novelas, textos comunes y textos escolares. |
| **Entrada** | Precio base de referencia. |
| **Salida** | Tres cantidades de unidades vendidas: novelas, textos comunes y escolares. Los acumuladores sin ventas coincidentes quedan en cero. |
| **Proceso** | Obtener los libros cuyo precio base supera estrictamente el precio recibido.<br>Sumar sus unidades vendidas, discriminando novelas, textos comunes y escolares.<br>Devolver las tres cantidades; si no hay ventas coincidentes, devolver cero para el tipo correspondiente. |

### Requerimiento 10

| Campo | Especificación |
| --- | --- |
| **Descripción** | Dada una editorial, listar todos los datos de uno de sus libros con la mayor cantidad de unidades vendidas. Cualquier empate es válido. Si es una novela con autor asociado, incluir sus datos. Si no hay libros de esa editorial, informar un error. |
| **Entrada** | Nombre de la editorial. |
| **Salida** | Datos completos de un libro con ventas máximas de esa editorial y de su autor cuando corresponda, o error si no existen libros de esa editorial. |
| **Proceso** | Obtener del diccionario un libro de la editorial recibida con la mayor cantidad de unidades vendidas.<br><strong>Si no hay libros de esa editorial entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Informar error: no hay libros registrados de esa editorial.<br><strong>Sino</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Obtener los datos completos del libro, incluido su tipo.<br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Si es una novela con autor asociado entonces</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Agregar los datos de su autor.<br>&nbsp;&nbsp;&nbsp;&nbsp;<strong>Fin</strong><br>&nbsp;&nbsp;&nbsp;&nbsp;Devolver los datos obtenidos.<br><strong>Fin</strong> |

### Reglas de precio utilizadas en los requerimientos 7 y 8

| Tipo de libro | Precio de venta por unidad |
| --- | --- |
| Novela | Precio base, salvo género infantil: 80% del precio base. |
| Texto común | Publicado antes del 01/01/2026: 75% del precio base. En otro caso, precio base. |
| Escolar | Se calcula como Texto; de primero a tercero se aplica además 10% de descuento sobre ese resultado. |

### Contrato de la Fachada y controles

Se sigue el Práctico 5, ejercicio 5: un enumerado `TipoError` y un parámetro
`TipoError&` de salida, en lugar de varios booleanos. Cada operación que recibe `TipoError&` establece
`SIN_ERROR` al comenzar y devuelve el primer error detectado. La presentación
convierte el resultado en un mensaje con `mensajeError(TipoError)`, definido en
`MensajesError.h/.cpp`; la lógica no usa `cin` ni `cout`. La Fachada incluye
`TipoError.h`, y el menú incluye `MensajesError.h`. Ambos módulos están
conectados con la Fachada y el menú implementados.
`TipoError` es un `enum`, no una clase ni un `struct`: define los códigos posibles.
La función de mensajes es independiente del enumerado y pertenece a presentación.
No imprime: devuelve un `string` para que el menú lo muestre.

Valores definidos en `TipoError.h`: `SIN_ERROR`, `AUTOR_YA_EXISTE`, `LIBRO_YA_EXISTE`,
`LIBRO_NO_EXISTE`, `AUTOR_NO_EXISTE`, `LIBRO_NO_ES_NOVELA`,
`SIN_COINCIDENCIAS`, `SIN_AUTORES`, `SIN_LIBROS_DE_EDITORIAL`,
`DATOS_INVALIDOS` y `LIMITE_VENTAS`.

| Requerimiento / método | Controles y resultados |
| --- | --- |
| 1 · registrarAutor | Si la cédula no es positiva o falta nombre o apellido: DATOS_INVALIDOS. Si la cédula existe: AUTOR_YA_EXISTE. En otro caso, crear e insertar el autor. |
| 2 · registrarLibro | Si el puntero es NULL, los datos son inválidos o ya tiene ventas: DATOS_INVALIDOS. Si el ISBN existe: LIBRO_YA_EXISTE. En otro caso, insertar el libro recibido, inicialmente con cero ventas. |
| 3 · asociarAutor | Comprobar ISBN, que sea una novela y cédula, en ese orden. Devolver LIBRO_NO_EXISTE, LIBRO_NO_ES_NOVELA o AUTOR_NO_EXISTE según corresponda. Solo asociar si todos los controles pasan. |
| 4 · listarPorPrefijo | Cargar las coincidencias. Si no hay: SIN_COINCIDENCIAS. |
| 5 · consultarLibro | Si no existe el ISBN: LIBRO_NO_EXISTE. En otro caso, devolver una referencia prestada al libro. |
| 6 · listarAutores | Si no hay autores: SIN_AUTORES. En otro caso, cargar el recorrido ascendente por cédula. |
| 7 · registrarVenta | Si no existe el ISBN: LIBRO_NO_EXISTE. Si alcanzó el máximo representable por int: LIMITE_VENTAS. En otro caso, calcular el importe e incrementar las ventas. |
| 8 · calcularRecaudacion | Devolver el total; cero también es un resultado válido. No necesita TipoError. |
| 9 · contarVentasPorTipo | Devolver los tres acumuladores por referencia; cero es válido. No necesita TipoError. |
| 10 · consultarMasVendido | Si no hay libros de la editorial: SIN_LIBROS_DE_EDITORIAL. En otro caso, devolver una referencia prestada a uno de los máximos. |

La presentación valida que las entradas puedan leerse y pertenezcan al dominio:
por ejemplo, una opción de menú permitida y una fecha que cumpla `esValida()`.
La Fachada comprueba existencia, duplicados, tipo de libro y resultados vacíos
antes de llamar a las primitivas con precondiciones. El control de tipo del
requerimiento 3 evita tratar un Texto o Escolar como Novela.

`registrarLibro` comprueba que el puntero no sea nulo, que `datosValidos()` sea
verdadero y que tenga cero ventas antes de acceder a la colección. Si el registro tiene éxito, la colección pasa
a ser su propietaria; si falla, el llamador conserva la propiedad y debe eliminarlo
o reutilizarlo. La Fachada comprueba cédula positiva y nombre y apellido no vacíos en
`registrarAutor`; crea el objeto únicamente si la cédula no está registrada.

Los iteradores de salida se reciben vacíos y se usan mientras exista la colección.
En las consultas que devuelven `Libro*&`, se establece el puntero en `NULL` al
comenzar; solo contiene un libro si el resultado es `SIN_ERROR`. El importe de
`registrarVenta` se inicializa en cero y solo representa una venta si no hubo error.
Los punteros de consulta son prestados: el menú no elimina esos libros ni sus autores.

### Menú y separación en capas

`Menu.h/.cpp` implementa la presentación; `main.cpp` llama a `ejecutarMenu()`: un ciclo `while` muestra opciones del 1 al 10,
correspondientes a los requerimientos en el mismo orden, y 0 para salir. Un `switch`
selecciona la operación y llama a la Fachada; el menú no accede directamente a las
colecciones. El fin de la entrada también termina el programa.

Antes de llamar a la lógica, se comprueban números completos y dentro de rango,
textos no vacíos, precios finitos no negativos, fechas reales entre 1900 y 2099 y
años escolares entre 1 y 6. Una entrada inválida se vuelve a pedir. Las líneas se leen con `cin.get()` hasta
el salto de línea, siguiendo el recorrido de `scan()` del Práctico 5; después
se comprueban los dígitos del número. Los precios aceptan notación decimal con punto.
Las fechas se
comprueban con `esValida()` antes de crear el libro. Los textos conservan espacios,
mayúsculas y minúsculas para respetar la búsqueda exacta por comienzo del título.

El menú usa `using namespace std`, `cout`, `cin.get()`, ciclos, condiciones y
`switch`. No usa `fixed`, `setprecision`, `getline` ni `istringstream`.
Los auxiliares de validación son propios del proyecto: el práctico muestra la
lectura carácter por carácter, pero no un menú completo con todos estos controles.
`cin.eof()` detecta el fin de entrada; `LONG_MAX` y `DBL_MAX` son los límites de los
tipos numéricos, usados para evitar valores que no puedan representarse.

La Fachada garantiza los controles de negocio antes de llamar a `buscar`,
`insertar`, `asignarAutor`, `calcularPrecioVenta` y `registrarVenta`. El menú traduce
TipoError con `mensajeError` y muestra los datos; la Fachada no imprime. Los iteradores
son variables locales, se pasan por referencia y no eliminan los objetos listados.
Al salir, se destruye la Fachada, primero sus libros y luego sus autores.

## 5. Diagrama de implementación

Un único diagrama reúne las entidades completas, colecciones, tipos auxiliares,
Fachada y el iterador común del diseño implementado. El rombo blanco de Novela a
Autor expresa agregación sin transferir la propiedad del autor a la novela.
El iterador se muestra suelto con `<<iterator>>`; los diccionarios se identifican
con `<<diccionario>>`. La lectura va de izquierda a derecha: Fachada,
colecciones y entidades. El iterador permanece independiente.

```mermaid
classDiagram
    direction LR

    Fachada "1" o-- "1" ColeccionLibros
    Fachada "1" o-- "1" ColeccionAutores
    ColeccionLibros "1" o-- "0..*" Libro
    ColeccionAutores "1" o-- "0..*" Autor
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
        -isbn: long int
        -titulo: string
        -editorial: string
        -precioBase: double
        -unidadesVendidas: int
        +Libro(long int, string, string, double)
        +~Libro()
        +getId() long int
        +getTitulo() string
        +getEditorial() string
        +getPrecioBase() double
        +getUnidadesVendidas() int
        +registrarVenta() void
        +comienzaCon(string) bool
        +datosValidos() bool
        +calcularPrecioVenta()* double
        +getTipo()* string
    }
    class Novela {
        -tipo: const string
        -genero: string
        -autor: Autor#42;
        +Novela(long int, string, string, double, string)
        +~Novela()
        +getGenero() string
        +getAutor() Autor #42;
        +asignarAutor(Autor #42;) void
        +datosValidos() bool
        +calcularPrecioVenta() double
        +getTipo() string
    }
    class Texto {
        -tipo: const string
        -materia: string
        -fechaPublicado: Fecha
        +Texto(long int, string, string, double, string, Fecha)
        +~Texto()
        +getMateria() string
        +getFechaPublicado() Fecha
        +datosValidos() bool
        +calcularPrecioVenta() double
        +getTipo() string
    }
    class Escolar {
        -tipo: const string
        -anioEscuela: int
        +Escolar(long int, string, string, double, string, Fecha, int)
        +~Escolar()
        +getAnioEscuela() int
        +datosValidos() bool
        +calcularPrecioVenta() double
        +getTipo() string
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
    }
    class ColeccionLibros {
        <<diccionario>>
        -tabla: Hash
        +ColeccionLibros()
        +~ColeccionLibros()
        +estaVacia() bool
        +existe(long int) bool
        +buscar(long int) Libro#42;
        +insertar(Libro#42;) void
        +cargarIterador(Iterador&) void
    }
    class ColeccionAutores {
        <<diccionario>>
        -tabla: ABB
        +ColeccionAutores()
        +~ColeccionAutores()
        +estaVacia() bool
        +existe(long int) bool
        +buscar(long int) Autor#42;
        +insertar(Autor#42;) void
        +cargarIterador(Iterador&) void
    }
    class Iterador {
        <<iterator>>
        -lista: LPPF
        +Iterador()
        +~Iterador()
        +insertar(Objeto#42;) void
        +reiniciar() void
        +hayActual() bool
        +actual() Objeto#42;
        +avanzar() void
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
        LIMITE_VENTAS
    }
    class Fachada {
        <<fachada>>
        -autores: ColeccionAutores
        -libros: ColeccionLibros
        +Fachada()
        +~Fachada()
        +registrarAutor(long int, string, string, TipoError&) void
        +registrarLibro(Libro#42;, TipoError&) void
        +asociarAutor(long int, long int, TipoError&) void
        +listarPorPrefijo(string, Iterador&, TipoError&) void
        +consultarLibro(long int, Libro#42;&, TipoError&) void
        +listarAutores(Iterador&, TipoError&) void
        +registrarVenta(long int, double&, TipoError&) void
        +calcularRecaudacion(string) double
        +contarVentasPorTipo(double, long int&, long int&, long int&) void
        +consultarMasVendido(string, Libro#42;&, TipoError&) void
    }
```

Cada función que devuelve un valor usa un único `return` al final, con el valor
entre paréntesis. Las funciones `void`, constructores y destructores no lo necesitan.

Los atributos privados llevan `-` y los métodos públicos llevan `+`. Los parámetros
se indican solo con sus tipos. Los métodos abstractos de Libro se muestran en
cursiva. `*` significa puntero; en el fuente Mermaid se escribe `#42;` para evitar
que se interprete como marca de método abstracto. Los retornos y referencias se
leen con la notación habitual de C++.

Los textos usan `string` estándar de C++. Sus miembros se destruyen
automáticamente; los destructores vacíos no llaman manualmente a sus destructores.
Objeto tiene destructor virtual; Libro y Autor lo conservan por herencia.
Novela, Texto y Escolar inicializan su constante privada `tipo` en el constructor;
`getTipo()` devuelve esa constante. El precio se calcula mediante polimorfismo,
sin comparar esos textos.

Objeto declara `getId()` abstracto. Libro lo implementa devolviendo ISBN y Autor
devolviendo cédula, sin agregar otro atributo identificador. El iterador almacena
Objeto* y no posee los objetos: libera únicamente los nodos de su LPPF.
Al recuperarlos, el llamador conoce si el recorrido fue cargado con libros o autores.

**Estado del diseño:** las entidades, las estructuras separadas, Objeto, el
iterador común, TipoError, la función de mensajes, la Fachada y el menú están
implementados. Hay pruebas de las clases y de los diez requerimientos mediante
la Fachada, además de pruebas del menú ejecutando el programa con entradas de consola.
Las pruebas del menú están en el mismo programa de pruebas C++ y verifican
sus entradas y salidas; también se incluyen en la medición de cobertura.
