#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: saver <result>\n");
        exit(1);
    }

    FILE *f = fopen("results.txt", "a");
    if (!f) {
        perror("fopen");
        exit(1);
    }
    fprintf(f, "%s\n", argv[1]);
    fclose(f);

    return 0;
}

