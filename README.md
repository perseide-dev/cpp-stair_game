# Informe del Juego “Escaleras y Serpientes 0-31”

**Lenguaje:** C++   
**Compilador recomendado:** `g++ -std=c++11`

---

## 1. Análisis de Requerimientos

| Componente | Detalle |
|-----------|---------|
| **Entradas** | - Número de jugadores (≥ 3)<br>- Pulsación Enter de cada jugador antes de lanzar<br>- Lanzamiento de dado (1 – 6) generado aleatoriamente |
| **Proceso** | 1. Inicializar posiciones en 0.<br>2. Repetir turnos por orden:<br>&nbsp;&nbsp;&nbsp;&nbsp;a. Esperar Enter.<br>&nbsp;&nbsp;&nbsp;&nbsp;b. Generar tirada.<br>&nbsp;&nbsp;&nbsp;&nbsp;c. Avanzar casilla.<br>&nbsp;&nbsp;&nbsp;&nbsp;d. Aplicar escalera/serpiente si corresponde.<br>&nbsp;&nbsp;&nbsp;&nbsp;e. Mostrar tablero y mensajes.<br>&nbsp;&nbsp;&nbsp;&nbsp;f. Verificar ganador (casilla 31). |
| **Salidas** | - Mensajes de turno y tirada.<br>- Avisos de sube / baja.<br>- Visualización del tablero con IDs de jugadores.<br>- Anuncio del ganador. |

---

## 2. Algoritmo Informal

- Preguntar cuántos jugadores participarán; validar que sean al menos tres.  
- Colocar a todos en la casilla 0.  
- Mientras nadie llegue a 31:  
  - Para cada jugador:  
    - Pedir que pulse Enter para lanzar.  
    - Tirar un dado de 6 caras.  
    - Moverse la cantidad obtenida (sin pasar de 31).  
    - Si la casilla es especial, subir o bajar según la tabla.  
    - Mostrar el tablero actualizado.  
    - Si el jugador llegó a 31, declarar la victoria y terminar.  

---

## 3. Algoritmo Formal (paso a paso)

```
Leer nJug

pos[i] ← 0 ⟵ 0 ≤ i < nJug

ganador ← FALSO

Mientras ¬ganador hacer
  Para turno ← 0 .. nJug-1 y ¬ganador
    EsperarEnter()
    dado ← Random(1,6)
    destino ← min(pos[turno] + dado, 31)
    Si destino ∈ especiales.origen entonces
      pos[turno] ← especiales.destino(destino)
    Sino
      pos[turno] ← destino
    ImprimirTablero(pos[])
    Si pos[turno] = 31 entonces
      ganador ← VERDADERO
```

---

## 4. Diagrama de Flujo (Mermaid)

```mermaid
flowchart TD
    A[Inicio] --> B{Leer nJug >= 3}
    B --> C[Inicializar posiciones en 0]
    C --> D{Ganador?}
    D -- No --> E[Por cada jugador]
    E --> F[Esperar Enter]
    F --> G[Tirar dado 1-6]
    G --> H[destino = pos + dado (max 31)]
    H --> I{Casilla especial?}
    I -- Si --> J[Actualizar a destino especial]
    I -- No --> K[Posicion = destino]
    J --> L[Mostrar tablero]
    K --> L
    L --> M{pos = 31?}
    M -- Si --> N[Ganador anunciado]
    M -- No --> E
    N --> O[Fin]
    D -- Si --> O
```

---

## 5. Codificación

### 5.1 Seudocódigo

```
const TABLERO_FIN ← 31
tablaEspeciales ← {{3,11},{9,23},{14,16},{18,30},{19,27},
                    {11,3},{23,9},{16,14},{30,18},{27,19}}

procedimiento main
    leer nJug
    repetir hasta que nJug ≥ 3
    crear arreglo pos[nJug] ← 0
    ganador ← falso
    mientras no ganador
        para turno = 0 hasta nJug-1 y no ganador
            esperarEnter()
            dado ← aleatorio(1..6)
            destino ← min(pos[turno]+dado,31)
            si destino existe en tablaEspeciales.origen entonces
                 pos[turno] ← tablaEspeciales.destino(destino)
                 anunciar subida/bajada
            sino pos[turno] ← destino
            imprimirTablero(pos)
            si pos[turno] = 31 entonces ganador ← verdadero
fin
```

### 5.2 Código Fuente C++

```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;
const int TABLERO_FIN = 31;
const int MAX_E = 10;
const int esp[MAX_E][2] = {
    {3,11},{9,23},{14,16},{18,30},{19,27},
    {11,3},{23,9},{16,14},{30,18},{27,19}
};
int dado() { return rand()%6+1; }
void esperar() {
    cout << "  (Enter) ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
int especial(int x) {
    for(int i=0;i<MAX_E;++i) if(x==esp[i][0]) return esp[i][1];
    return x;
}
void tablero(int n,const int p[]) {
    for(int c=0;c<=TABLERO_FIN;++c){
        cout.width(2); cout<<c<<":";
        bool f=false;
        for(int j=0;j<n;++j) if(p[j]==c){cout<<j+1;f=true;}
        if(!f)cout<<" ";
        cout<<'\t';
        if(c%8==7) cout<<'\n';
    }
    cout<<'\n';
}
int main(){
    srand(time(nullptr));
    int n; do{cout<<"Players (>=3): ";cin>>n;}while(n<3);
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    int* pos=new int[n]{0};
    bool win=false; cout<<"\n--- Start ---\n";
    while(!win){
        for(int t=0;t<n && !win;++t){
            cout<<"\nPlayer "<<t+1; esperar();
            int d=dado();
            int dest=min(pos[t]+d,TABLERO_FIN);
            cout<<"rolls "<<d<<" -> "<<dest;
            int fin=especial(dest);
            if(fin>dest) cout<<" climbs to "<<fin;
            else if(fin<dest) cout<<" slides to "<<fin;
            pos[t]=fin;
            if(fin==TABLERO_FIN){
                cout<<"\n*** Player "<<t+1<<" wins! ***\n";
                win=true;
            }
            tablero(n,pos);
        }
    }
    delete[] pos; return 0;
}
```

---

## 6. Descripción de las Estructuras Utilizadas

| Estructura     | Tipo                        | Uso                                                  |
|----------------|-----------------------------|------------------------------------------------------|
| `esp[10][2]`   | Matriz de enteros           | Almacena los pares origen → destino de escaleras y serpientes. |
| `pos[n]`       | Arreglo dinámico de enteros | Guarda la casilla actual de cada jugador.            |
| Variables simples | `int`, `bool`            | Contadores, tirada de dado, banderas de control.     |

> No se emplean vector, map, clases ni objetos, cumpliendo el paradigma exclusivamente procedimental.

---

## 7. Descripción de Resultados al Jugar

- **Interactividad:** Cada turno se detiene esperando que el jugador pulse Enter antes de lanzar, haciendo la partida participativa.

- **Mensajes de Estado:**
  - “Player X rolls Y → Z” indica la tirada y la casilla de destino.
  - “climbs to …” o “slides to …” anuncia la escalera/serpiente.

- **Tablero en Tiempo Real:** Tras cada movimiento se imprime un tablero de 0 a 31 con los ID de los jugadores en sus casillas; casillas compartidas muestran múltiples IDs concatenados.

- **Final:** Cuando algún jugador alcanza la casilla 31, se muestra `*** Player N wins! ***` y concluye la ejecución.

> Con estas salidas el usuario puede verificar visualmente el avance, las subidas y bajadas, y la condición de victoria de forma clara.
