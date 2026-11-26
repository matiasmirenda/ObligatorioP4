#ifndef PARAMETROS_H
#define PARAMETROS_H

const float FACTOR_PRECIO_INFANTIL = 0.80f;
const float FACTOR_PRECIO_TEXTO_ANTIGUO = 0.75f;
const float FACTOR_PRECIO_ESCOLAR_INICIAL = 0.90f;
const int ENERO = 1;
const int FEBRERO = 2;
const int MARZO = 3;
const int ABRIL = 4;
const int MAYO = 5;
const int JUNIO = 6;
const int JULIO = 7;
const int AGOSTO = 8;
const int SEPTIEMBRE = 9;
const int OCTUBRE = 10;
const int NOVIEMBRE = 11;
const int DICIEMBRE = 12;
const int PRIMER_DIA_MES = 1;
const int DIAS_MES_LARGO = 31;
const int DIAS_MES_CORTO = 30;
const int DIAS_FEBRERO_BISIESTO = 29;
const int DIAS_FEBRERO_COMUN = 28;
const int CICLO_BISIESTO = 4;
const int ANIOS_POR_SIGLO = 100;
const int CICLO_SIGLO_BISIESTO = 400;
const int DIA_CORTE_DESCUENTO = PRIMER_DIA_MES;
const int MES_CORTE_DESCUENTO = ENERO;
const int ANIO_CORTE_DESCUENTO = 2026;
const int ANIO_PUBLICACION_MINIMO = 1900;
const int ANIO_PUBLICACION_MAXIMO = 2099;
const int ANIO_ESCOLAR_MINIMO = 1;
const int ANIO_ESCOLAR_MAXIMO = 6;
const int ULTIMO_ANIO_DESCUENTO_ESCOLAR = 3;
const int MAX_LARGO_TEXTO = 100;
const int DIGITOS_CEDULA = 8;
const int DIGITOS_ISBN = 9;

const int BASE_DECIMAL = 10;
const float VALOR_DECIMA = 0.1f;
const char DIGITO_CERO = '0';
const char DIGITO_NUEVE = '9';
const char SEPARADOR_DECIMAL = '.';
const int MIN_LARGO_TEXTO = 1;

const int OPCION_TIPO_NOVELA = 1;
const int OPCION_TIPO_TEXTO = 2;
const int OPCION_TIPO_ESCOLAR = 3;

const int OPCION_SALIR = 0;
const int OPCION_REGISTRAR_AUTOR = 1;
const int OPCION_REGISTRAR_LIBRO = 2;
const int OPCION_ASOCIAR_AUTOR = 3;
const int OPCION_LISTAR_POR_PREFIJO = 4;
const int OPCION_CONSULTAR_LIBRO = 5;
const int OPCION_LISTAR_AUTORES = 6;
const int OPCION_REGISTRAR_VENTA = 7;
const int OPCION_CALCULAR_RECAUDACION = 8;
const int OPCION_CONTAR_VENTAS_POR_TIPO = 9;
const int OPCION_CONSULTAR_MAS_VENDIDO = 10;

#endif
