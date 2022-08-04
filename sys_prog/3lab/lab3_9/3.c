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

    FILE *f = fopen(filename, "w+");

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
        int i;
        for(i = 0; i < n; i++)
        {
            read(fd[0], str, 100);
            char c;
            fseek(f, 0, SEEK_END);
            fseek(f, (-1)*sizeof(char), SEEK_CUR);
            fscanf(f, "%c", &c);
            fseek(f, 0, SEEK_END);
            printf("%c", c);
            kill(getppid(), SIGUSR1);
        }
        printf("\n");
        fclose(f);
        close(fd[0]);
    }else{
        char str[100];
        int i;
        for(i = 0 ; i < n; i++)
        {
            char c = 'b';
            fseek(f, 0, SEEK_END);
            fprintf(f, "%c", c);   
        
            strcpy(str, "send");
            write(fd[1], str, 100);
            
            while(x == 0){}
            x = 0;
        }
        close(fd[1]);
    }
    return 0;
}
