#include <iostream>
#include <cmath>
#include <omp.h>
using namespace std;

int strike(bool comp[], int start, int step, int stop) {
    int i;
    for (i = start; i <= stop; i += step) comp[i] = true;
    return i;
}

int unfriendlySieve(int n) {
    int m = sqrt(n), count = 0;
    bool* comp = new bool[n + 1]();
    double t = omp_get_wtime();

    for (int i = 2; i <= m; i++)
        if (!comp[i]) { count++; strike(comp, 2 * i, i, n); }
    for (int i = m + 1; i <= n; i++)
        if (!comp[i]) count++;

    cout << "Time: " << omp_get_wtime() - t << endl;
    delete[] comp;
    return count;
}

int friendlySieve(int n) {
    int m = sqrt(n), count = 0, fac_cnt = 0;
    bool* comp = new bool[n + 1]();
    int *striker = new int[m], *factor = new int[m];
    double t = omp_get_wtime();

    for (int i = 2; i <= m; i++)
        if (!comp[i]) {
            count++;
            factor[fac_cnt] = i;
            striker[fac_cnt++] = strike(comp, 2 * i, i, m);
        }

    for (int left = m + 1; left <= n; left += m) {
        int right = min(left + m - 1, n);
        for (int k = 0; k < fac_cnt; k++)
            striker[k] = strike(comp, striker[k], factor[k], right);
        for (int i = left; i <= right; i++)
            if (!comp[i]) count++;
    }

    cout << "Time: " << omp_get_wtime() - t << endl;
    delete[] striker; delete[] factor; delete[] comp;
    return count;
}

int parallelySieve(int n) {
    int m = sqrt(n), count = 0, fac_cnt = 0;
    bool* comp = new bool[n + 1]();
    int *striker = new int[m], *factor = new int[m];
    double t = omp_get_wtime();

    for (int i = 2; i <= m; i++)
        if (!comp[i]) {
            count++;
            factor[fac_cnt] = i;
            striker[fac_cnt++] = strike(comp, 2 * i, i, m);
        }

    for (int left = m + 1; left <= n; left += m) {
        int right = min(left + m - 1, n);
        #pragma omp parallel for
        for (int k = 0; k < fac_cnt; k++)
            striker[k] = strike(comp, striker[k], factor[k], right);
        #pragma omp parallel for reduction(+:count)
        for (int i = left; i <= right; i++)
            if (!comp[i]) count++;
    }

    cout << "Time: " << omp_get_wtime() - t << endl;
    delete[] striker; delete[] factor; delete[] comp;
    return count;
}

int main() {
    int n = 100000000;
    cout << "Unfriendly: " << unfriendlySieve(n) << endl;
    cout << "Friendly:   " << friendlySieve(n) << endl;
    cout << "Parallel:   " << parallelySieve(n) << endl;
}
