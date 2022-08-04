#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct mArgs//через эту структуру передаем аргументы в функцию, которую запускает поток
{
    char* arg;
    int thread_num;
};

struct logMes//через эту структуру передаем данные которые нужны, чтобы записать логи
{
    int thread_nums;
    char mes[1024];
    long int s_time;
    struct tm *m_time;
};
//mutex нужен чтобы синхронизировать работу потоков при борьбе за нераспределенные данные
//в область залоченную мьюексом, в которую уже зашел один поток, никакой другой не сможет зайти,
//пока первый поток не выйдет из этой области.
pthread_mutex_t mutex_long;//отдельный мьютекс под запись в файл длинных слов
pthread_mutex_t mutex_short;//отдельный мьютекс под запись в файл коротких слов
pthread_mutex_t mutex_log;//отдельный мьютекс под запись в файл логов

void* routine(void* args);//функция которую будут выполнять потоки

void write_log(void* log);//функция записи в лог

void write_short(char* a);//функция записи коротких слов

void write_long(char* a);//функция записи длинных слов

int main(int argc, char* argv[])
{
    int N = argc - 1;//количество файлов переданных прграмме.

    //инициализация мьютексов
    pthread_mutex_init(&mutex_short, NULL);
    pthread_mutex_init(&mutex_long, NULL);
    pthread_mutex_init(&mutex_log, NULL);

    //открываем и закрываем файлы для длинных и коротких слов, чтобы очистить их.
    FILE* clear;
    clear = fopen("short_words.txt", "w");
    fclose(clear);
    clear = fopen("long_words.txt", "w");
    fclose(clear);

    //инициализация стуктуры для хранения аргументов для каждого потока.
    struct mArgs margs[20];   
    int i;

    pthread_t t[20];//сами потоки

    for(i = 0; i < N; i++)
    {
        margs[i].arg = argv[i+1];//передаем название файла, с которым будет работать поток
        margs[i].thread_num = i + 1;
        pthread_create(&t[i], NULL, &routine, &margs[i]);//создаем поток с заданными аргументами
    }

    for(i = 0; i < N; i++)
    {
        pthread_join(t[i], NULL);//ждем пока все потоки закончат работу так же нужен для возврата знач. из ф-ии
    }
    
    //уничтожаем мьютексы
    pthread_mutex_destroy(&mutex_log);
    pthread_mutex_destroy(&mutex_short);
    pthread_mutex_destroy(&mutex_long);
    
    
}


void write_log(void* log_mes)
{
    pthread_mutex_lock(&mutex_log);//включаем мьютекс
    
    struct logMes *logm = (struct logMes*)log_mes;//каст void* в структуру для аргументов логов
    
    FILE* log;
    log = fopen("log.log", "a");//файл записи логов

    char str[128];//для даты
    char buf[1024];//общая строка
    
    strftime(str, 128, "Date: %x %X", logm->m_time);//получаем дату и время в строку
    snprintf(buf, 2048, "%s: Thread #%d: %s \n", str, logm->thread_nums, logm->mes);//формируем общую строку.

    fprintf(log, "%s", buf);//пишем в файл
    fclose(log);

    pthread_mutex_unlock(&mutex_log);//выключаем мьютекс
}



void* routine(void* args)//главная функция
{
    struct mArgs *margs = (struct mArgs*)args;//кастим void* в структуру для аргументов    
    //заполняем данные для логов
    struct logMes l;
    l.thread_nums =  margs->thread_num;
    l.s_time = time(NULL);
    l.m_time = localtime(&l.s_time);
    snprintf(l.mes, 100, "Try to read file: %s", margs->arg);
    write_log(&l);//пишем в лог о начале работы потока

    FILE* f;
    char buffer[20];
    char q;
    f = fopen(margs->arg, "r");//открываем файл
    if(f == NULL)//обрабатывам ошибки, пишем в лог.
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
        write_log(&l);//пишем в лог, что открыли файл
        fscanf(f, "%s", buffer);//начинаем читать файл
        while(!feof(f))//читаем пока файл не кончится
        {
            if(strlen(buffer) <= 5)//короткое слово
            {
                l.s_time = time(NULL);
                l.m_time = localtime(&l.s_time);
                snprintf(l.mes, 100, "Wrote word |%s| to shorts from file: %s", buffer, margs->arg);
                write_log(&l);//пишем лог, что нашли короткое слово
                write_short(buffer);//пишем это слово в файл коротких слов.
            }
            if(strlen(buffer) >= 10)//длинное слово
            {
                l.s_time = time(NULL);
                l.m_time = localtime(&l.s_time);
                snprintf(l.mes, 100, "Wrote word |%s| to longs from file: %s", buffer, margs->arg);
                write_log(&l);//пишем лог, что нашли длинное слово
                write_long(buffer);//пишем слово в файл длинных слов
            }
            fscanf(f, "%s", buffer);//читаем очередное слово из файла
            
        }
        fclose(f);
        l.s_time = time(NULL);
        l.m_time = localtime(&l.s_time);
        snprintf(l.mes, 100, "Closed file: %s", margs->arg);
        write_log(&l);// как закончили обработку файла пишем об этом в лог
    }
    
    
}

void write_long(char* a)//просто запись с мьюексом в файл длинных слов
{
    pthread_mutex_lock(&mutex_long);
    FILE* f;
    f = fopen("long_words.txt", "a");
    fprintf(f, "%s\n", a);
    fclose(f);
    pthread_mutex_unlock(&mutex_long);
}

void write_short(char* a)//просто запись с мьюексом в файл коротких слов
{
    pthread_mutex_lock(&mutex_short);

    FILE* f;
    f = fopen("short_words.txt", "a");
    fprintf(f, "%s\n", a);
    fclose(f);

    pthread_mutex_unlock(&mutex_short);
}