#include <iostream>
#include <vector>
#include <cstdint>
#include <queue>

using namespace std;

#define INF INT32_MAX

int c, N, cantNodos;
vector<vector<int>> tablero;

vector<vector<int>> capacity; // Digrafo con las capacidades para la red de flujo
vector<vector<int>> adj; // Grafo con aristas de la red

// BFS para edmonds-karp
int bfs(int s, int t, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int next : adj[cur]) {
            if (parent[next] == -1 && capacity[cur][next]) {
                parent[next] = cur;
                int new_flow = min(flow, capacity[cur][next]);
                if (next == t)
                    return new_flow;
                q.push({next, new_flow});
            }
        }
    }

    return 0;
}

// Implementación Edmond-Karp de flujo máximo
int maxflow(int s, int t) {
    int flow = 0;
    vector<int> parent(cantNodos+2);
    int new_flow;

    while (new_flow = bfs(s, t, parent)) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow;
            capacity[cur][prev] += new_flow;
            cur = prev;
        }
    }

    return flow;
}

int contarEspaciosDeCerosFila(const std::vector<std::vector<int>>& matriz, int fila) {
    int contador = 0;
    int subsecuencia = 0;

    for (int j = 0; j < matriz[fila].size(); j++) {
        if (matriz[fila][j] == 0) {
            subsecuencia = 1;  // Comienza un nuevo espacio de ceros
        } else {
            if (subsecuencia > 0) {
                contador++;  // Se encontró un espacio de ceros
                subsecuencia = 0;
            }
        }
    }

    if (subsecuencia > 0) {
        contador++;  // Termina el espacio
    }

    return contador;
}

int contarEspaciosDeCerosColumna(const std::vector<std::vector<int>>& matriz, int columna) {
    int contador = 0;
    int subsecuencia = 0;

    for (int i = 0; i < matriz.size(); i++) {
        if (matriz[i][columna] == 0) {
            subsecuencia = 1;  // Comienza un nuevo espacio de ceros
        } else {
            if (subsecuencia > 0) {
                contador++;  // Se encontró un espacio de ceros
                subsecuencia = 0;
            }
        }
    }

    if (subsecuencia > 0) {
        contador++;  // Termina el espacio
    }

    return contador;
}

int main() {
    cin >> c;
    while (c > 0) {
        cin >> N;
        tablero = vector<vector<int>>(N, vector<int>(N, 0));

		// Me guardo mi tablero en una matriz
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int casilla;
                cin >> casilla;
                tablero[i][j] = casilla;
            }
        }
		
		// Voy a contar la cant de nodos de mi red de flujo, esta contiene 2 capas:
		//    Capa filas: hay 1 nodo por cada conjunto de casilleros que podría poner 1 torre, en cada fila.
		//                O sea, si en una fila de 3 casillas la segunda está rota, se insertan 2 nodos porque podrian ponerse 2 torres
		//    Capa columnas: mismo criterio que capa filas pero ahora para columnas
		int cantSubFilas = 0;
		int cantSubColumnas = 0;
		vector<pair<int, int>> contadoresSubColumnas;
		vector<bool> last_in_col_was_cero(N, false);
		vector<pair<int, int>> contadoresSubFilas;
        
        for (int ri = 0; ri < N; ++ri) {
			int subfilas = contarEspaciosDeCerosFila(tablero, ri);
			contadoresSubFilas.push_back({cantSubFilas, 0});
            cantSubFilas += subfilas;
        }
		contadoresSubFilas.push_back({cantSubFilas, 0});
		
        for (int cj = 0; cj < N; ++cj) {
			int subcolumnas = contarEspaciosDeCerosColumna(tablero, cj);
			contadoresSubColumnas.push_back({cantSubColumnas, 0});
            cantSubColumnas += subcolumnas;
        }
		contadoresSubColumnas.push_back({cantSubColumnas, 0});
		
		cantNodos = cantSubFilas + cantSubColumnas;

		// Mi nodo 0 es s, luego vienen todos los nodos de mi primera capa, luego los de la 2da y finalmente t
        int s = 0;
        int t = (cantNodos) + 1;
		int inicioFilas = 1; // me guardo donde empieza la capa de filas
		int inicioCols = cantSubFilas+1; // me guardo donde empieza la capa de columnas
		
		adj = vector<vector<int>>((cantNodos) + 2, vector<int>());
        capacity = vector<vector<int>>((cantNodos) + 2, vector<int>((cantNodos) + 2, 0));
		
		// Armo la red de flujo segun las "intersecciones" de mis subfilas y subfilas
		for (int i = 0; i < N; ++i) {
			int ri = inicioFilas+contadoresSubFilas[i].first;
			bool last_in_row_was_cero = false; // si hay varios casilleros rotos seguidos no quiero añadir nodos de más
			
			for (int j = 0; j < N; ++j) {
				// necesito ajustar bien mis indices segun la cant de nodos de mis cols pasadas, para eso los contadores
				int cj = inicioCols+contadoresSubColumnas[j].first+contadoresSubColumnas[j].second;
                // En adj añado arista en ambas direcciones
				// en capacity solo s->ri->ci->t
				if (tablero[i][j] != 1) {
					adj[ri].push_back(cj);
					adj[cj].push_back(ri);
                    capacity[ri][cj] = 1;
					last_in_row_was_cero = true;
					last_in_col_was_cero[j] = true;
                } else {
					if (last_in_row_was_cero) ri++;
					if (last_in_col_was_cero[j]) contadoresSubColumnas[j].second++;
					last_in_row_was_cero = false;
					last_in_col_was_cero[j] = false;
				}
			}
        }
		
		// Conecto s con la 1ra capa
		for (int i = 0; i < cantSubFilas; ++i) {
			int ri = inicioFilas+i;
            adj[s].push_back(ri);
            adj[ri].push_back(s);
            capacity[s][ri] = 1;
        }

		// Conecto la 2da capa con t
        for (int j = 0; j < cantSubColumnas; ++j) {
			int cj = inicioCols+j;
            adj[t].push_back(cj);
            adj[cj].push_back(t);
            capacity[cj][t] = 1;
        }

        // Calculo flujo maximo
        int res = maxflow(s, t);

        cout << res << endl;

        c--;
    }
    return 0;
}
