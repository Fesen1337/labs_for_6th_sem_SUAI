#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define PROCESS_NUM 3

int stop_global_variable = 0;
int id1 = -2, id2 = -2;

void myAlarm(int var)
{
    kill(id2, SIGUSR1);
}

void handle_sigusr1(int sig)
{
    stop_global_variable = 1;
}

int main(int argc, char* argv[])
{
    long int s_time;
    struct tm *m_time;
    char str[124];

    signal(SIGALRM, myAlarm);

    struct sigaction sa = { 0 };
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &handle_sigusr1;
    sigaction(SIGUSR1, &sa, NULL);
        
    id1 = getpid();
    FILE *f = fopen("1.txt", "w+");

    id2 = fork();
    if(id2 == 0)
    {
        s_time = time(NULL);
        m_time = localtime(&s_time);
        strftime(str, 128, "Date: %x %X", m_time);
        fprintf(f, "%s", str);
        fprintf(f, " program started\n");
        while(stop_global_variable == 0){}
        fclose(f);
        execl("w","program", "ended", NULL);
    }else{
        
        alarm(1);
        wait(NULL);
        
        f = fopen("1.txt", "r");
        char c;
        while (fscanf(f, "%c", &c) == 1)
        {
            fprintf(stdout, "%c", c);
        }
        
        fclose(f);
        
    }

    return 0;
}
