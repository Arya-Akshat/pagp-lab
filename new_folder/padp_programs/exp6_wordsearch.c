#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <omp.h>
#define COUNT 10
char *keys[COUNT] = {"The","around","graphics","from","by","be","any","which","various","mount"};
long count_word(const char *file, const char *key) {
    FILE *fp = fopen(file, "r");
    if (!fp) { printf("cannot open %s\n", file); exit(1); }
    char word[64];
    long cnt = 0;
    int ch, len = 0;
    while ((ch = getc_unlocked(fp)) != EOF) {
        if (isalpha(ch)) { if (len < 63) word[len++] = ch; }
        else if (len) { word[len] = '\0'; if (!strcasecmp(word, key)) cnt++; len = 0; }
    }
    if (len) { word[len] = '\0'; if (!strcasecmp(word, key)) cnt++; }
    fclose(fp);
    return cnt;
}
int main(int argc, char *argv[]) {
    char *file = (argc > 1) ? argv[1] : "words.txt";
    int nt = (argc > 2) ? atoi(argv[2]) : 4;
    long counts[COUNT];
    omp_set_num_threads(nt);
    double t = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < COUNT; i++)
        counts[i] = count_word(file, keys[i]);
    t = omp_get_wtime() - t;
    for (int i = 0; i < COUNT; i++) printf("%-10s : %ld\n", keys[i], counts[i]);
    printf("Threads = %d Time taken = %f s\n", nt, t);
    return 0;
}
