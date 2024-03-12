#include <iostream>
#include <vector>

using namespace std;

int N;                                  // Cantidad de puestos
int K;                                  // Cantidad de proveedurias a colocar
vector<int> puestos;                    // Vector de puestos
vector<vector<vector<int>>> mem;        // Matriz 3d de memoización

int sumar_restantes(int d) {
    int dist_total = 0;
    for (int i = d+1; i<= N; i++) {
        dist_total += abs(puestos[i] - puestos[d]);
    }
    return dist_total;
}

// Función para saber que distancia sumar a los puestos ubicados entre 2 proveedurías.
int distancia_minima_entre(int a, int b) {
    int dist_total = 0;
    for (int i = b+1; i<a; i++) {
        int dist_a = abs(puestos[i] - puestos[a]);
        int dist_b = abs(puestos[i] - puestos[b]);
        dist_total += min(dist_a, dist_b);
    }
    return dist_total;
}

int seleccionar_puestos(int i, int j, int ult) {
    // i = tenemos en cuenta los puestos desde el primero hasta el i-ésimo
    // j = proveedurias restantes por colocar
    // ult = puesto donde colocamos la última proveeduria

    // Casos base
    if (i > N+1) {
        return INT32_MAX;
    }
    // Sumamos desde la últia proveeduria colocada hasta el final de la recta de puestos
    if (j == 0) {
        return sumar_restantes(ult);
    }

    // Tenemos más proveedurias por colocar que puestos
    if (N - i + 1 <= j) {
        return distancia_minima_entre(i, ult);
    }

    // Memoización
    if (mem[i][j][ult] != -1) {
        return mem[i][j][ult];
    }

    // Llamados recursivos
    int pongo = seleccionar_puestos(i+1, j-1, i) + distancia_minima_entre(i, ult);
    int no_pongo = seleccionar_puestos(i+1, j, ult);

    // Memoización
    return mem[i][j][ult] = min(pongo, no_pongo);
}

int main() {
	int c;
	cin >> c;
	while (c > 0) {
		cin >> N >> K;
        // Inicializamos estructuras de datos
		puestos = vector<int>(N+1, 0);
        mem = vector<vector<vector<int>>>(N+1, vector<vector<int>>(K+1,vector<int>(N+1, -1)));
        puestos[0] = INT32_MAX;
		for (int i = 1; i<=N; i++) {
			cin >> puestos[i];
		}

        //  Utilizamos el truco de poner un puesto 0 a distancia máxima para así "colocarle" una proveeduría encima
        //  y facilitarnos la tarea de llamar a la función por primera vez.
        //  Esto tambien nos facilita la primer llamada a distancia_minima_entre ya que vamos a solamente sumar la distancia
        //  de los puestos con respecto a la primer proveeduría colocada (la proveeduría 0, al estar a distancia infinita, siempre va a dar
        //  una distancia mayor).
        int min_dist = seleccionar_puestos(1, K, 0);

        cout<<min_dist<<endl;

        // Iniciamos la búsqueda de las proveedurias óptimas
        vector<int> proveedurias;
        int i = 1; int j = K; int u = 0;
        while (j > 0) {
            if (i >= N) { // Guarda anti-segmentation fault
                proveedurias.push_back(puestos[i]);
                break;
            }
            // Ya están memoizados
            int pongo = seleccionar_puestos(i+1, j-1, i) + distancia_minima_entre(i, u);
            int no_pongo = seleccionar_puestos(i+1, j, u);

            // <= Para quedarnos con las óptimas lexicográficamente
            if (pongo <= no_pongo) {
                u = i; i++; j--;
                proveedurias.push_back(puestos[u]);
            } else {
                i++;
            }
        }

        for (int i = 0; i<K; i++) {
			cout << proveedurias[i] << " ";
		}
        cout<<endl;

		c--;
	}
}