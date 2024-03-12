#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
typedef long long ll;

int N, M;

// Estructura para representar una arista con vértices y peso
struct Edge {
    int start, end;
    ll distance;
    ll repeaters;
    double weight;
};

// ######################################################## KRUSKAL
struct DSU {
    vector<int> p;
    vector<int> rank; // altura del árbol

    DSU(int n){
        p = vector<int>(n, -1);
        rank = vector<int>(n, 1);
    }

    void unite(int u, int v){
        int c1, c2;
        c1 = find(u); c2 = find(v);
        if(c1 == c2) return;
        if(rank[c2] > rank[c1]) swap(c1, c2);
        p[c2] = c1;
        rank[c1] = max(rank[c1], rank[c2]+1);
    }

    int find(int u){
        if(p[u] == -1) return u;
        int rep = find(p[u]);
        p[u] = rep;
        return rep;
    }
};

bool compareEdgesByWeightDesc(const Edge& a, const Edge& b) {
    if (a.weight != b.weight) {
        return a.weight > b.weight; // Ordenar en orden decreciente de peso
    }
    // Si los pesos son iguales, queremos colocar en el maxheap a la de menos repetidores primero
    return a.repeaters < b.repeaters;
}

pair<double, pair<ll, ll>> kruskal(vector<Edge>& edges) {
    sort(edges.begin(), edges.end(), compareEdgesByWeightDesc); // Ordena según w (el peso).
    DSU dsu(N);
    int aristasUsadas = 0;
    double pesoTotal = 0;
    ll suma_d = 0;
    ll suma_r = 0;
    for (auto e : edges) {
        double w = e.weight;
        ll r = e.repeaters, d = e.distance;
        int u = e.start, v = e.end;
        if (dsu.find(u) == dsu.find(v)) continue;
        dsu.unite(u, v);
        pesoTotal += w;
        suma_d += d;
        suma_r += r;
        aristasUsadas++;
        if (aristasUsadas == N - 1) {
            return {pesoTotal, {suma_d, suma_r}};
        }
    }
    return {pesoTotal, {suma_d, suma_r}};
}



// ######################################################## PRIM

// Función comparadora para usar en la cola de prioridad (max heap)
struct CompareEdges {
    bool operator()(const Edge& a, const Edge& b) {
        if (a.weight != b.weight) {
            return a.weight < b.weight; // Ordenar en orden decreciente de peso
        }
        // Si los pesos son iguales, queremos colocar en el maxheap a la de menos repetidores primero
        return a.repeaters > b.repeaters;
    }
};

pair<double, pair<ll, ll>> primMaxAdj(vector<vector<Edge>>& adj) {
    priority_queue<Edge, vector<Edge>, CompareEdges> maxHeap;
    vector<bool> visited(N, false);
    int startVertex = 0;
    visited[startVertex] = true;

    // Agrega todas las aristas conectadas al vértice de inicio en la cola de prioridad
    for (auto edge : adj[startVertex]) {
        maxHeap.push(edge);
    }

    vector<Edge> maximumSpanningTree;

    ll sum_d = 0, sum_r = 0;
    double treeWeight = 0;

    // Iteramos hasta que todas las aristas hayan sido procesadas o se haya formado el árbol
    while (!maxHeap.empty() && maximumSpanningTree.size() < N - 1) {
        Edge currentEdge = maxHeap.top();
        maxHeap.pop();

        // Determinamos el vértice no visitado (por las dudas).
        int unvisitedVertex = (visited[currentEdge.start]) ? currentEdge.end : currentEdge.start;

        if (!visited[unvisitedVertex]) {
            maximumSpanningTree.push_back(currentEdge);

            sum_d += currentEdge.distance;
            sum_r += currentEdge.repeaters;
            treeWeight += currentEdge.weight;
            visited[unvisitedVertex] = true;

            for (auto edge : adj[unvisitedVertex]) {
                if (!visited[edge.end]) {
                    maxHeap.push(edge);
                }
            }
        }
    }
    return {treeWeight, {sum_d, sum_r}};
}


int main() {
    int c;
    cin >> c;
    while (c > 0) {
        cin >> N >> M;

        // Lista de aristas para Kruskal
        vector<Edge> edges;

        // Lista de adyacencias para Prim
//        vector<vector<Edge>> listaAdj(N);

        ll max_d = 0;
        ll min_r = INT64_MAX;
        for (int i = 0; i < M; i++) {
            int u, v;
            ll d, r;
            cin >> u >> v >> d >> r;

            // Kruskal
            edges.push_back({u-1, v-1, d, r, 0});

            // Prim
//            listaAdj[u-1].push_back({u-1, v-1, d, r, 0});
//            listaAdj[v-1].push_back({v-1, u-1, d, r, 0});

            if (d > max_d) max_d = d;
            if (r < min_r) min_r = r;
        }

        // Variables para saber los rangos de la búsqueda binaria
        double cFin = (max_d / (double) min_r) + 1;
        double cInic = 0;

        // Variables para saber cuando terminar la búsqueda binaria
        bool termine = false;
        int existio = 2;
        double lastC = cFin + 1;

        ll dFin = 0, rFin = 0;
        while (!termine) {
            double cMid = (cInic + cFin) / 2;

            // Kruskal
            vector<Edge> modEdges;
            for (auto edge : edges) {
                double w_e = (double) edge.distance - (cMid * (double) edge.repeaters);
                modEdges.push_back({edge.start, edge.end, edge.distance, edge.repeaters, w_e});
            }
            pair<double, pair<int, int>> pesos = kruskal(modEdges);

            // Prim
//            vector<vector<Edge>> modListaAdj = listaAdj;
//            for (int i = 0; i < N; ++i) {
//                for (int j = 0; j < modListaAdj[i].size(); ++j) {
//                    modListaAdj[i][j].weight = modListaAdj[i][j].distance - (cMid * (double) modListaAdj[i][j].repeaters);
//                }
//            }
//            pair<double, pair<ll, ll>> pesos = primMaxAdj(modListaAdj);


            double cActual = pesos.second.first / (double) pesos.second.second;

            if (pesos.first >= 0) {
                cInic = cMid;
                if (existio == 0 && lastC == cActual) {
                    termine = true;
                }
                existio = 1;
            } else {
                cFin = cMid;
                if (existio == 1 && lastC == cActual) {
                    termine = true;
                }
                existio = 0;
            }

            lastC = cActual;

            dFin = pesos.second.first;
            rFin = pesos.second.second;
        }

        cout<<dFin<<" "<< rFin<<endl;

        c--;
    }

    return 0;
}