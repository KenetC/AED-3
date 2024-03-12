#include <iostream>
#include <vector>
#include <iomanip>
#include <stack>
#include <algorithm>

using namespace std;

vector<vector<int>> G;
vector<bool> visited;
vector<vector<int>> treeEdges;
vector<vector<int>> treeEdgesT;
vector<vector<int>> backEdges;
vector<vector<int>> backEdgesT;
int NO_LO_VI = 0, EMPECE_A_VER = 1, TERMINE_DE_VER = 2;
vector<int> estado;
vector<int> backEdgesConExtremoINFERIOR;
vector<int> backEdgesConExtremoSUPERIOR;
vector<int> padres;
vector<int> memo;
int n, m;

void inicializarVectores() {
    visited = vector<bool>(n, false);
    padres = vector<int>(n, -1);
    backEdges = vector<vector<int>>(n, vector<int>());
    treeEdges = vector<vector<int>>(n, vector<int>());
    estado = vector<int>(n, NO_LO_VI);
    backEdgesConExtremoSUPERIOR = vector<int>(n, 0);
    backEdgesConExtremoINFERIOR = vector<int>(n, 0);
    memo = vector<int>(n, -1);
}


void DFS(int nodo, int padre = -1){
    padres[nodo] = padre;
    estado[nodo] = EMPECE_A_VER;
    for (int vecino : G[nodo]) {
        if (estado[vecino] == NO_LO_VI) {
            treeEdges[nodo].push_back(vecino);
            DFS(vecino, nodo);
        } else if (vecino != padre) {
            // Si mi vecino fue visitado y no es mi padre entonces es backEdge
            if (estado[vecino] == EMPECE_A_VER) {
                // Si aun no lo terminé de ver entonces es un ancestro
                // backEdges[vecino].push_back(nodo);
                backEdgesConExtremoINFERIOR[nodo]++;
            } else {
                // Si ya lo terminé de ver entonces es descendiente
                backEdges[nodo].push_back(vecino);
                backEdgesConExtremoSUPERIOR[nodo]++;
            }
        }
    }
    estado[nodo] = TERMINE_DE_VER;
}

int cubren(int nodo, int padre = -1) {
    if (memo[nodo] != -1) return memo[nodo];
    int res = 0;
    for (int hijo: treeEdges[nodo]) {
        if (hijo != padre) {
            res += cubren(hijo, nodo);
        }
    }
    res -= backEdgesConExtremoSUPERIOR[nodo];
    res += backEdgesConExtremoINFERIOR[nodo];

    return memo[nodo] = res;
}

bool esClaveBE(int v, int w) {
    // Sabemos que w es descendiente de v por como armamos backEdges
    bool res = false;
    int nodoActual = w;
    int ancestro = padres[w];
    while (nodoActual != v && !res) {
        if (cubren(nodoActual, ancestro) == 1) {
            res = true;
        }
        nodoActual = ancestro;
        ancestro = padres[ancestro];
    }
    return res;
}

void DFSSinArista(int nodo, int padre = -1, int w = -1, int z = -1){
    padres[nodo] = padre;
    estado[nodo] = EMPECE_A_VER;
    for (int vecino : G[nodo]) {

        // Si estoy usando la arista que quiero sacar entonces la salto
        if ((nodo == w && vecino == z) || (nodo == z && vecino == w)) {
            continue;
        }

        if (estado[vecino] == NO_LO_VI) {
            treeEdges[nodo].push_back(vecino);
            DFSSinArista(vecino, nodo, w, z);
        } else if (vecino != padre) {
            // Si mi vecino fue visitado y no es mi padre entonces es backEdge
            if (estado[vecino] == EMPECE_A_VER) {
                // Si aun no lo terminé de ver entonces es un ancestro
                //backEdges[vecino].push_back(nodo);
                backEdgesConExtremoINFERIOR[nodo]++;
            } else {
                // Si ya lo terminé de ver entonces es descendiente
                backEdges[nodo].push_back(vecino);
                backEdgesConExtremoSUPERIOR[nodo]++;
            }
        }
    }
    estado[nodo] = TERMINE_DE_VER;
}

int puentes_sin_arista(int w, int z) {
    // Inicializo Vectores
    inicializarVectores();

    // Hago DFS excluyendo mi arista (w, z)
    int componentes = 0;
    for (int i = 0; i < n; i++) {
        if (estado[i] == NO_LO_VI) {
            DFSSinArista(i, -1, w, z);
            componentes++;
        }
    }

    // Cuento la cantidad de puentes
    int puentes = 0;
    for (int i = 0; i < n; i++) {
        if (cubren(i) == 0) {
            puentes++;
        }
    }
    puentes -= componentes;
    return puentes;
}

int main() {
    int c;
    cin >> c;
    while (c > 0) {
        cin >> n >> m;
        // Armamos G
        G = vector<vector<int>>(n, vector<int>());
        inicializarVectores();
        for (int i = 0; i < m; ++i) {
            int v; int w;
            cin >> v; cin >> w;
            G[v].push_back(w);
            G[w].push_back(v);
        }

        // Calculamos el DFS Tree
        DFS(0);
        vector<vector<int>> BEDFSTREE = backEdges;
        vector<vector<int>> TEDFSTREE = treeEdges;


        int cantidadClaves = 0;
        vector<pair<int,int>> aristasClave;


        // Vemos si hay backedges clave
        for (int v = 0; v < n; ++v) {
            for (auto w : BEDFSTREE[v]) {
                if (esClaveBE(v, w)) {
                    cantidadClaves++;
                    if (v < w) {
                        aristasClave.push_back({v, w});
                    } else {
                        aristasClave.push_back({w, v});
                    }
                }
            }
        }
        // Vemos si hay treeedges clave
        // La sacamos y corremos algoritmo de puentes

        for (int v = 0; v < n; ++v) {
            for (auto w: TEDFSTREE[v]) {
                if (puentes_sin_arista(v, w) > 0) {
                    cantidadClaves++;
                    if (v < w) {
                        aristasClave.push_back({v, w});
                    } else {
                        aristasClave.push_back({w, v});
                    }

                }
            }
        }
        cout << cantidadClaves << endl;
        sort(aristasClave.begin(), aristasClave.end());
        for (auto arista : aristasClave) {
            cout << arista.first << " " << arista.second << endl;
        }

        c--;
    }
    return 0;
}