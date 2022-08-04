#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int x = 0;
int id1 = -2, id2 = -2;
void handle_sigusr1(int sig) 
{
    x++;
}


int main(int argc, char* argv[])
{
    int n = 10;
    if(argc == 2)
        n = atoi(argv[1]);
    char filename[10];
    strcpy(filename, "1.txt");

    FILE *f = fopen(filename, "w");

    struct sigaction sa = { 0 };
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &handle_sigusr1;
    sigaction(SIGUSR1, &sa, NULL);
    id1 = getpid();

    int fd[2];
    //fd[0] - read
    //fd[1] - write
    if(pipe(fd) == -1)
    {
        printf("An error ocurre with opening the pipe\n");
        return 1;
    }

    id2 = fork();
    if(id2 == 0)//обработка второго процесса
    {
        char str[100];
        read(fd[0], str, 100);
        close(fd[0]);
        printf("%s\n", str);
        kill(getppid(), SIGUSR1);
    }else{
        char str[100];
        int i;
        for(i =0 ; i < n; i++)
        {
            fprintf(f, "a");
            str[i] = 'a';   
        }
        str[i] = '\0';
        fclose(f);
        write(fd[1], str, 100);
        close(fd[1]);
        while(x == 0){}
    }
    return 0;
}