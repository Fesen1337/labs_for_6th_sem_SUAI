#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define PROCESS_NUM 3

int x = 0;
int id1 = -2, id2 = -2, id3 = -2;
void handle_sigusr1(int sig)
{
    x = 0;
    //kill(id3, SIGKILL);
    //kill(id2, SIGKILL);
}

void handle_sigusr2(int sig)
{
    
    kill(id3, SIGKILL);
    kill(getpid(), SIGKILL);
}

int main(int argc, char* argv[])
{
    int n = 10;
    if(argc == 2)
        n = atoi(argv[1]);

    struct sigaction sa = { 0 };
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &handle_sigusr1;
    sigaction(SIGUSR1, &sa, NULL);

    struct sigaction sa2 = { 0 };
    sa2.sa_flags = SA_RESTART;
    sa2.sa_handler = &handle_sigusr2;
    sigaction(SIGUSR2, &sa2, NULL);

        
    id1 = getpid();
    FILE *f = fopen("1.txt", "w+");

    id2 = fork();
    if(id2 == 0)
    {
        id3 = fork();
        if(id3 == 0)
        {
            x = 1;
            while(1)
            {
                sleep(1);
                if(x == 0)
                {
                    fseek(f, 0, SEEK_END);
                    fprintf(f, "3");
                    x = 1;
                    kill(getppid(), SIGUSR1);                        
                }
                
            }
        }else{
            while(1)
            {
                sleep(1);
                if(x == 0)
                {
                    fseek(f, 0, SEEK_END);
                    fprintf(f, "2");
                    x = 1;
                    kill(id3, SIGUSR1);    
                }
                
            }
        }
    }else{
        int size = 0;
        //sleep(5);
        FILE *q = fopen("1.txt", "r");
        while(size < n)
        {
            fseek(q, 0, SEEK_END); 
            size = ftell(q);
        }
        kill(id2, SIGUSR2);
        wait(NULL);
        fclose(q);
        fseek(f, 0, SEEK_SET);
        char buf[1024];
        fscanf(f, "%s", buf);
        printf("%s\n", buf);
        
        fclose(f);
        
    }

    return 0;
}
