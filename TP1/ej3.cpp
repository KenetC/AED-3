#include <iostream>
#include <vector>
using namespace std;

int N;
int W;
vector<vector<int>> MEM;
vector<string> sols;
vector<int> entradas;

bool descubri_signos (int sp, int ei, string cad, int sumarest) {
    
    if (ei >= N) {
        if (sp == W) sols.push_back(cad);
        return (sp == W);
    }

    if (MEM[sp+10000][ei] != -1) {
        if (MEM[sp+10000][ei] == 1) {
            sols.push_back(cad+'X');
        }
        return (MEM[sp+10000][ei] == 1);
    }
    
    bool res = false;
    if (!(sp+sumarest < W || sp-sumarest > W)) {
        bool pos = descubri_signos(sp + entradas[ei], ei+1, cad+'+', sumarest-entradas[ei]);
        bool neg = descubri_signos(sp - entradas[ei], ei+1, cad+'-', sumarest-entradas[ei]);
        res = (pos || neg);
    }
    
    return MEM[sp+10000][ei] = res;
}

int main() {

    int c;
    cin >> c;
    while (c > 0) {
        cin >> N >> W;
        W = W/100;
        MEM = vector<vector<int>>(20001, vector<int>(101, -1));
        entradas = vector<int>(N, 0);
        sols = vector<string>();
        int sumarest=0;
        for (int i = 0; i<N; i++) {
            int temp;
            cin>>temp;
            entradas[i] = temp/100;
            sumarest+=entradas[i];
        }

        descubri_signos(0, 0, "", sumarest);
        string sol_f = sols[0];
        for (auto s : sols) {
            //cout<<s<<endl;
            for (int i = 0; i < N; i++) {
                if (s[i] == 'X') break;
                if (s[i] != sol_f[i]) sol_f[i] = '?';
            }
        }
        cout<<sol_f<<endl;
        c--;
    }
}

//1
//10 100000
//10000 10000 10000 10000 10000 10000 10000 10000 10000 10000

//1
//4 400
//500 700 700 100