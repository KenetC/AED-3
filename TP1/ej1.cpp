#include <iostream>
#include <vector>
#include <string>
#include <climits>

using namespace std;

vector<vector<char>> L;
vector<vector<bool>> visitados;
int N, M;
int minimo, maximo, hashtags;

int maximoPosible, minimoPosible;

enum orientacion {VERTICAL, HORIZONTAL, AMBAS};
vector<pair<int, int>> movimientos = {{1, 0}, {0, 1}, {0, -1}, {-1, 0}};

bool esMovValido(pair<int, int> pos, pair<int, int> next, int orActual) {
    if (next.first >= 0 && next.first < N && next.second >= 0 && next.second < M) {
        if (visitados[next.first][next.second]) return false; // Ya pasé por ahí
        if (L[next.first][next.second] == '#') return false; // Celda vacía

        // Verificar la orientación actual y la dirección del movimiento
        if (orActual == HORIZONTAL && next.first != pos.first) return false; // No puedes moverte verticalmente
        if (orActual == VERTICAL && next.second != pos.second) return false; // No puedes moverte horizontalmente

        return true;
    }
    return false;
}

bool movHorizontal(pair<int, int> posActual, pair<int, int> next) {
    return (next.second == posActual.second + 1 || next.second == posActual.second - 1);
}

bool movVertical(pair<int, int> posActual, pair<int, int> next) {
    return (next.first == posActual.first + 1   || next.first == posActual.first - 1);
}

//bool esMovPosible(pair<int, int> posActual, pair<int, int> next, orientacion orActual) {
//    switch(orActual) {
//        case AMBAS:
//            return true;
//        case HORIZONTAL:
//            return movHorizontal(posActual, next);
//        case VERTICAL:
//            return movVertical(posActual, next);
//    }
//}

orientacion nuevaOrientacion(pair<int, int> posActual, pair<int, int> next, orientacion orPrevia) {
    switch (L[next.first][next.second]) {
        case '+':
            return AMBAS;
        case 'L':
            if (orPrevia == VERTICAL) return HORIZONTAL;
            if (orPrevia == HORIZONTAL) return VERTICAL;
            if (movVertical(posActual, next)) return HORIZONTAL;
            if (movHorizontal(posActual, next)) return VERTICAL;
        case 'I':
            if (orPrevia != AMBAS) return orPrevia;
            if (movVertical(posActual, next)) return VERTICAL;
            if (movHorizontal(posActual, next)) return HORIZONTAL;
    }
}



pair<int, int> sumaDupla(pair<int,int> d1, pair<int,int> d2) {
    return {d1.first + d2.first, d1.second + d2.second};
}


void calcularCaminos(pair<int, int> posActual, int caminado, orientacion orActual, int& minimo, int& maximo) {
    if (posActual.first == N - 1 && posActual.second == M - 1) {
        if (caminado < minimo) minimo = caminado;
        if (caminado > maximo) maximo = caminado;
        return;
    }

    for (auto movimiento : movimientos) {
        pair<int, int> next = sumaDupla(posActual, movimiento);
        if (esMovValido(posActual, next, orActual)) {
            visitados[next.first][next.second] = true;
            orientacion orNext = nuevaOrientacion(posActual, next, orActual);
            calcularCaminos(next, caminado + 1, orNext, minimo, maximo);
            visitados[next.first][next.second] = false;
        }
    }
}

int main() {
    int c;
    cin >> c;
    while (c--) {
        cin >> N >> M;
        visitados = vector<vector<bool>>(N, vector<bool>(M, false));
        L = vector<vector<char>>(N, vector<char>(M, '#'));
        int hashtags = 0;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                cin >> L[i][j];
                if (L[i][j] == '#') hashtags++;
            }
        }

        int minimoPosible = N + M - 2;
        int maximoPosible = (N * M) - 1 - hashtags;
        if (maximoPosible < minimoPosible) {
            cout << "IMPOSIBLE" << endl;
            continue;
        }

        visitados[0][0] = true;
        int minimo = INT_MAX;
        int maximo = INT_MIN;

        calcularCaminos({0, 0}, 0, AMBAS, minimo, maximo);

        if (minimo <= maximoPosible && maximo >= minimoPosible) {
            cout << "POSIBLE " << minimo << " " << maximo << endl;
        } else {
            cout << "IMPOSIBLE" << endl;
        }
    }
    return 0;
}