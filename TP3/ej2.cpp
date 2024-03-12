#include <iostream>
#include <vector>
#include <queue>

#define INF INT32_MAX

using namespace std;

vector<vector<int>> latencias;
vector<vector<int>> G;

int c, N;


int main() {
    cin >> c;
    while (c > 0) {
        cin >> N;

        // NOTA: BASADO EN EL EJERCICIO 13 DE P4

        // Armamos la matriz de latencias todos a todos
        latencias = vector<vector<int>>(N, vector<int>(N, 0));

        // Armamos el grafo representado como una matriz (matriz de distancias con aristas de peso 1).
        G = vector<vector<int>>(N, vector<int>(N, 0));


        for (int j = 0; j < N; ++j) {
            for (int i = 0; i < j; ++i) {
                int v;
                cin >> v;
                latencias[i][j] = v;
                latencias[j][i] = v;
                G[i][j] = 1;
                G[j][i] = 1;
            }
        }

        bool valida = true;

        for (int k = 0; k < N && valida; ++k) {
            for (int i = 0; i < N && valida; ++i) {
                for (int j = 0; j < N && valida; ++j) {
                    if (k == i || k == j || i == j) continue;
                    // Chequeamos si la matriz de latencias es válida (check si vale postcondición de FW)
                    if (latencias[i][j] > latencias[i][k] + latencias[k][j]) {
                        valida = false;
                        // Sacamos las aristas redundantes del grafo
                    } else if (latencias[i][j] == latencias[i][k] + latencias[k][j]) {
                        G[i][j] = INF;
                        G[j][i] = INF;
                    }
                }
            }
        }

        // Floyd-Warshall: Armamos la matriz de distancias minimas (donde todas las aristas tienen peso 1).
        for (int k = 0; k < N; ++k) {
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    if ((G[i][j] > G[i][k] + G[k][j]) && G[i][k] != INF && G[k][j] != INF ){
                        G[i][j] = G[i][k] + G[k][j];
                        G[j][i] = G[i][k] + G[k][j];
                    }
                }
            }
        }

        if (!valida) {
            cout << "IMPOSIBLE" << endl;
            c--;
            continue;
        } else {
            cout << "POSIBLE" << endl;
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    cout << G[i][j] << " ";
                }
                printf("\n");
            }
        }
        c--;
    }
    return 0;
}