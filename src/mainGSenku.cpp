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

    ficheroTablero = string(argv[1]);
    ficheroMovimientos = string(argv[2]);

    try
    {
        retardo = stoi(argv[3]);
    }
    catch (...)
    {
        cerr << "Error: El retardo especificado no es un numero entero." << endl;
        return false;
    }

    if (argc == 5)
        ficheroSalida = string(argv[4]);
    else
        ficheroSalida = "resultado.txt";

    return true;
}

/**
 * Progrema principal que resuleve el juego de GSenku.
 *
 * @param argc Número de argumentos pasados al programa.
 * @param argv Array de argumentos pasados al programa.
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