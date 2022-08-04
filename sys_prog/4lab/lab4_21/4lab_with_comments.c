#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int queue_size = 20;//размер очереди 
int id_next_client = 0;//номер клиента которого следующего нужно обслужить
int queue[100];// массив клиентов (0 - обслужен, 1 - еще нет)

pthread_mutex_t mutex; //объявление мьютекса, он нужен, чтобы у потоков не было борьбы за нераспределенные ресурсы

struct mArgs//аргументы, которые передаем в поток
{
    char* name;// имя консультанта
    int thread_num;// номер потока
};

struct logMes// нужна для организации записи в лог
{
    FILE* f;//ссылка на файл логов
    char mes[1024];//сообщение которое пишем в лог
    long int s_time;
    struct tm *m_time;//время сообщения.
};

//объявляем потоки и их переменные
pthread_t t[3];
struct mArgs margs[3];

void init();//инициализируем значение переменных потоков
void* routine(void* args);//функция которую будут выполнять потоки
int try_to_take_client();//попытка достать клиента из очереди
void write_log(void* log_mes);//запись в лог


int main(int argc, char* argv[])
{
    if(argc == 1)
    {
        printf("I need queue size in argument for working\n");
    }else{
        srand(time(NULL));//чтоб рандом работал
        queue_size = atoi(argv[1]);//считываем размер очереди 
        pthread_mutex_init(&mutex, NULL);//инициализация мьютекса
        init();

        //создаем потоки
        pthread_create(&t[0], NULL, &routine, &margs[0]);
        pthread_create(&t[1], NULL, &routine, &margs[1]);
        pthread_create(&t[2], NULL, &routine, &margs[2]);

        //дожидаемся выполнения всех потоков
        pthread_join(t[0], NULL);
        pthread_join(t[1], NULL);
        pthread_join(t[2], NULL);

        pthread_mutex_destroy(&mutex);//уничтожаем мьютекс
        printf("All clients are served\n");
    }
}

void* routine(void* args)
{
    struct mArgs *margs = (struct mArgs*)args;//достаем аргументы
    struct logMes l;//создаем структуру формирования логов
    char log_file_name[20];
    snprintf(log_file_name, 20, "%s.log", margs->name);
    FILE *log_file = fopen(log_file_name,"w");//открываем файл логов
    l.f = log_file;
    l.s_time = time(NULL);
    l.m_time = localtime(&l.s_time);//установка времени
    snprintf(l.mes, 1024, "%s went to work", margs->name);//вывод сообщения в логи
    write_log(&l);
    


    int action;
    while(1)
    {
        action = rand()%10;//генерируем событие (1 - 9 обслуживание клиентов), (0 - выход на обеды)
        if(action != 0)
        {
            int id = try_to_take_client();//пытаемся взять клиента
            if(id == -1)
            {
                return;//если клиенты кончились - завершаем работу
            }else{
                l.s_time = time(NULL);
                l.m_time = localtime(&l.s_time);
                snprintf(l.mes, 1024, "%s took client №%d", margs->name, id + 1);
                write_log(&l);
                
                printf("%s took client %d\n", margs->name, id + 1);
                complete(id);//если удалось взять клиента - обслуживаем его
                l.s_time = time(NULL);


                l.s_time = time(NULL);
                l.m_time = localtime(&l.s_time);
                snprintf(l.mes, 1024, "%s ended work with client №%d", margs->name, id + 1);
                write_log(&l);
            }
        }else{
            l.s_time = time(NULL);
            l.m_time = localtime(&l.s_time);
            snprintf(l.mes, 1024, "%s went to launch", margs->name);
            write_log(&l);
    
            printf("%s went to launch\n", margs->name);
            sleep(3);//симулируем выход на обед

            l.s_time = time(NULL);
            l.m_time = localtime(&l.s_time);
            snprintf(l.mes, 1024, "%s went from launch", margs->name);
            write_log(&l);
            
        }
    }
    
}

void write_log(void* log_mes)
{   //запись в лог
    struct logMes *logm = (struct logMes*)log_mes;
    
    char str[128];
    char buf[1024];
    
    strftime(str, 128, "Date: %x %X", logm->m_time);
    snprintf(buf, 2048, "%s: %s \n", str, logm->mes);

    fprintf(logm->f, "%s", buf);
}

void complete(int id)
{
    int t = rand()%3;//генерируем время, которое будет затрачено на обслуживание клиента
    queue[id] = 0;//отмечаем его, как обслуженного 
    sleep(t+1);//симулируем работу консультанта
}

int try_to_take_client()
{
    pthread_mutex_lock(&mutex);     //выставляем мьютекс (если какой то поток вошел в эту часть кода, 
    int flag = -1;                  //остальные потоки не смогут войти в эту часть кода, пока вошедший 
    if(id_next_client < queue_size) //поток не завершит работу
    {
        flag = id_next_client;//если есть еще клиенты возьмем текущего
        id_next_client++;
    }
    pthread_mutex_unlock(&mutex);
    return flag;
}


void init()
{
    margs[0].name = "Chebyshev";
    margs[1].name = "Kolmogorov";
    margs[2].name = "Raygorodsky";

    margs[0].thread_num = 1;
    margs[1].thread_num = 2;
    margs[2].thread_num = 3;

    for(int i = 0; i < 100; i++)
    {
        queue[i] = 1;
    }
}
