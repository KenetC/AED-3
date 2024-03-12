#include <iostream>
#include <vector>
#include <queue>

#define INF INT32_MAX

using namespace std;

int c, N, M;
vector<vector<pair<int, int>>> D;
vector<vector<pair<int, int>>> D_sin_pasadizos;
vector<vector<int>> pasadizos;
vector<vector<int>> memo;


//void dijkstra() {
//    vector<bool> procesado(N+1);
//    for (int i = 1; i < N + 1; ++i) {
//        distancia[i] = INF;
//        procesado[i] = false;
//    }
//    distancia[0] = 0;
//    int p_usados = 0;
//
//    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> Q;
//    Q.emplace(0, 0);
//    while (!Q.empty()) {
//        if (procesado[N]) { // Queremos encontrar el camino mínimo hasta el último salón
//            break;
//        }
//
//        int nodo = Q.top().second; Q.pop();
//
//        if (procesado[nodo]) continue;
//        procesado[nodo] = true;
//
//        for (auto vecinoPair : D[nodo]) {
//            int vecino = vecinoPair.first;
//            int costo = vecinoPair.second;
//            if (distancia[nodo] + costo < distancia[vecino]) {
//                if (costo == 2) {
//                    if (p_usados >= 3) continue;
//                    p_usados++;
//                }
//                distancia[vecino] = distancia[nodo] + costo;
//                Q.emplace(distancia[vecino], vecino);
//            }
//        }
//    }
//}

int rescate() {
    vector<int> pasadizos(N+1, 0);
    vector<int> distancia(N+1, INF);
    distancia[0] = 0;

    // Hacemos un dijkstra bottom up usando el topological order que son los nodos ordenados ascendentemente [0,1,2...,N-1,N].
    for (int v = 0; v < N + 1; ++v) {
        for (auto vecinoPair : D[v]) {
            int w = vecinoPair.first;
            int costo = vecinoPair.second;
            int nueva_distancia = distancia[v] + costo;

            int p_utilizados = pasadizos[v] + (costo == 2 ? 1 : 0);
            if (p_utilizados > 3) continue; // Si ya usamos 3 pasadizos no podemos volver a usar otro

            if (distancia[w] > nueva_distancia) {
                distancia[w] = nueva_distancia;
                pasadizos[w] = p_utilizados;
            }

        }
    }
    return distancia[N];
}

int rescate_td(int v, int pasadizos_usados) {
    // Casos base
    if (v == N) {
        return 0;
    }

    if (memo[v][pasadizos_usados] != -1) return memo[v][pasadizos_usados];

    int rescate_saltando_muralla = 1 + rescate_td(v+1, pasadizos_usados);

    if (pasadizos_usados == 3 || pasadizos[v].empty()) {
        return memo[v][pasadizos_usados] = rescate_saltando_muralla;
    }

    // Paso recursivo
    int rescate_min = rescate_saltando_muralla;
    for (int w : pasadizos[v]) {
        int rescate_usando_pasadizo = 2 + rescate_td(w, pasadizos_usados + 1);
        if (rescate_usando_pasadizo < rescate_min) rescate_min = rescate_usando_pasadizo;
    }

    return memo[v][pasadizos_usados] = rescate_min;
}

int main() {
    cin >> c;
    while (c > 0) {
        cin >> N >> M;

        // Contamos que el nodo 0 es donde empieza Pablo
        D = vector<vector<pair<int,int>>>(N + 1, vector<pair<int,int>>());
        D_sin_pasadizos = vector<vector<pair<int,int>>>(N + 1, vector<pair<int,int>>());
        pasadizos = vector<vector<int>>(N + 1, vector<int>());
        memo = vector<vector<int>>(N + 1, vector<int>(4, -1));

        // Añadimos los pasadizos
        for (int i = 0; i < M; ++i) {
            int v, w;
            cin >> v >> w;
            if (w - v <= 2) continue;
            D[v].emplace_back(w, 2);
            pasadizos[v].emplace_back(w);
        }
        // Añadimos las aristas que representan saltar una muralla
        for (int i = 0; i < N; ++i) {
            D[i].emplace_back(i+1, 1);
            D_sin_pasadizos[i].emplace_back(i+1, 1);
        }

//        int camino_min = rescate();
        int camino_min = rescate_td(0, 0);
        cout << camino_min << endl;

        c--;
    }
    return 0;
}
