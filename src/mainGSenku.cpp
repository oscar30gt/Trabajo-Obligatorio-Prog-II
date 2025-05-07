/** ======================================================================
 * mainGSenku.cpp
 * Programa principal del juego GSenku.
 *
 * RESUMEN:
 * Este programa resuelve el juego GSenku utilizando un algoritmo de búsqueda
 * de soluciones por backtracking. La función `main` recibe dos ficheros con
 * el tablero a resolver y los movimientos permitidos, además de un retardo
 * en milisegundos entre cada movimiento para mostrar la resolución en tiempo
 * real. El resultado se guarda en un fichero de salida, que por defecto
 * es "resultado.txt". Se puede especificar un fichero de salida diferente
 * mediante un 4o argumento opcional.
 *
 * AUTORES:
 * - Hugo García Sánchez (nip: 930108)
 * - Óscar Grimal Torres (nip: 926897)
 * ==================================================================== */

#include <iostream>
#include <string>
#include "GSenku.hpp"

using namespace std;

/**
 * Lee los argumentos pasados al programa mediante de la línea de comandos y los almacena en las variables dadas.
 *
 * @param argc Número de argumentos pasados al programa.
 * @param argv Array de argumentos pasados al programa.
 * @param [out] ficheroTablero Nombre del fichero que contiene el tablero.
 * @param [out] ficheroMovimientos Nombre del fichero que contiene los movimientos permitidos.
 * @param [out] retardo Retardo en milisegundos entre cada movimiento.
 * @param [out] ficheroSalida Nombre del fichero donde se guardará el resultado.
 * @return `true` si los argumentos son válidos, `false` en caso contrario. En caso de error, se imprime el mensaje de error correspondiente.
 */
bool leerArgumentos(
    int argc, char *argv[],
    string &ficheroTablero,
    string &ficheroMovimientos,
    int &retardo,
    string &ficheroSalida)
{
    // Se deben pasar 3 o 4 argumentos (sin contar el nombre del ejecutable)
    if (!(argc == 4 || argc == 5))
    {
        cerr << "Error: Numero de argumentos incorrecto. Argumentos esperados:\033[37m ficheroTablero ficheroMovimientos retardo [ficheroSalida] \033[0m" << endl;
        return false;
    }

    // Ficheros de entrada
    ficheroTablero = string(argv[1]);
    ficheroMovimientos = string(argv[2]);

    // Retardo
    try
    {
        retardo = stoi(argv[3]);
    }
    catch (...)
    {
        cerr << "Error: El retardo especificado no es un numero entero." << endl;
        return false;
    }

    // Fichero de salida
    ficheroSalida = argc == 5 ? string(argv[4]) : "resultado.txt";

    return true;
}

/**
 * Progrema principal que resuleve el juego de GSenku.
 *
 * @param argc Número de argumentos pasados al programa.
 * @param argv Vector de argumentos pasados al programa.
 */
int main(int argc, char *argv[])
{
    // Lectura argumentos
    string ficheroTablero, ficheroMovimientos, ficheroSalida;
    int retardo;
    bool argumentosOk = leerArgumentos(argc, argv, ficheroTablero, ficheroMovimientos, retardo, ficheroSalida);
    if (!argumentosOk)
        return 1;

    // Inicialización de variables
    tpTablero tablero;
    tpMovimientosValidos movimientosValidos;
    tpListaMovimientos solucion;
    bool lecturaOk = inicializarTablero(ficheroTablero, tablero);
    if (!lecturaOk)
        return 1;
    lecturaOk = inicializarMovimientosValidos(ficheroMovimientos, movimientosValidos);
    if (!lecturaOk)
        return 1;

    // Busqueda de la solución
    int sol = buscaSolucion(tablero, movimientosValidos, solucion, retardo);
    if (sol == -1)
        cout << "No se ha encontrado solucion." << endl;
    else if (sol == 1)
        cout << "Se ha encontrado solucion." << endl;

    escribeListaMovimientos(ficheroSalida, solucion);
    return 0;
}