#include <iostream>
#include <vector>
#include <bits/stdc++.h>
using namespace std;

typedef long long LL;

int N;
vector<LL> M;
vector<LL> C;
vector<pair<long double, int>> razon_descontento;

#define MOD 1000000007

LL potMod(LL x, LL y, LL n) {
    if (y == 0) return 1;
    LL res = (potMod(x, y/2, n));
    res = (res * res) % n;
    if (y % 2 == 0) {
        return res;
    } else {
        return (res * x) % n;
    }
}

LL multMod(LL a, LL b, LL mod) {
    LL res = 0;
    a = a % mod;
    while (b > 0) {
        // Si b es impar, suma 'a' al res
        if (b % 2 == 1) res = (res + a) % mod;

        a = (a * 2) % mod;

        b /= 2;
    }
    return res % mod;
}

LL minimizar_descontento() {
	sort(razon_descontento.begin(), razon_descontento.end());
	
	LL tiempo = 0;
	LL descontento = 0;
	for (int i = 0; i<N; i++) {
		int estudiante = razon_descontento[i].second;
		tiempo += M[estudiante];
		descontento = (descontento + multMod(tiempo, C[estudiante], MOD)) % MOD; 
	}
	
	return descontento;
}

int main() {
	
	int c;
	cin >> c;
	while (c > 0) {
		cin >> N;
		M = vector<LL>(N, 0);
		C = vector<LL>(N, 0);
		razon_descontento = vector<pair<long double, int>>(N, make_pair(0.0,0));
		for (int i = 0; i<N; i++) {
			cin>>M[i];
		}
		for (int i = 0; i<N; i++) {
			cin>>C[i];
			razon_descontento[i] = make_pair( (long double) M[i]/C[i], i);
		}
		
		LL min_descontento = minimizar_descontento();
		cout << min_descontento << endl;
		
		c--;
	}
}
