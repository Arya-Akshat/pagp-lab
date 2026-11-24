#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gd.h>
#include <omp.h>
int main(int argc, char *argv[]) {
    if (argc < 6) {
        printf("usage: %s in.png out.png sched chunk threads [mark]\n", argv[0]);
        return 1;
    }
    char *sched = argv[3];
    int chunk = atoi(argv[4]), nt = atoi(argv[5]);
    int mark = (argc > 6) ? atoi(argv[6]) : 0;
    FILE *fp = fopen(argv[1], "rb");
    if (!fp) { printf("cannot open %s\n", argv[1]); return 1; }
    gdImagePtr img = gdImageCreateFromPng(fp);
    fclose(fp);
    if (!img) { printf("not a valid PNG: %s\n", argv[1]); return 1; }
    gdImagePaletteToTrueColor(img);
    int w = gdImageSX(img), h = gdImageSY(img);
    if (!strcmp(sched, "static")) omp_set_schedule(omp_sched_static, chunk);
    else if (!strcmp(sched, "dynamic")) omp_set_schedule(omp_sched_dynamic, chunk);
    else if (!strcmp(sched, "guided")) omp_set_schedule(omp_sched_guided, chunk);
    else omp_set_schedule(omp_sched_static, 0);
    omp_set_num_threads(nt);
    double t = omp_get_wtime();
    #pragma omp parallel for schedule(runtime)
    for (int x = 0; x < w; x++) {
        int tid = omp_get_thread_num();
        for (int y = 0; y < h; y++) {
            int c = gdImageGetPixel(img, x, y);
            int g = (gdTrueColorGetRed(c) + gdTrueColorGetGreen(c) + gdTrueColorGetBlue(c)) / 3;
            int col;
            if (!mark) col = gdTrueColor(g, g, g);
            else if (tid % 4 == 0) col = gdTrueColor(g, 0, 0);
            else if (tid % 4 == 1) col = gdTrueColor(0, g, 0);
            else if (tid % 4 == 2) col = gdTrueColor(0, 0, g);
            else col = gdTrueColor(g, g, 0);
            gdImageSetPixel(img, x, y, col);
        }
    }
    t = omp_get_wtime() - t;
    printf("%dx%d schedule=%s chunk=%d threads=%d time = %f s\n", w, h, sched, chunk, nt, t);
    fp = fopen(argv[2], "wb");
    gdImagePng(img, fp);
    fclose(fp);
    gdImageDestroy(img);
    return 0;
}
