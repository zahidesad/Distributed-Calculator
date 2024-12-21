#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>


int main() {
    int pipe_add_in[2], pipe_add_out[2];
    int pipe_sub_in[2], pipe_sub_out[2];
    int pipe_mul_in[2], pipe_mul_out[2];
    int pipe_div_in[2], pipe_div_out[2];

    // Her işlem için iki pipe oluşturuyoruz.
    if (pipe(pipe_add_in) == -1 || pipe(pipe_add_out) == -1 ||
        pipe(pipe_sub_in) == -1 || pipe(pipe_sub_out) == -1 ||
        pipe(pipe_mul_in) == -1 || pipe(pipe_mul_out) == -1 ||
        pipe(pipe_div_in) == -1 || pipe(pipe_div_out) == -1) {
        perror("pipe");
        exit(1);
    }

    // addition child sürecini yarat
    pid_t pid_add = fork();
    if (pid_add == -1) {
        perror("fork");
        exit(1);
    }
    if (pid_add == 0) {
        // Child addition sürecindeyiz
        close(pipe_add_in[1]);  
        close(pipe_add_out[0]);

        char in_fd_str[10];
        char out_fd_str[10];
        sprintf(in_fd_str, "%d", pipe_add_in[0]);
        sprintf(out_fd_str, "%d", pipe_add_out[1]);

        execl("./addition", "addition", in_fd_str, out_fd_str, (char*)NULL);
        perror("execl addition");
        exit(1);
    }
    // Parent addition için kullanmayacaği uclari kapat
    close(pipe_add_in[0]);  
    close(pipe_add_out[1]);

    // subtraction child süreci
    pid_t pid_sub = fork();
    if (pid_sub == -1) {
        perror("fork");
        exit(1);
    }
    if (pid_sub == 0) {
        close(pipe_sub_in[1]);
        close(pipe_sub_out[0]);

        char in_fd_str[10];
        char out_fd_str[10];
        sprintf(in_fd_str, "%d", pipe_sub_in[0]);
        sprintf(out_fd_str, "%d", pipe_sub_out[1]);

        execl("./subtraction", "subtraction", in_fd_str, out_fd_str, (char*)NULL);
        perror("execl subtraction");
        exit(1);
    }
    close(pipe_sub_in[0]);
    close(pipe_sub_out[1]);

    // multiplication child süreci
    pid_t pid_mul = fork();
    if (pid_mul == -1) {
        perror("fork");
        exit(1);
    }
    if (pid_mul == 0) {
        close(pipe_mul_in[1]);
        close(pipe_mul_out[0]);

        char in_fd_str[10];
        char out_fd_str[10];
        sprintf(in_fd_str, "%d", pipe_mul_in[0]);
        sprintf(out_fd_str, "%d", pipe_mul_out[1]);

        execl("./multiplication", "multiplication", in_fd_str, out_fd_str, (char*)NULL);
        perror("execl multiplication");
        exit(1);
    }
    close(pipe_mul_in[0]);
    close(pipe_mul_out[1]);

    // division child süreci
    pid_t pid_div = fork();
    if (pid_div == -1) {
        perror("fork");
        exit(1);
    }
    if (pid_div == 0) {
        close(pipe_div_in[1]);
        close(pipe_div_out[0]);

        char in_fd_str[10];
        char out_fd_str[10];
        sprintf(in_fd_str, "%d", pipe_div_in[0]);
        sprintf(out_fd_str, "%d", pipe_div_out[1]);

        execl("./division", "division", in_fd_str, out_fd_str, (char*)NULL);
        perror("execl division");
        exit(1);
    }
    close(pipe_div_in[0]);
    close(pipe_div_out[1]);


    int choice;
    while (1) {
        printf("Calculator:\n");
        printf("1- addition\n");
        printf("2- subtraction\n");
        printf("3- multiplication\n");
        printf("4- division\n");
        printf("5- exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        if (choice == 5) {
            break;
        }

        double a, b;
        printf("Enter two operands: ");
        if (scanf("%lf%lf", &a, &b) != 2) {
            fprintf(stderr, "Invalid input\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }

        // Child'a gönderecek veriyi stringe çevir
        char buffer[100];
        sprintf(buffer, "%lf %lf", a, b);

        int in_fd = -1, out_fd = -1;

        if (choice == 1) {
            in_fd = pipe_add_in[1];
            out_fd = pipe_add_out[0];
        } else if (choice == 2) {
            in_fd = pipe_sub_in[1];
            out_fd = pipe_sub_out[0];
        } else if (choice == 3) {
            in_fd = pipe_mul_in[1];
            out_fd = pipe_mul_out[0];
        } else if (choice == 4) {
            in_fd = pipe_div_in[1];
            out_fd = pipe_div_out[0];
        } else {
            printf("Invalid choice\n");
            continue;
        }

        // Veriyi child'a yaz
        if (write(in_fd, buffer, strlen(buffer)+1) == -1) {
            perror("write to child");
            continue;
        }

        // Child'tan sonucu oku
        char result_buf[100];
        memset(result_buf, 0, sizeof(result_buf));
        if (read(out_fd, result_buf, sizeof(result_buf)) == -1) {
            perror("read from child");
            continue;
        }

        printf("Result: %s\n", result_buf);
    }

    // exit seçilince çocuklari oldur
    kill(pid_add, SIGTERM);
    kill(pid_sub, SIGTERM);
    kill(pid_mul, SIGTERM);
    kill(pid_div, SIGTERM);

    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}

