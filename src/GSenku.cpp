/** ======================================================================
 * GSenku.cpp
 * Fichero de implementación de las funciones de la interfaz "GSenku.hpp" 
 *
 * AUTORES:
 * - Hugo García Sánchez (nip: 930108)
 * - Óscar Grimal Torres (nip: 926897)
 * ==================================================================== */

#include "GSenku.hpp"
#include <fstream>
#include <chrono>
#include <thread>

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

    for (int y = 0; y < tablero.nfils; y++)
        for (int x = 0; x < tablero.ncols; x++)
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
            switch (tablero.matriz[x][y])
            {
                case VACIA:
                    cout << "\033[47m" << "  " << "\033[0m";
                    break;
                case OCUPADA:
                    cout << "\033[47;31m" << "O " << "\033[0m";
                    break;
                case NO_USADA:
                    cout << "\033[41m" << "  " << "\033[0m";
                    break;
            }
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
    int fichas = 0;
    for (int y = 0; y < tablero.nfils; y++)
        for (int x = 0; x < tablero.ncols; x++)
            if (tablero.matriz[x][y] == OCUPADA)
                fichas++;

    if (fichas == 1)
        return 1; // Solución encontrada

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

            // Itera sobre las celdas adyacentes
            for (int i = 0; i < 8; i++)
            {
                // Prueba un movimiento
                bool movValido = moverFicha(tablero, movValidos, solucionParcial, {x, y}, (tpDireccion)i);
                if (!movValido)
                    continue;

                // Movimiento realizado
                if (retardo > 0)
                {
                    mostrarTablero(tablero);
                    cout << "\033[" << tablero.nfils + 1 << "A";
                    this_thread::sleep_for(chrono::milliseconds(retardo));
                }
            
                // Llamada recursiva para buscar la solución desde el nuevo estado del tablero
                int solucion = buscaSolucion(tablero, movValidos, solucionParcial, retardo);
                if (solucion == 1)
                    return 1;

                // Si no se encuentra solución, deshacer el movimiento y seguir buscando
                tpMovimientoPieza movDeshacer = solucionParcial.pop();
                tablero.matriz[movDeshacer.origen.x][movDeshacer.origen.y] = OCUPADA;
                tablero.matriz[(movDeshacer.origen.x + movDeshacer.destino.x) / 2]
                              [(movDeshacer.origen.y + movDeshacer.destino.y) / 2] = OCUPADA;
                tablero.matriz[movDeshacer.destino.x][movDeshacer.destino.y] = VACIA;

                // Movimiento revertido
                if (retardo > 0)
                {
                    mostrarTablero(tablero);
                    cout << "\033[" << tablero.nfils + 1 << "A";
                    this_thread::sleep_for(chrono::milliseconds(retardo));
                }
            }
        }

    // Si despues de probar todos los movimientos no se ha encontrado solución, devolver -1
    return -1;
}

// Pre: tablero contiene el estado del tablero desde el que se quiere mover una ficha
//      movValidos contiene los movimientos válidos a realizar
//      solucionParcial contiene la lista de movimientos realizados hasta el momento
//      posicionInicial contiene la posición inicial de la ficha a mover (no necesariamente tiene que haber una ficha en esa posición)
//      direccionMovimiento contiene la dirección del movimiento (podría ser un movimiento no válido)
// Post: mueve la ficha en la dirección indicada y actualiza el tablero y la lista de movimientos. Si no se puede mover, devuelve false y no actualiza nada.
bool moverFicha(tpTablero &tablero, const tpMovimientosValidos &movValidos, tpListaMovimientos &solucionParcial, const tpPosicion posicionInicial, const tpDireccion direccionMovimiento)
{
    if (!movValidos.validos[direccionMovimiento])
        return false; // Movimiento no válido

    // Comprobar que se salta a una celda vacía
    tpPosicion nuevaPosicion = {posicionInicial.x, posicionInicial.y};
    switch (direccionMovimiento)
    {
        case superiorIzquierda:
            nuevaPosicion.x -= 2;
            nuevaPosicion.y -= 2;
            break;
        case superiorDerecha:
            nuevaPosicion.x += 2;
            nuevaPosicion.y -= 2;
            break;
        case inferiorIzquierda:
            nuevaPosicion.x -= 2;
            nuevaPosicion.y += 2;
            break;
        case inferiorDerecha:
            nuevaPosicion.x += 2;
            nuevaPosicion.y += 2;
            break;
        case izquierda:
            nuevaPosicion.x -= 2;
            break;
        case derecha:
            nuevaPosicion.x += 2;
            break;
        case superior:
            nuevaPosicion.y -= 2;
            break;
        case inferior:
            nuevaPosicion.y += 2;
            break;
        default:
            return false;
    }

    if (nuevaPosicion.x < 0 || nuevaPosicion.x >= tablero.ncols ||
        nuevaPosicion.y < 0 || nuevaPosicion.y >= tablero.nfils ||
        tablero.matriz[nuevaPosicion.x][nuevaPosicion.y] != VACIA)
        return false; // Movimiento fuera de los límites del tablero o celda ocupada/no usada

    // Comprobar que se salta sobre una celda ocupada
    tpPosicion posicionIntermedia = {(posicionInicial.x + nuevaPosicion.x) / 2, (posicionInicial.y + nuevaPosicion.y) / 2};
    if (tablero.matriz[posicionIntermedia.x][posicionIntermedia.y] != OCUPADA)
        return false; // Un movimiento siempre salta una celda ocupada

    // MOVIMIENTO VÁLIDO
    // Actualizar el tablero
    tablero.matriz[posicionInicial.x][posicionInicial.y] = VACIA;
    tablero.matriz[posicionIntermedia.x][posicionIntermedia.y] = VACIA;
    tablero.matriz[nuevaPosicion.x][nuevaPosicion.y] = OCUPADA;

    // Actualizar la lista de movimientos
    tpMovimientoPieza nuevoMovimiento = {posicionInicial, nuevaPosicion};
    solucionParcial.push(nuevoMovimiento);
    return true;
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

    // Si la lista de movimientos está vacía, se escribe un -1
    if (solucion.numMovs == 0)
        f << -1 << endl;

    else
        for (int i = 0; i < solucion.numMovs; i++)
            f << solucion.movs[i].origen.y << ","
              << solucion.movs[i].origen.x << ":"
              << solucion.movs[i].destino.y << ","
              << solucion.movs[i].destino.x << endl;

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