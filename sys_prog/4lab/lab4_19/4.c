#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct threadArguments
{
    int thread_num;
};

char a[100][1000];

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

void reverse_full(char str[]);
void *func(void* args);
void clean_file();
void write_log(void* log);
void write_to_file(long int id, int i);
void swap(char *a1, char *a2);
void reverse(char str[]);

int main(int argc, char* argv[])
{
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_log, NULL);
    struct threadArguments th_args[100];
    pthread_t t[100];
    char filename[20];
    strcpy(filename, "1.txt");
    FILE *f = fopen(filename, "r");
    int n_thread = 0;
    char str[1000];
    while(fgets(str, 1000, f) != NULL)
    {
        strcpy(a[n_thread], str);
        n_thread++;
    }
    fclose(f);

    for(int i = 0; i < n_thread; i++)
    {
        th_args[i].thread_num = i;
        pthread_create(&t[i], NULL, &func, &th_args[i]);
    }

    for(int i = 0; i < n_thread; i++)
    {
        pthread_join(t[i], NULL);
    }
    f = fopen("file.txt", "w");
    for(int i = 0; i < n_thread; i++)
    {
        fprintf(f, "%s", a[i]);
    }
    fclose(f);
    for(int i = 0; i < n_thread; i++)
    {
        printf("%s", a[i]);
    }

    pthread_mutex_destroy(&mutex_log);
    pthread_mutex_destroy(&mutex);
}

void *func(void* args)
{
    struct threadArguments *th_args = (struct threadArguments*)args;
    struct logMessageArguments log_mes_arg;
    log_mes_arg.thread_nums =  th_args->thread_num + 1;
    log_mes_arg.s_time = time(NULL);
    log_mes_arg.m_time = localtime(&log_mes_arg.s_time);
    snprintf(log_mes_arg.mes, 100, "was created");
    write_log(&log_mes_arg);
    char ans[1000];
    char tmp_ans[1100];
    char flag[100];
    int id = 0;
    reverse(a[th_args->thread_num]);
    for(int i = 0; i < strlen(a[th_args->thread_num]); i++)
    {
        if(a[th_args->thread_num][i] == ' ' || a[th_args->thread_num][i] == '\n')
        {
            flag[id] = '\0';
            reverse_full(flag);
            if(a[th_args->thread_num][i] == '\n')
                snprintf(tmp_ans, 1100, "%s%s\n", ans, flag);    
            else
                snprintf(tmp_ans, 1100, "%s%s ", ans, flag);
            strcpy(ans, tmp_ans);
            id = 0;
        }else{
            flag[id] = a[th_args->thread_num][i];
            id++;
        }
    }
    strcpy(a[th_args->thread_num], ans);
    log_mes_arg.s_time = time(NULL);
    log_mes_arg.m_time = localtime(&log_mes_arg.s_time);
    snprintf(log_mes_arg.mes, 100, "ended work");
    write_log(&log_mes_arg);
}

void reverse(char str[])
{
    for(int i =0; i < (strlen(str) - 1)/2; i++)
        swap(&str[i], &str[ (strlen(str))-2-i ]);
}
void reverse_full(char str[])
{
    for(int i =0; i < strlen(str)/2; i++)
        swap(&str[i], &str[ strlen(str)-1-i ]);
}

void swap(char *a1, char *a2)
{
    char c = *a1;
    *a1 = *a2;
    *a2 = c;
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


void clean_file()
{
    FILE* f;
    f = fopen("file.txt", "w");
    fclose(f);
}