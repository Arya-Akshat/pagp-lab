#include <iostream>
#include <cstdio>
#include <gd.h>
#include <omp.h>
using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <in.png> <out.png>\n";
        return 1;
    }

    FILE *ifp = fopen(argv[1], "rb"), *ofp = fopen(argv[2], "wb");
    double t = omp_get_wtime();

    gdImagePtr img = gdImageCreateFromPng(ifp);
    int w = gdImageSX(img), h = gdImageSY(img);

    #pragma omp parallel for schedule(dynamic)
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            int c = gdImageGetPixel(img, x, y);
            int avg = (gdImageRed(img, c) + gdImageGreen(img, c) + gdImageBlue(img, c)) / 3;
            c = gdImageColorAllocate(img, avg, avg, avg);
            #pragma omp critical
            gdImageSetPixel(img, x, y, c);
        }
    }

    gdImagePng(img, ofp);
    gdImageDestroy(img);
    fclose(ifp);
    fclose(ofp);

    cout << "Time: " << omp_get_wtime() - t << endl;
}
