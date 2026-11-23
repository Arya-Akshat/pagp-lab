#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
using namespace std;

void lower(string &s) {
    for (char &c : s) c = tolower(c);
}

int getCount(string filename, string key) {
    ifstream in(filename);
    string word;
    int count = 0;
    while (in >> word) {
        lower(word);
        if (key == word) count++;
    }
    return count;
}

int main() {
    string words[] = {"lorem", "ipsum", "dolo", "sit", "amet"};
    int n = 5, counts[5] = {0};

    double t = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < n; i++)
        counts[i] = getCount("words.txt", words[i]);

    cout << "Time: " << omp_get_wtime() - t << endl;
    for (int i = 0; i < n; i++)
        cout << words[i] << ": " << counts[i] << endl;
}
