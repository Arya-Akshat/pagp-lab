#include <iostream>
#include <cstdio>
#include <gd.h>
#include <omp.h>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <file.png>\n";
        return 1;
    }

    FILE *fp = fopen(argv[1], "wb");
    int w = 1000, h = 1000;
    gdImagePtr img = gdImageCreateTrueColor(w, h);
    double t = omp_get_wtime();

    #pragma omp parallel for num_threads(4) schedule(static)
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            int color = 0xFF << (omp_get_thread_num() * 8);
            #pragma omp critical
            gdImageSetPixel(img, x, y, color);
        }
    }

    gdImagePng(img, fp);
    gdImageDestroy(img);
    fclose(fp);

    cout << "Time: " << omp_get_wtime() - t << endl;
}
