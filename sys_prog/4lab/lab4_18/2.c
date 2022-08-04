#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct mArgs
{
    int n;
    int start;
    int step;  
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


int a[100][100];
int b[100][100];
int c[100][100];

void print(int n, char c);
void init(int n);
void prod(void*);

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
        printf("I need 2 arguments\n");
    }else if(argc == 2)
    {
        printf("I need 2 arguments\n");
    }else{
        n = atoi(argv[1]); 
        n_threads = atoi(argv[2]);
        init(n);
        print(n, 'a');
        printf("\n");
        print(n, 'b');
        for(i = 0; i < n_threads; i++)
        {
            margs[i].n = n;
            margs[i].start = i;
            margs[i].step = n_threads;
            margs[i].thread_num = i;
            pthread_create(&t[i], NULL, &prod, &margs[i]);
        }
        for(i = 0; i < n_threads; i++)
        {
            pthread_join(t[i], NULL);
        }
        printf("\n");
        print(n, 'c');
    }
    pthread_mutex_destroy(&mutex_log);
}

void prod(void* args)
{
    struct mArgs *margs = (struct mArgs*)args;
    struct logMes l;
    l.thread_nums =  margs->thread_num;
    int cur = margs->start;
    int n = margs->n;

    int i, j;
    transform(n, cur, &i, &j);
    int k = 0;

    while(cur < n*n)
    {   
        l.s_time = time(NULL);
        l.m_time = localtime(&l.s_time);
        snprintf(l.mes, 100, "Start calculatuing %d %d item", i, j);
        write_log(&l);
        c[i][j] = 0;
        for(k = 0; k < n; k++)
        {
            c[i][j] += (a[i][k] * b[k][j]);
        }
        l.s_time = time(NULL);
        l.m_time = localtime(&l.s_time);
        snprintf(l.mes, 100, "Item : (%d %d) =  %d", i, j, c[i][j]);
        write_log(&l);
        cur += margs->step;
        transform(n, cur, &i, &j);
    }
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

void transform(int n, int cur, int* i, int* j)
{
    *i = cur/n;
    *j = cur%n;
}

void init(int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            a[i][j] = rand()%10;
            b[i][j] = rand()%10;
        }
    }
}

void print(int n, char ch)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(ch == 'a')
                printf("%d ", a[i][j]);
            if(ch == 'b')
                printf("%d ", b[i][j]);
            if(ch == 'c')
                printf("%d ", c[i][j]);   
        }
        printf("\n");
    }
}