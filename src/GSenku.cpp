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
    for (int y = 0; y < tablero.ncols; y++)
        for (int x = 0; x < tablero.nfils; x++)
        {
            char aux;
            f >> aux;
            tablero.matriz[x][y] = (tpEstadoCelda)aux;
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
    for (int y = 0; y < tablero.nfils; y++)
    {
        for (int x = 0; x < tablero.ncols; x++)
            cout << (char)tablero.matriz[x][y] << " ";
        cout << endl;
    }
    cout << endl;
}

// Pre: tablero contiene el estado inicial del que se parte para la búsqueda,
//      movimientosValidos contiene los movimientos que están permitidos,
//      solucionParcial contiene la solución actual como lista de movimientos, En el tablero se han colocada las n primeras piezas de vEntrada, en la columnas indicadas respectivamente en vSalida
// Post: solucionParcial contendrá la lista de movimientos completa (si no se llega a una solución, estará vacía, numMovs == 0)
//       Devuelve 1 si encuentra solución, -1 si no la encuentra.
int buscaSolucion(tpTablero &tablero, const tpMovimientosValidos &movValidos, tpListaMovimientos &solucionParcial, const int retardo)
{
    tpSimetria simetria = simetriaTablero(tablero);

    // Area de búsqueda
    // Si el tablero es simétrico, se puede reducir la búsqueda a la mitad
    int buscarX = (simetria & 0b01) ? (tablero.ncols + 1) / 2 : tablero.ncols;
    int buscarY = (simetria & 0b10) ? (tablero.nfils + 1) / 2 : tablero.nfils;

    // Itera sobre el tablero probando movimientos
    for (int y = 0; y < buscarY; y++)
        for (int x = 0; x < buscarX; x++)
        {
            tpEstadoCelda celda = tablero.matriz[x][y];
            if (celda != OCUPADA)
                continue;
        }

    return 1; // Placeholder: Implementar la lógica de búsqueda de solución
}

// Pre: listaMovimientos contiene la lista de movimientos con la solucion
// Post: escribe la lista de movimientos en el fichero que se le pasa como argumento siguiendo el
//      formato especificado en el guión (si está vacía, se escribe un -1 en el fichero)
void escribeListaMovimientos(string nombreFichero, const tpListaMovimientos &solucion)
{
    ofstream f(nombreFichero);
    if (!f.is_open())
    {
        cerr << "Error al abrir el fichero de salida." << endl;
        return;
    }

    if (solucion.numMovs == 0)
        f << -1 << endl;

    else
        for (int i = 0; i < solucion.numMovs; i++)
            f << solucion.movs[i].origen.x << ","
              << solucion.movs[i].origen.y << ":"
              << solucion.movs[i].destino.x << ","
              << solucion.movs[i].destino.y << endl;

    f.close();
}

// Pre: tablero contiene el estado del tablero
// Post: devuelve la simetría del tablero (vertical, horizontal, ambas o ninguna)
tpSimetria simetriaTablero(const tpTablero &tablero)
{
    tpSimetria simetria = NO_SIMETRICA;

    // Simetría horizontal
    // Se comprueba si el tablero es simétrico respecto al eje vertical
    bool simetricaHorizontal = true;
    for (int y = 0; y < tablero.nfils; y++)
        for (int x = 0; x < tablero.ncols / 2; x++)
            if (tablero.matriz[x][y] != tablero.matriz[tablero.ncols - x - 1][y])
                simetricaHorizontal = false;

    if (simetricaHorizontal)
        simetria = HORIZONTAL;


    // Simetría vertical
    // Se comprueba si el tablero es simétrico respecto al eje horizontal
    bool simetricaVertical = true;
    for (int y = 0; y < tablero.nfils / 2; y++)
        for (int x = 0; x < tablero.ncols; x++)
            if (tablero.matriz[x][y] != tablero.matriz[x][tablero.nfils - y - 1])
                simetricaVertical = false;

    if (simetricaVertical)
        simetria = (tpSimetria)(simetria | VERTICAL);

    return simetria;
}