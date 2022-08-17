#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/random.h>

void handle_response() {
    int ret = alarm(0);
    char path [200] = "./to_client";
    pid_t pid = getpid();
    char pid_client[50];
    sprintf(pid_client, "%d", pid);
    strcat(path , pid_client);
    strcat(path , ".txt");
    int fd1 = open("./to_srv.txt", O_RDONLY | S_IRWXU | S_IXGRP);
    char first_input[100] = {};
    int i =0;
    int charsr1 = read(fd1, &first_input[i], 1);
    while (charsr1 == 1 && first_input[i] != ' ') {
        i++;
        charsr1 = read(fd1, &first_input[i], 1);
    }
    first_input[i] = 0;
    printf("%s\n", first_input);
    close(fd1);
    remove(path);
}

void alarm_hand(int sig) {
    printf("Client closed because no response was received from the server for 30 seconds\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("ERROR_FROM_EX4\n");
        exit(0);
    }
    // init signal.
    struct sigaction act;
    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigfillset(&block_mask);
    act.sa_mask = block_mask;
    act.sa_flags = 0;
    act.sa_handler = alarm_hand;
    sigaction(SIGALRM, &act, NULL);
    act.sa_handler = handle_response;
    sigaction(SIGUSR1, &act, NULL);
    //read arguments.
    char pid_server[50];
    strcpy(pid_server, argv[1]);
    char first_input[100];
    strcpy(first_input, argv[2]);
    char second_input[100];
    strcpy(second_input, argv[3]);
    char three_input[100];
    strcpy(three_input, argv[4]);
    // try to open
    char path[150] = "./to_srv.txt";
    int fd;
    for (int i = 0; i <= 10; i++) {
        if (i == 10) {
            printf("ERROR_FROM_EX4\n");
            exit(0);
        }
        fd = open(path, O_WRONLY | O_TRUNC | O_EXCL | O_CREAT, S_IRWXU | S_IXGRP);
        if (fd == -1) {
            unsigned int tmp = 5;
            getrandom(&tmp, sizeof(unsigned int), GRND_NONBLOCK);
            tmp = tmp % 6;
            sleep(tmp);
        } else {
            break;
        }
    }
    pid_t pid = getpid();
    char pid_client[150];
    sprintf(pid_client, "%d", pid);
    write(fd, pid_client, strlen(pid_client));
    write(fd, " ", strlen(" "));
    write(fd, first_input, strlen(first_input));
    write(fd, " ", strlen(" "));
    write(fd, second_input, strlen(second_input));
    write(fd, " ", strlen(" "));
    write(fd, three_input, strlen(three_input));
    pid_t server_pid;
    server_pid = atoi(pid_server);
    kill(server_pid, SIGUSR1);
    int ret = alarm(30);
    pause();
    return 0;
}
