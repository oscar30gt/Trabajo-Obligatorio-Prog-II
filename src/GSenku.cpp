#include "GSenku.hpp"
#include <fstream>

// Pre: true
// Post: lee la configuración y el estado del tablero del fichero de configuración que se le pasa como argumento
//      inicializando tablero y devolviendo true si todo ha ido bien y false si ha habido algún error
bool inicializarTablero(const string nombreFichero, tpTablero &tablero)
{
    ifstream f(nombreFichero);
    if (!f.is_open())
    {
        cerr << "Error al abrir el fichero del tablero." << endl;
        return false;
    }

    f >> tablero.nfils >> tablero.ncols;
    for (int i = 0; i < tablero.nfils; ++i)
        for (int j = 0; j < tablero.ncols; ++j)
        {
            char aux;
            f >> aux;
            tablero.matriz[i][j] = (tpEstadoCelda)aux;
        }

    f.close();
    return true;
}

// Pre: true
// Post: lee los movimientos válidos del fichero que se le pasa como argumento
//      inicializando la estructura y devolviendo true si todo ha ido bien y false si ha habido algún error
bool inicializarMovimientosValidos(const string nombreFichero, tpMovimientosValidos &movimientos)
{
    ifstream f(nombreFichero);
    if (!f.is_open())
    {
        cerr << "Error al abrir el fichero de movimientos." << endl;
        return false;
    }

    int i = 0;
    while (i < 8)
    {
        char aux;
        f >> aux;

        if (aux == '-')
            movimientos.validos[i++] = false;
        else if (aux == '+')
            movimientos.validos[i++] = true;
    }

    f.close();
    return true;
}

// Pre: tablero contiene el estado actual de la ejecución de la búsqueda de la solución
// Post: Se ha mostrado el tablero por pantalla
void mostrarTablero(const tpTablero &tablero)
{
    for (int i = 0; i < tablero.nfils; ++i)
    {
        for (int j = 0; j < tablero.ncols; ++j)
            cout << (char)tablero.matriz[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

// Pre: tablero contiene el estado inicial del que se parte para la búsqueda,
//      movimientosValidos contiene los movimientos que están permitidos,
//      solucionParcial contiene la solución actual como lista de movimientos, En el tablero se han colocada las n primeras piezas de vEntrada, en la columnas indicadas respectivamente en vSalida
// Post: solucionParcial contendrá la lista de movimientos completa (si no se llega a una solución, estará vacía, numMovs == 0)
//       Devuelve 1 si encuentra solución, -1 si no la encuentra.
int buscaSolucion(tpTablero &tablero, const tpMovimientosValidos &movValidos, tpListaMovimientos &solucionParcial, const int retardo = 0)
{
}

// Pre: listaMovimientos contiene la lista de movimientos con la solucion
// Post: escribe la lista de movimientos en el fichero que se le pasa como argumento siguiendo el
//      formato especificado en el guión (si está vacía, se escribe un -1 en el fichero)
void escribeListaMovimientos(string nombreFichero, const tpListaMovimientos &solucion)
{
}
