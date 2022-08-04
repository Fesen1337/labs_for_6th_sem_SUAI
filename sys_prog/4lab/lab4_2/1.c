#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

pthread_mutex_t mutex_log;

struct mArgs
{
    int thread_num;
};

struct logMes
{
    int thread_nums;
    char mes[1024];
    long int s_time;
    struct tm *m_time;
};

int c[10][30];
int res[10][30];

void print(int n, int n_threads, char c);
void init(int n, int n_arrays);
void* routine(void* args);
void write_log(void* log_mes);

int main(int argc, char* argv[])
{
    pthread_mutex_init(&mutex_log, NULL);
    int n;
    int n_arrays;
    int i;
    struct mArgs margs[10];
    pthread_t t[100];
    srand(time(NULL));

    if(argc == 1)
    {
        printf("I need 1 argument\n");
    }else{
        n = 30;
         
        n_arrays = atoi(argv[1]);
        
        init(n, n_arrays);
        
        print(n, n_arrays, 'c');
        for(i = 0; i < n_arrays; i++)
        {
            margs[i].thread_num = i;
            pthread_create(&t[i], NULL, &routine, &margs[i]);
        }
        
        for(i = 0; i < n_arrays; i++)
        {
            pthread_join(t[i], NULL);
        }
        printf("\n");
        print(n, n_arrays, 'r');        
    }
    pthread_mutex_destroy(&mutex_log);
}

void* routine(void* args)
{
    struct mArgs *margs = (struct mArgs*)args;
    struct logMes l;
    l.s_time = time(NULL);
    l.thread_nums = margs->thread_num;
    l.m_time = localtime(&l.s_time);
    snprintf(l.mes, 100, "started work with line№%d",margs->thread_num);
    write_log(&l);

    int id = 0;
    
    for(int i = 0; i < 30; i++)
    {
        if(c[margs->thread_num][i] % 10 == 0)
        {
            l.s_time = time(NULL);
            l.m_time = localtime(&l.s_time);
            snprintf(l.mes, 100, "found mod10 num(%d) in line№%d",c[margs->thread_num][i], margs->thread_num);
            write_log(&l);   
            res[margs->thread_num][id] = c[margs->thread_num][i];
            id++;
        }
    }
    l.s_time = time(NULL);
    l.thread_nums = margs->thread_num;
    l.m_time = localtime(&l.s_time);
    snprintf(l.mes, 100, "ended work with line№%d",margs->thread_num);
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

void init(int n, int n_arrays)
{
    for(int i = 0; i < n_arrays; i++)
    {
        for(int j = 0; j < n; j++)
        {
            c[i][j] = rand()%100;
            res[i][j] = -1;
        }
    }
}


void print(int n, int n_threads, char ch)
{
    
    if(ch == 'c')
    {
        printf("Arrays:\n");
        for(int i = 0; i < n_threads; i++)
        {
            printf("Line №%d: ", i+1);
            for(int j = 0; j < n; j++)
            {       
                printf("%d ", c[i][j]);   
            }
            printf("\n");
        }
    }
    
    if(ch == 'r')
    {
        printf("Mod10 elements in arrays:\n");
        for(int i = 0; i < n_threads; i++)
        {
            printf("Line №%d: ", i+1);
            for(int j = 0; j < n; j++)
            {
                if(res[i][j] != -1)
                    printf("%d ", res[i][j]);
            }
            printf("\n");
        }
    }
}