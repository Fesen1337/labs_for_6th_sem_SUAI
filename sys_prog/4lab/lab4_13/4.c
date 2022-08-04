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


int a[7];
int fact_res[7];
int res = 0;

void* prod(void*);
void write_log(void* log_mes);

int main(int argc, char* argv[])
{
    pthread_mutex_init(&mutex_log, NULL);
    struct mArgs margs[7];
    int i;
    pthread_t t[7];
    for(i = 0; i < 7; i++)
    {
        margs[i].thread_num = i;
        pthread_create(&t[i], NULL, &prod, &margs[i]);
    }
    for(i = 0; i < 7; i++)
    {
        pthread_join(t[i], NULL);
    }
    for(int i = 0; i < 7; i++)
        res += fact_res[i];
    printf("Sum of factorial from 5 to 11 = %d\n", res);
    pthread_mutex_destroy(&mutex_log);
}

void* prod(void* args)
{
    struct mArgs *margs = (struct mArgs*)args;
    struct logMes l;
    l.thread_nums =  margs->thread_num;
    l.s_time = time(NULL);
    l.m_time = localtime(&l.s_time);
    int last = margs->thread_num + 5;
    snprintf(l.mes, 100, "Start calculatuing factorial of %d", last);
    write_log(&l);

    int ans = 1;
    for(int i = 1; i <= last; i++)
    {
        ans *= i;
    }
    fact_res[margs->thread_num] = ans; 

    l.s_time = time(NULL);
    l.m_time = localtime(&l.s_time);
    snprintf(l.mes, 100, "End calculation factorial of %d. Res = %d", last, ans);
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
