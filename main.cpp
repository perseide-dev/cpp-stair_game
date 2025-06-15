#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>      // para std::numeric_limits

using namespace std;

/* ----------------- Constantes ----------------- */
const int TABLERO_FIN    = 31;
const int MAX_ESPECIALES = 10;     // 5 pares (sube/baja)

/* pares origen-destino (escaleras y serpientes) */
const int especiales[MAX_ESPECIALES][2] = {
    { 3, 11}, { 9, 23}, {14, 16}, {18, 30}, {19, 27},
    {11,  3}, {23,  9}, {16, 14}, {30, 18}, {27, 19}
};

/* ------------- Funciones utilitarias ---------- */
int tirarDado() { return rand() % 6 + 1; }

void solicitarJugadores(int& n)
{
    do {
        cout << "Enter a number of players (>=3): ";
        cin  >> n;
        if (n < 3) cout << "Number of players must be at least 3.\n";
    } while (n < 3);
}

int aplicarEspecial(int posIni, int& posFin)
{
    for (int i = 0; i < MAX_ESPECIALES; ++i)
        if (posIni == especiales[i][0]) {
            posFin = especiales[i][1];
            return (posFin > posIni) ? 1 : -1;
        }
    posFin = posIni;
    return 0;
}

void imprimirTablero(int nJug, const int pos[])
{
    cout << "\nTablero:\n";
    for (int cas = 0; cas <= TABLERO_FIN; ++cas) {
        cout.width(2); cout << cas << ":";
        bool alguien = false;
        for (int j = 0; j < nJug; ++j)
            if (pos[j] == cas) { cout << (j + 1); alguien = true; }
        if (!alguien) cout << ' ';
        cout << '\t';
        if (cas % 8 == 7) cout << '\n';
    }
    cout << '\n';
}

/* ==================== MAIN ==================== */
int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    int nJug = 0;
    solicitarJugadores(nJug);

    // limpiar el '\n' pendiente tras leer el número
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int* pos = new int[nJug];
    for (int i = 0; i < nJug; ++i) pos[i] = 0;

    bool ganador = false;
    cout << "\n--- ¡Comienza el juego! ---\n";

    while (!ganador)
    {
        for (int turno = 0; turno < nJug && !ganador; ++turno)
        {
            /* --- pausa: esperar Enter --- */
            cout << "\nJugador " << (turno + 1)
                 << ", presiona Enter para lanzar el dado...";
            cin.get();                          // espera al usuario

            int dado    = tirarDado();
            int destino = pos[turno] + dado;
            if (destino > TABLERO_FIN) destino = TABLERO_FIN;

            cout << " obteniste un " << dado
                 << " -> avanzas a " << destino;

            int posFinal, tipo = aplicarEspecial(destino, posFinal);
            if (tipo ==  1) cout << " y **subes** a "   << posFinal;
            if (tipo == -1) cout << " y **bajas** a "   << posFinal;

            pos[turno] = posFinal;
            if (posFinal == TABLERO_FIN) {
                cout << "\n\n*** ¡El jugador " << (turno + 1)
                     << " ha ganado! ***\n";
                ganador = true;
            }

            imprimirTablero(nJug, pos);
        }
    }

    delete[] pos;
    return 0;
}