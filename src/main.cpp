#include <iostream>
#include <string>
#include "GSenku.hpp"

using namespace std;

/**
 * Lee los argumentos pasados al programa mediante de la línea de comandos y los almacena en las variables dadas.
 * 
 * @param argc Número de argumentos pasados al programa.
 * @param argv Array de argumentos pasados al programa.
 * @param [out] ficheroTablero Nombre del fichero que contiene el tablero. Sin extensión.
 * @param [out] ficheroMovimientos Nombre del fichero que contiene los movimientos permitidos. Sin extensión.
 * @param [out] retardo Retardo en milisegundos entre cada movimiento.
 * @param [out] ficheroSalida Nombre del fichero donde se guardará el resultado.
 * @return `true` si los argumentos son válidos, `false` en caso contrario. En caso de error, se imprime un mensaje de error en la salida estándar.
 */
bool leerArgumentos(
    int argc, char *argv[],
    string &ficheroTablero,
    string &ficheroMovimientos,
    int &retardo,
    string &ficheroSalida)
{
    // Se debben pasar 3 o 4 argumentos (sin contar el nombre del ejecutable)
    if (!(argc == 4 || argc == 5))
    {
        cerr << "Error: Número de argumentos incorrecto. Argumentos esperados `ficheroTablero ficheroMovimientos retardo [ficheroSalida]`" << endl;
        return false;
    }

    ficheroTablero = "./tableros_modelo/" + string(argv[1]) + ".txt";
    ficheroMovimientos = "./movimientos/" + string(argv[2]) + ".txt";

    try
    {
        retardo = stoi(argv[3]);
    }
    catch (...)
    {
        cerr << "Error: El retardo especificado no es un número entero." << endl;
        return false;
    }

    if (argc == 5)
        ficheroSalida = string(argv[4]);
    else
        ficheroSalida = "resultado.txt";

    return true;
}

int main(int argc, char *argv[])
{

    string ficheroTablero, ficheroMovimientos, ficheroSalida;
    int retardo;
    bool argumentosOk = leerArgumentos(argc, argv, ficheroTablero, ficheroMovimientos, retardo, ficheroSalida);

    // Borrar para la entega. Solo para ver que se leen los argumentos
    cout << "Argumentos leídos: " << endl
         << "------------------------------------------------" << endl
         << "Fichero Tablero: " << ficheroTablero << endl
         << "Fichero Movimientos: " << ficheroMovimientos << endl
         << "Retardo: " << retardo << endl
         << "Fichero Salida: " << ficheroSalida << endl
         << "------------------------------------------------" << endl;

    tpTablero tablero;
    tpMovimientosValidos movimientosValidos;

    bool lecturaOk = inicializarTablero("./tableros_modelo/tableroEuropeo.txt", tablero);
    lecturaOk = inicializarMovimientosValidos("./movimientos/movimientosClasicos.txt", movimientosValidos);

    mostrarTablero(tablero);

    return 0;
}