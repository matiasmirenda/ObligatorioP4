# Obligatorio P4 UDE

Los encabezados estan en include/ y las implementaciones en src/.
Las entidades del dominio siguen vacias. Fecha contiene dia, mes y anio
como enteros, validacion y comparaciones de menor, mayor e igual.
Antes de construir una Fecha se debe comprobar Fecha::esValida(dia, mes, anio).

Para compilar y ejecutar las pruebas desde la terminal de Visual Studio Code:

```sh
mkdir -p build
g++ -std=c++11 -Wall -Wextra -pedantic -Iinclude src/Fecha.cpp tests/main.cpp -o build/p4_tests
./build/p4_tests
```

GitHub Actions compila, ejecuta las pruebas y exige un minimo de 90% de
cobertura de lineas en src/.

## Propuesta de TAD y estructuras de datos

Esta es una propuesta de diseño para discutir antes de implementar. La letra
pide justificar las colecciones; no impone hash, ABB ni AVL.

Un TAD define las operaciones y su significado. La estructura elegida define
como se guardan los datos para resolver esas operaciones. En objetos, una clase
abstracta puede expresar el contrato y una clase concreta implementar la estructura.
No es necesario que toda clase del sistema sea abstracta.

| TAD / contrato | Implementacion propuesta | Motivo y requerimientos |
| --- | --- | --- |
| ColeccionLibros: diccionario por ISBN | HashLibros: tabla hash con encadenamiento | Hay aproximadamente 500 titulos. Registrar sin duplicados y consultar por ISBN son operaciones frecuentes (2, 3, 5 y 7). Busqueda e insercion esperadas O(1) con buena dispersion y carga controlada; peor caso O(n). |
| ColeccionAutores: diccionario por cedula | ABBAutores: arbol binario de busqueda | No hay cota para autores. Permite buscar por cedula (1 y 3) y listar ordenados mediante recorrido en orden (6), en O(n) para el listado. Buscar e insertar cuestan O(h), y pueden llegar a O(n) si el arbol se degenera. |
| IteradorLibros: recorrido de libros | IteradorHashLibros | Recorre cubetas y sus cadenas para los requerimientos 4, 8, 9 y 10, sin exponer nodos a la Fachada. |
| IteradorAutores: recorrido ordenado de autores | IteradorABBAutores | Recorre el ABB en orden para el requerimiento 6, sin exponer nodos a la Fachada. |

La cantidad aproximada de libros permite elegir una cantidad inicial de cubetas;
no significa que 500 sea un limite obligatorio. Las colisiones se resolveran con
cadenas de nodos enlazados, que tambien permiten superar esa estimacion. Los
recorridos completos del hash cuestan O(n + cantidad de cubetas).

### ABB, AVL y listas

Se propone ABB como punto inicial por su menor complejidad de implementacion.
AVL es una alternativa si se requiere garantizar O(log n) para buscar e insertar
autores, incluso con cedulas ingresadas en orden; agrega balanceos y rotaciones.
La ausencia de cota por si sola no obliga a usar AVL ni impide usar hash.

Una lista enlazada permite insercion sencilla y recorrido secuencial, pero buscar
un ISBN o una cedula cuesta O(n). Por eso se usaran cadenas enlazadas para las
colisiones del hash, sin agregar por ahora una segunda lista con todos los libros.
Una lista circular no aporta una ventaja para los recorridos finitos pedidos.
La sigla LPPF debe confirmarse con la nomenclatura del curso antes de atribuirle
una estructura y compararla en la documentacion definitiva.

No se agregan indices por editorial, precio ni titulo: con unos 500 libros alcanza
un recorrido completo para los requerimientos 4, 8, 9 y 10. Asi se evita mantener
indices adicionales. Si aumentara mucho el volumen, esta decision se revisaria.

### Contratos propuestos para las colecciones

- ColeccionLibros: estaVacia, existe(isbn), buscar(isbn), insertar(libro), crearIterador.
- ColeccionAutores: estaVacia, existe(cedula), buscar(cedula), insertar(autor), crearIterador.
- Iteradores: hayActual, actual, avanzar. El recorrido comienza en el primer elemento.
- buscar: PRECONDICION: la clave existe.
- insertar: PRECONDICION: el puntero es valido y la clave no existe.
- actual y avanzar: PRECONDICION: hayActual devuelve verdadero.

Los chequeos de existencia los realiza la logica antes de llamar operaciones con
esas precondiciones. Las interfaces abstractas tendran destructor virtual.
Cada iterador se liberara al terminar su uso y no sera propietario de los objetos
que recorre. La coleccion no se modificara mientras se use su iterador.

### Objetos, punteros y memoria

- Fecha se guarda por valor dentro de Texto: es un dato pequeno con tres enteros.
- La coleccion de libros guarda Libro* para conservar el polimorfismo de Novela,
  Texto y Escolar. Libro necesitara destructor virtual.
- La coleccion de autores guarda Autor* y es propietaria de esos objetos.
- Novela guarda Autor* como referencia al autor registrado, inicialmente nula;
  no copia al autor ni lo elimina. Reasignar autor solo cambia esa referencia.
- Cada coleccion elimina una sola vez sus objetos y sus nodos al finalizar.
  Primero se destruyen libros y despues autores. Se evitara copiar colecciones
  propietarias para no duplicar la liberacion de memoria.

### Fecha y capas

Fecha::esMenor(Fecha(1, 1, 2026)) permite aplicar el descuento de los textos
publicados antes del corte exigido. No se pide un listado por fecha en esta version.
Las comparaciones mayor e igual quedan disponibles como operaciones del tipo.

Fachada coordina las operaciones y delega en las colecciones y entidades.
La entrada por teclado y los mensajes se resuelven en la capa de presentacion;
los nodos, cubetas y recorridos internos pertenecen a las estructuras.

### Orden incremental de implementacion

1. Fecha y sus pruebas.
2. Autor, Libro y las subclases Novela, Texto y Escolar, con sus pruebas.
3. Contratos de colecciones e iteradores.
4. HashLibros e IteradorHashLibros, probando colisiones y recorridos vacios.
5. ABBAutores e IteradorABBAutores, probando orden y busqueda.
6. Fachada y los diez requerimientos, con pruebas de integracion.
7. Programa principal y configuracion de entrega para Code::Blocks.

Los archivos nuevos de estructuras e iteradores se reservan vacios. La propuesta
no constituye aun la especificacion UML completa exigida para la entrega.

## Plan por fases para tres integrantes (sin fechas)

Se identifican como A, B y C hasta asignar nombres. Cada entrega incluye sus
pruebas y se integra mediante PR a develop. Se comienza una rama por entregable
a partir de develop actualizado. No se trabaja directamente en develop.

| Fase | Integrante A | Integrante B | Integrante C | Condicion para cerrar la fase |
| --- | --- | --- | --- | --- |
| 1. Acuerdos y base | Fecha: validacion y comparaciones | Autor: datos y consultas | Libro: datos comunes y contrato de precio | Acordar firmas, tipos de claves, resultados de error y propiedad de punteros; probar las tres clases |
| 2. Trabajo paralelo | Novela, Texto y Escolar: precios, fecha y asociacion con Autor | ColeccionAutores, ABBAutores y sus iteradores | ColeccionLibros, HashLibros y sus iteradores | Compilar las interfaces acordadas; probar descuentos, orden, duplicados, colisiones y recorridos |
| 3. Operaciones de logica | Consultas de libros: prefijo y detalle (4 y 5) | Registro de autores y listado ordenado (1 y 6) | Registro de libros y asociacion de autor (2 y 3) | Probar operaciones vacias, claves inexistentes y resultados correctos |
| 4. Ventas y consultas agregadas | Ventas y precio final (7) | Recaudacion por editorial y libro mas vendido (8 y 10) | Unidades vendidas por precio y tipo (9) | Probar ventas sucesivas, tipos de libro, ausencia de editorial y empates |
| 5. Integracion y entrega | Main, entrada y mensajes | UML y justificacion de estructuras y memoria | Proyecto Code::Blocks y revision del paquete | Los diez requerimientos funcionan, CI pasa con cobertura minima 90%, documentos y codigo coinciden |

### Como evitar bloqueos y conflictos

- Antes de la fase 2, cerrar juntos las interfaces de Libro, Autor, colecciones
  e iteradores. Los nombres y tipos deben coincidir en las tres ramas.
- Cada integrante implementa contra esos contratos; los cambios de interfaz se
  acuerdan y se integran primero mediante un PR pequeno.
- En fases 3 y 4, escribir y probar cada operacion en el modulo que posee sus
  datos. Un responsable de integracion por fase conecta las llamadas en Fachada:
  A en fase 3 y C en fase 4. Asi no se editan simultaneamente sus mismos metodos.
- Los metodos de Fachada se acuerdan antes de comenzar esas fases. La logica no
  debe depender del menu: el main se conecta al final.
- Agregar pruebas en archivos separados por modulo y mantener un unico main de
  pruebas que las invoque. Actualizar el comando de CI al incorporar archivos
  con implementacion y nuevas pruebas; los archivos reservados vacios no bastan.
- Cada PR debe incluir alcance, precondiciones y pruebas. Otra persona lo revisa;
  la regla obligatoria de aprobacion se activara despues del primer merge acordado.
- Una fase no exige que todos terminen al mismo tiempo: se puede comenzar una
  tarea cuando sus dependencias esten integradas y probadas.

### Criterios de aceptacion por funcionalidad

| Requerimiento | Entregable comprobable |
| --- | --- |
| 1 | Registra autor; rechaza cedula repetida sin modificar la coleccion |
| 2 | Registra cada tipo de libro con cero ventas; rechaza ISBN repetido |
| 3 | Asocia y reemplaza autor; informa claves inexistentes y verifica que sea novela |
| 4 | Filtra por prefijo exacto del titulo y produce datos basicos; detecta resultado vacio |
| 5 | Consulta datos completos por ISBN e incluye autor cuando corresponda |
| 6 | Recorre autores por cedula ascendente; detecta coleccion vacia |
| 7 | Incrementa una venta y devuelve el precio con sus descuentos; rechaza ISBN inexistente |
| 8 | Suma unidades vendidas por precio de venta para la editorial consultada |
| 9 | Suma unidades, no titulos, con precio base estrictamente mayor; separa los tres tipos |
| 10 | Obtiene un libro con ventas maximas de la editorial, incluye detalle y autor; admite cualquier empate |

La eleccion inicial para autores es ABB: la letra no exige operaciones O(log n).
AVL queda como mejora posible, fuera del alcance inicial, si se decide garantizar
ese costo aun con cedulas insertadas en orden.
