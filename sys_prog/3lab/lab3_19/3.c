#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int x = 0;
int flag = 0;
int strs = 0;
int id1 = -2, id2 = -2, id3 = -2;
void handle_sigusr1(int sig) 
{
    if(id2 > 0 && id3 > 0)
    {
        flag++;
        flag = flag%2;
        if(flag == 0)
        {
            kill(id3, SIGUSR1);
        }else{
            strs++;
            kill(id2, SIGUSR1);
        }
    }else{
        x++;
    }
    
}

int main(int argc, char* argv[])
{
    int n = 10;
    if(argc == 2)
        n = atoi(argv[1]);
    char filename[10];
    strcpy(filename, "1.txt");
    
    FILE *f = fopen(filename, "w");
    fclose(f);

    struct sigaction sa = { 0 };
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &handle_sigusr1;
    sigaction(SIGUSR1, &sa, NULL);

    id1 = getpid();

    id2 = fork();
    if(id2 == 0)
    {
        while(1)
        {
            while(x == 0){}
            x = 0;
            f = fopen(filename, "a");
            fprintf(f, "hello world\n");
            fclose(f);
            kill(id1, SIGUSR1);
        }
        
    }else{
        id3 = fork();
        if(id3 == 0)//обработка 3го процесса
        {
            x = 1;
            int id = 0;
            while(id < n)
            {
                
                while(x == 0){}
                x = 0;
                
                id++;
                
                f = fopen(filename, "a");
                fprintf(f,"%3d) ", id);
                fclose(f);
                
                kill(id1, SIGUSR1);
            }
            
        }else{//обработка главного процесса (1го)
            wait(NULL);       
            kill(id2, SIGKILL);
            f = fopen(filename, "r");
            char str[100];
            while(fgets(str, 100, f) != NULL)
            {
                printf("%s", str);
            }
        }
    }
    return 0;
}
