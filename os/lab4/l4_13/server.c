/*
Задание:
Генерация последовательности Фибоначчи:
на входе начальные параметры последовательности F0 и F1 и 
длина ряда. Формула ряда следующая: Fn = Fn-1 + Fn-2

Пример ряда: 1 1 2 3 5 8 13 21 34...
Каждое новое число ряда определяется как сумма двух предыдущих элементов ряда

*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h> 
#include <math.h>

#define BUFFER_SIZE 1024 //размер буффера, в который будут записываться сообщения перед отправкой
#define SERVER_IP "127.0.0.1"//ip адрес по которому будет общение в нашем случае loopback
#define DEFAULT_PORT 1601//порт по которому создадим соединение
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'//забей это осталось от старого кода, можно стереть

int client, server;//в них будем записывать дескрипторы сокетов
int servs[100];//в них будем записывать аргумент для работы потоков (в каждом потоке будут обрабатываться запросы одного пользователя)
char buffer[BUFFER_SIZE];//сюда будут записываться запросы перед отправкой, а также пришедшие ответы
void* clientHandler(void* server);//функция, в которой происходит оработка запросов пользователя
int is_num(char *str);//проверка является ли введенная строка числом
void itoa(int n, char s[]);//перевод числа в строку
void reverse(char s[]);//переворот строки, нужен для работы функции itoa 
void task_func(int f0, int f1, int n, int*ser);//выполняется функция указанная в задании в нашем случае вычисляем ряд фибоначчи
void* server_routine();//функция работы сервера

int main(int argc, char* argv[])
{
    pthread_t server;
    pthread_create(&server, NULL, &server_routine, NULL);//создание потока в котором будет работать сервер(это нужно чтобы мы в любой момент могли через консоль офффнуть сервер)
    pthread_yield(server);//сервер будет работать пока работает main
    char c;
    while(1)
    {
        scanf("%c", &c);
        if(c == '#')//если ввели # закоончим работу сервера
        {
            printf("SERVER LOG: server off.\n");
            close(client);//закроем сокет
            return 0;
        }
    }

}

void *server_routine()
{
    pthread_t t[100];//потоки для обработки запросов клиентов
    int last_handle = 0;//индекс первого свободного потока

    struct sockaddr_in server_address;//структура адреса sockaddr_in, а не sockaddr, т.к. мы хотим использовать tcp/ip !!
    client = socket(AF_INET, SOCK_STREAM, 0);//создаем сокет сервера, возвращает дескриптор сокета сервера!!
    if(client < 0)//если не удалось создать сокет выведем ошибку
    {
        printf("SERVER ERROR: establishing socket error.");
        exit(0);
    }

    printf("SERVER LOG: socket for server was successfully created\n");//если удалось напишем об этом

    //заполняем поля структуры адреса
    server_address.sin_port = htons(DEFAULT_PORT);//указываем порт!!
    server_address.sin_family = AF_INET;//семейство адресов!!
    server_address.sin_addr.s_addr = htons(INADDR_ANY);//какой может быть IP (INADDR_ANY - любой) !!

    int ret = bind(client, (struct sockaddr *)&server_address, sizeof(server_address));//делаем bind - присваивает сокету имя. До присваивания сокет недоступен программам-клиентам!!
    if(ret < 0)//если не удалось выведем ошибку
    {
        printf("SERVER ERROR: binding connection. Socket was already been establishing");
        return 0;

    }   

    socklen_t size = sizeof(server_address);
    printf("SERVER LOG: Listening clients... \n");
    listen(client, SOMAXCONN);//слушаем клиентов по указанному сокету - SOMAXCONN - неограниченное кол-во пользователей !! 

    while(1)
    {
        server = accept(client, (struct sockaddr *)&server_address, &size);//accept - создает сокет и возвращает его дескриптор. Создает !! 
        //отдельный сокет под каждое новое подключение, это позволяет подключаться к серверу нескольким пользователямы
        if(server < 0)//если не удалось выведем ошибку
        {
            printf("SERVER ERROR: Cant accepting client.\n");
        }
        
        strcpy(buffer, "server connected\n");
        send(server, buffer, BUFFER_SIZE, 0);//отошлем клиенту, что подключение произошло успешно
        servs[last_handle] = server;
        pthread_create(&t[last_handle], NULL, &clientHandler, &servs[last_handle]);//создадим поток в котором будем обрабатывать запросы пользователя
        pthread_yield(t[last_handle]);//работает до тех пор пока работает сервер
        last_handle++;//указываем что заняли один поток и теперь первый свободный - следующий
    }
}

void* clientHandler(void* server)
{
    int *ser = (int*)server;//рекастим в int дескриптор сокета по которому идет общение с клиентом
    char buf[BUFFER_SIZE];//буффер уже писал зачем
    int a[100];//не нужен, забыл удалить
    int n, f0, f1;//параметры необходимые для вычисления ряда
    recv(*ser, buf, BUFFER_SIZE, 0);//получваем подтверждение от клиента что произошло подключение
    printf("Hello, client №%d!\n", *ser);//пишем уведомление, что клиент подключился
    while(1)
    {
        if(recv(*ser, buf, BUFFER_SIZE, 0) == -1)//получаем первый параметр от клиента
            break;
        if(buf[0] == '#')//если это терминирующий символ (#) - отключим клиента.
        {
            printf("Goodbye, client№%d\n",*ser);
            break;
        }
            
        n = atoi(buf);//переводим первый полученный параметр(n) в число 
        strcpy(buf, "OK");
        send(*ser, buf, BUFFER_SIZE, 0);//отправляем клиенту подтверждения получения

        recv(*ser, buf, BUFFER_SIZE, 0);//получаем следующий параметр
        f0 = atoi(buf);//переводим первый полученный параметр(f0) в число 
        strcpy(buf, "OK");
        send(*ser, buf, BUFFER_SIZE, 0);//отправляем клиенту подтверждения получения
        
        recv(*ser, buf, BUFFER_SIZE, 0);//получаем следующий параметр
        f1 = atoi(buf);//переводим первый полученный параметр(f1) в число 
        strcpy(buf, "OK");
        send(*ser, buf, BUFFER_SIZE, 0);//отправляем клиенту подтверждения получения

        printf("Client №%d: n = %d, F0 = %d, F1 = %d\n", *ser, n, f0, f1);//выведем на сервере сообщение о всех полученных параметрах
        task_func(f0, f1, n, ser);//запустим функцию формирования ряда фибоначчи по полученным параметрам

    }
    
}

void task_func(int f0, int f1, int n, int *ser)
{
    char buf[1024];//ну уже не смешно, буффер это
    itoa(n, buf);//переводим n в строку
    send(*ser, buf, BUFFER_SIZE, 0);//отправляем клиенту количество пакетов, которое он будет должен получить
    recv(*ser, buf, BUFFER_SIZE, 0);//получаем подтверждения от клиента о получении сообщения

    int res[1000];//сюда запишем ряд фибоначчи

    res[0] = f0;//первое число ряда 
    res[1] = f1;//второе число ряда 
    for(int i = 2; i < n; i++)
    {
        res[i] = res[i - 1] + res[i - 2];//вычисление iго числа ряда
    }

    for(int i = 0; i < n; i++)
    {
        itoa(res[i], buf);//переводим i-ое число ряда в строку
        send(*ser, buf, BUFFER_SIZE, 0);//отправляем его клиенту
        recv(*ser, buf, BUFFER_SIZE, 0);//получаем подтвержение получения
    }
    
}

void itoa(int n, char s[])//перевод числа в строку (украдено) позаимствовано с stackoverflow
{
    int i, sign;

    if ((sign = n) < 0)  
        n = -n;          
    i = 0;
    do {       
        s[i++] = n % 10 + '0';   
    } while ((n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void reverse(char s[])//реверс строки (украдено) позаимствовано с stackoverflow
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) 
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

