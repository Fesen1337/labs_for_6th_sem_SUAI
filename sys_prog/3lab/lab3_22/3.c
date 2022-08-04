#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int x = 0;
int id1 = -2, id2 = -2, id3 = -2;
void handle_sigusr1(int sig)
{
    x = 1;
}

int main(int argc, char* argv[])
{
    int m = 5;
    if(argc == 2)
        m = atoi(argv[1]);
    char filename[10];
    strcpy(filename, "1.txt");

    struct sigaction sa = { 0 };
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &handle_sigusr1;
    sigaction(SIGUSR1, &sa, NULL);

    id1 = getpid();

    id2 = fork();
    if(id2 == 0)//обработка второго процесса
    {
        FILE *f = fopen(filename, "r");
        char str[1024];
        FILE *buf;
        buf = fopen("buf.txt", "w");
        int id = 1;
        while(fgets(str, 1024, f) != NULL)
        {
            fprintf(buf, "%d)%s",id, str);
            id++;
        }
        fclose(f);
        fclose(buf);
        f = fopen(filename, "w");
        buf = fopen("buf.txt", "r");

        while(fgets(str, 1024, buf) != NULL)
        {
            fprintf(f, "%s",str);
        }
        fclose(buf);
        fclose(f);
        remove("buf.txt");
        kill(getppid(), SIGUSR1);
    }else{
        id3 = fork();
        if(id3 == 0)//обработка 3го процесса
        {
            while(x == 0){}
            FILE *f = fopen(filename, "r");
            char str[1024];
            FILE *buf = fopen("buf.txt", "w");
            int id = 0;
            while(fgets(str, 1024, f) != NULL)
            {
                fprintf(buf, "%s", str);
                id++;
            }
            fclose(f);
            fclose(buf);

            f = fopen(filename, "w");
            buf = fopen("buf.txt", "r");

            int res = id - m;
            for(int i = 0; i < res; i++)
            {
                fgets(str, 1024, buf);
                fprintf(f, "%s",str);
            }
            fclose(f);
            fclose(buf);
            remove("buf.txt");
            kill(getppid(), SIGUSR1);
        }else{//обработка главного процесса (1го)
            char str[1024];
            while(x == 0){}
            kill(id3, SIGUSR1);
            x = 0;
            usleep(2000);
            while(x == 0){}
            FILE *f = fopen(filename, "r");
            while(fgets(str, 1024, f) != NULL)
            {
                printf("%s", str);
            }
            fclose(f);
        }
    }
    return 0;
}
