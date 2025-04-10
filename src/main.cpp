#include "GSenku.hpp"

#include <iostream>

using namespace std;

int main()
{
    tpTablero tablero;

    inicializarTablero("../tableros_modelo/tableroEuropeo.txt", tablero);
    mostrarTablero(tablero);

    return 0;
}