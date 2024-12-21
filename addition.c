#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, char* argv[]) {

    int in_fd = atoi(argv[1]);
    int out_fd = atoi(argv[2]);

    char buffer[100];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = read(in_fd, buffer, sizeof(buffer));
        if (n <= 0) {
            exit(0);
        }

        double a, b;
        if (sscanf(buffer, "%lf %lf", &a, &b) != 2) {
            continue;
        }

        double sum = a + b;

        // Sonucu stringe çevir
        char result[50];
        sprintf(result, "%f", sum);

        

        // saver'i çagirarak dosyaya yaz
        pid_t pid = fork();
        if (pid == 0) {
            // child: saver'a exec
            execl("./saver", "saver", result, (char*)NULL);
            exit(1);
        }
        wait(NULL);

        // Sonucu ana programa geri gönder
        write(out_fd, result, strlen(result)+1);
    }

    return 0;
}

