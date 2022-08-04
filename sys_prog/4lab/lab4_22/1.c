#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct threadArguments
{
    int thread_num;
};

struct logMessageArguments
{
    int thread_nums;
    char mes[1024];
    long int s_time;
    struct tm *m_time;
};

int time_now = 0;
int stop = 0;
pthread_mutex_t mutex_log;
pthread_mutex_t mutex;

void *func(void* args);

void *tick();

void clean_file();

void write_log(void* log);

void write_to_file(long int id, int i);

int is_num(char* str);

int main(int argc, char* argv[])
{
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_log, NULL);
    struct threadArguments th_args[20];
    pthread_t t[20];
    pthread_t timer;

    if(argc == 1)
    {
        printf("I need time in argument for working\n");
    }else if(argc == 2)
    {
        if(is_num(argv[1]) == 0)
        {
            printf("Time argument must be a digit\n");
            return 0;
        }
        int n_thread = atoi(argv[1]);
        int i;
        
        if(n_thread > 10)
        {
            printf("Please use argument < 11 for non-produce too much files\n");
        }else{
            clean_file();
            pthread_create(&timer, NULL, &tick, NULL);
            pthread_yield(timer);
            for(i = 0; i < n_thread; i++)
            {
                th_args[i].thread_num = i+1;
                pthread_create(&t[i], NULL, &func, &th_args[i]);
                int flag = time_now;
                while(1)
                {
                    if(flag != time_now)
                    {
                        break;
                    }
                }       
            }
            stop = 1;
            for(i = 0; i < n_thread; i++)
            {
                pthread_join(t[i], NULL);
            }
        }

    }else{
        printf("Too much arguments\n");
    }
    pthread_mutex_destroy(&mutex_log);
    pthread_mutex_destroy(&mutex);
}

void *func(void* args)
{
    char filename[20];
    int my_timer = 0;
    struct threadArguments *th_args = (struct threadArguments*)args;
    struct logMessageArguments log_mes_arg;

    log_mes_arg.thread_nums =  th_args->thread_num;
    log_mes_arg.s_time = time(NULL);
    log_mes_arg.m_time = localtime(&log_mes_arg.s_time);
    snprintf(log_mes_arg.mes, 100, "was created");
    write_log(&log_mes_arg);

    write_to_file(pthread_self(), th_args->thread_num);
    snprintf(filename, 20, "%d.txt", th_args->thread_num);
    FILE* f = fopen(filename, "w");
    while(stop == 0)
    {
        int flag = time_now;
        while(stop == 0)
        {
            if(flag != time_now)
            {
                break;
            }
        }       
            
        my_timer++;
        fprintf(f, "My name is %lu, and I'm alive %d seconds yet.\n",pthread_self(), my_timer);
        snprintf(log_mes_arg.mes, 100, "are showing signs of life");
        write_log(&log_mes_arg);
    }
    fclose(f);
}

void write_to_file(long int id, int i)
{
    pthread_mutex_lock(&mutex);
    FILE* f;
    f = fopen("file.txt", "a");
    fprintf(f, "I was born! My name is %lu. You can find me in file %d.txt\n", id, i);
    printf("I was born! My name is %lu. You can find me in file %d.txt\n", id, i);
    fclose(f);
    pthread_mutex_unlock(&mutex);

}

void write_log(void* log_mes)
{
    pthread_mutex_lock(&mutex_log);
    
    struct logMessageArguments *log_mes_arg = (struct logMes*)log_mes;
    
    FILE* log;
    log = fopen("log.log", "a");

    char str[128];
    char buf[1024];
    
    strftime(str, 128, "Date: %x %X", log_mes_arg->m_time);
    snprintf(buf, 2048, "%s: Thread #%d: %s \n", str, log_mes_arg->thread_nums, log_mes_arg->mes);

    fprintf(log, "%s", buf);
    fclose(log);

    pthread_mutex_unlock(&mutex_log);
}

void* tick()
{
    while(1)
    {
        sleep(1);
        time_now++;
    }
}

void clean_file()
{
    FILE* f;
    f = fopen("file.txt", "w");
    fclose(f);
}

int is_num(char* str)
{
    for(int i = 0; i < strlen(str); i++)
    {
        if(str[i] > '9' || str[i] < 0)
        {
            return 0;
        }
    }
    return 1;
}