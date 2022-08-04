#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct mArgs
{
    int thread_num;
};
pthread_mutex_t mutex_log;
struct logMes
{
    int thread_nums;
    char mes[1024];
    long int s_time;
    struct tm *m_time;
};


int a[100][2];
int res[100];

void print(int n, char c);
void init(int n);
void *prod(void*);
void write_log(void* log_mes);

int main(int argc, char* argv[])
{
    pthread_mutex_init(&mutex_log, NULL);
    int n;
    int n_threads;
    struct mArgs margs[100];
    int i;
    pthread_t t[100];
    srand(time(NULL));
    if(argc == 1)
    {
        printf("I need count of rectangles in arguments\n");
    }else{
        n = atoi(argv[1]); 
        n_threads = n;
        init(n);
        print(n, 'a');
        printf("\n");
        for(i = 0; i < n_threads; i++)
        {
            margs[i].thread_num = i;
            pthread_create(&t[i], NULL, &prod, &margs[i]);
        }
        for(i = 0; i < n_threads; i++)
        {
            pthread_join(t[i], NULL);
        }
        printf("\n");
        print(n, 'r');
    }
    pthread_mutex_destroy(&mutex_log);
}

void *prod(void* args)
{
    struct mArgs *margs = (struct mArgs*)args;
    struct logMes l;
    l.thread_nums =  margs->thread_num;


    l.s_time = time(NULL);
    l.m_time = localtime(&l.s_time);
    snprintf(l.mes, 100, "Start calculatuing %d item", margs->thread_num);
    write_log(&l);
    res[margs->thread_num] = a[margs->thread_num][0] * a[margs->thread_num][1];

    l.s_time = time(NULL);
    l.m_time = localtime(&l.s_time);

    snprintf(l.mes, 100, "Item : (%d) =  %d", margs->thread_num, res[margs->thread_num]);
    write_log(&l);
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

void init(int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            a[i][j] = rand()%1000 + 1;
        }
    }
}

void print(int n, char ch)
{
    for(int i = 0; i < n; i++)
    {
        if(ch == 'a')
            for(int j = 0; j < 2; j++)
            {
                
                    printf("%d ", a[i][j]);   
            }
        else{
            printf("%d * %d = %d", a[i][0], a[i][1], res[i]);
        }
        printf("\n");
    }
}