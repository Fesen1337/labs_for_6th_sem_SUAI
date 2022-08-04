#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct mArgs
{
    char* arg;
    int thread_num;
};

struct logMes
{
    int thread_nums;
    char mes[1024];
    long int s_time;
    struct tm *m_time;
};

pthread_mutex_t mutex_long;
pthread_mutex_t mutex_short;
pthread_mutex_t mutex_log;

void* routine(void* args);

void write_log(void* log);

void write_links(char* a);

int is_link(char* str);

int main(int argc, char* argv[])
{
    int N = argc - 1;

    
    pthread_mutex_init(&mutex_short, NULL);
    pthread_mutex_init(&mutex_long, NULL);
    pthread_mutex_init(&mutex_log, NULL);

    
    FILE* clear;
    clear = fopen("links.txt", "w");
    fclose(clear);

    
    struct mArgs margs[20];   
    int i;

    pthread_t t[20];

    for(i = 0; i < N; i++)
    {
        margs[i].arg = argv[i+1];
        margs[i].thread_num = i + 1;
        pthread_create(&t[i], NULL, &routine, &margs[i]);
    }

    for(i = 0; i < N; i++)
    {
        pthread_join(t[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex_log);
    pthread_mutex_destroy(&mutex_short);
    pthread_mutex_destroy(&mutex_long);
    
    
}


void write_log(void* log_mes)
{
    pthread_mutex_lock(&mutex_log);
    
    struct logMes *logm = (struct logMes*)log_mes;
    
    FILE* log;
    log = fopen("log.log", "a");

    char str[128];
    char buf[1024];
    
    strftime(str, 128, "Date: %x %X", logm->m_time);
    snprintf(buf, 2048, "%s: Thread #%d: %s \n", str, logm->thread_nums, logm->mes);

    fprintf(log, "%s", buf);
    fclose(log);

    pthread_mutex_unlock(&mutex_log);
}



void* routine(void* args)
{
    struct mArgs *margs = (struct mArgs*)args;
    struct logMes l;
    l.thread_nums =  margs->thread_num;
    l.s_time = time(NULL);
    l.m_time = localtime(&l.s_time);
    snprintf(l.mes, 100, "Try to read file: %s", margs->arg);
    write_log(&l);

    FILE* f;
    char buffer[100];
    char q;
    f = fopen(margs->arg, "r");
    if(f == NULL)
    {
        l.s_time = time(NULL);
        l.m_time = localtime(&l.s_time);
        snprintf(l.mes, 100, "Can't open file: %s", margs->arg);
        write_log(&l);
        printf("Can't open file: %s\n", margs->arg);
    }else{
        l.s_time = time(NULL);
        l.m_time = localtime(&l.s_time);
        snprintf(l.mes, 100, "Opened file: %s", margs->arg);
        write_log(&l);
        fscanf(f, "%s", buffer);
        while(!feof(f))
        {
            if(is_link(buffer) == 1)
            {
                l.s_time = time(NULL);
                l.m_time = localtime(&l.s_time);
                snprintf(l.mes, 200, "Was find link -> |%s| in file: %s", buffer, margs->arg);
                write_log(&l);
                write_links(buffer);
                printf("%s\n", buffer);
            }
            fscanf(f, "%s", buffer);
            
        }
        fclose(f);
        l.s_time = time(NULL);
        l.m_time = localtime(&l.s_time);
        snprintf(l.mes, 100, "Closed file: %s", margs->arg);
        write_log(&l);
    }
    
    
}

int is_link(char *str)
{
    if(strlen(str) <= 7)
        return 0;
    if(str[0] != 'h')
        return 0;
    if(str[1] != 't')
        return 0;
    if(str[2] != 't')
        return 0;
    if(str[3] != 'p')
        return 0;
    if(str[4] != 's')
        return 0;
    if(str[5] != ':')
        return 0;
    if(str[6] != '/')
        return 0;
    if(str[7] != '/')
        return 0;
    return 1;
}


void write_links(char* a)
{
    pthread_mutex_lock(&mutex_long);
    FILE* f;
    f = fopen("links.txt", "a");
    fprintf(f, "%s\n", a);
    fclose(f);
    pthread_mutex_unlock(&mutex_long);
}
