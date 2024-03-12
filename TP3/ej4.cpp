#include <iostream>
#include <vector>
#include <cstdint>
#include <queue>

using namespace std;

#define INF INT32_MAX

int c, N, M, x;

vector<vector<int>> original_capacity;
vector<vector<int>> capacity;
vector<vector<int>> adj;

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

int maxflow(int s, int t) {
    int flow = 0;
    vector<int> parent(N);
    int new_flow;

    while (new_flow = bfs(s, t, parent)) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow;  // Actualizar la matriz de flujo
            capacity[cur][prev] += new_flow;  // Actualizar el flujo en sentido contrario
            cur = prev;
        }
    }
    return flow;
}

void actualizarCapacidadades(int k) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            capacity[i][j] /= k;
        }
    }
}

int main() {
    cin >> c;
    while (c > 0) {
        cin >> N >> M >> x;

        original_capacity = vector<vector<int>>(N, vector<int>(N, 0));
        adj = vector<vector<int>>(N, vector<int>());

        for (int i = 0; i<M; i++) {
            int v,w,c;
            cin >> v >> w >> c;
            original_capacity[v-1][w-1] = c;
            adj[v-1].push_back(w-1);
            adj[w-1].push_back(v-1);
        }

        int s = 0;
        int t = N-1;

        capacity = original_capacity;
        int cotaMaxima = maxflow(s, t);

        if (cotaMaxima < x) {
            cout << 0 << endl;
        } else if (cotaMaxima == x) {
            cout << 1 << endl;
        } else {
            int r = (cotaMaxima / x) + 1;
            int l = 0;
            int k;
            int last_correct = 0;
            while (l + 1 < r) {
                k = (l + r) / 2;


                // redefinir aristas
                capacity = original_capacity;
                actualizarCapacidadades(k);

                int cant_personas_que_llevan_k = maxflow(s, t);

                if (cant_personas_que_llevan_k >= x) {
                    l = k;
                    last_correct = k;
                } else if (cant_personas_que_llevan_k < x) {
                    r = k;
                }

            }
            cout << last_correct*x << endl;

        }

        c--;
    }
    return 0;
}
