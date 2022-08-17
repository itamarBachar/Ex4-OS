// Itamar Bachar 318781630
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <wait.h>
#include <string.h>


// printf(ERROR_FROM_EX4\n
void alarm_hand(int sig) {
    printf("The server closed because no service request was received for the last 60 seconds\n");
    exit(0);
}

void func_handle_clint(int sig) {
    int ret = alarm(60);
    int stat, waited;
    pid_t pid;
//    pid = fork();
//    if (pid == -1) {
//        perror("fork failed");
//    }
//    if (pid == 0) {
        int fd1 = open("./to_srv.txt", O_RDONLY | S_IRWXU | S_IXGRP);
        int flag = 0, i = 0;
        char pid_child[100] = {};
        char first_input[100] = {};
        char second_input[100] = {};
        char three_input[100] = {};
        int charsr1 = read(fd1, &pid_child[i], 1);
        while (charsr1 == 1 && pid_child[i] != ' ') {
            i++;
            charsr1 = read(fd1, &pid_child[i], 1);
        }
        pid_child[i] = 0;
        i = 0;
        charsr1 = read(fd1, &first_input[i], 1);
        while (charsr1 == 1 && first_input[i] != ' ') {
            i++;
            charsr1 = read(fd1, &first_input[i], 1);
        }
        first_input[i] = 0;
        i = 0;
        charsr1 = read(fd1, &second_input[i], 1);
        while (charsr1 == 1 && second_input[i] != ' ') {
            i++;
            charsr1 = read(fd1, &second_input[i], 1);
        }
        second_input[i] = 0;
        i = 0;
        charsr1 = read(fd1, &three_input[i], 1);
        while (charsr1 == 1 && three_input[i] != ' ') {
            i++;
            charsr1 = read(fd1, &three_input[i], 1);
        }
        three_input[i] = 0;
        remove("./to_srv.txt");
        char path[200]= "./to_client_";
        strcat(path, pid_child);
        strcat(path, ".txt");
        int result;
        int first = atoi(first_input);
        int second = atoi(second_input);
        int three = atoi(three_input);
        if (second == 1) {
            result = first + three;
        } else if (second == 2) {
            result = first - three;
        } else if (second == 3) {
            result = first * three;
        } else if (second == 4) {
            result = first / 4;
        }
        int fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU | S_IXGRP);
        char result_fo_file [100];
        sprintf(result_fo_file , "%d" , result );
        write(fd , result_fo_file, strlen(result_fo_file));
        close(fd);
        int pid_client = 0;
        sprintf(pid_child , "%d" , pid_client );
        kill(pid_client, SIGUSR1);
//    } else {
//        waited = wait(&stat);
//    }
}


int main() {
    remove("./to_srv.txt");
    struct sigaction act;
    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigfillset(&block_mask);
    act.sa_mask = block_mask;
    act.sa_flags = 0;
    act.sa_handler = alarm_hand;
    sigaction(SIGALRM, &act, NULL);
    act.sa_handler = func_handle_clint;
    sigaction(SIGUSR1, &act, NULL);
    unsigned int ret = alarm(60);
    pause();
    return 0;
}

