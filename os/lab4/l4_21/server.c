#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h> 

#define DEFAULT_PORT 1601
#define BUFFER_SIZE 1024
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'

int client, server; 
int servs[100]; 
char buffer[BUFFER_SIZE];
void* clientHandler(void* server);
int is_num(char *str);
void itoa(int n, char s[]);
void reverse(char s[]);
void buffer_to_line(int line_id, int* a, char* buf);
void line_to_buffer(int* a, int k, char* buf);
int task_func(int, char*);
void* server_routine(); // общаемся с клиентом
void enter(int, char*);


int main(int argc, char* argv[])
{
    pthread_t server;
    pthread_create(&server, NULL, &server_routine, NULL);//заупускаем работу сервера в отдельном потоке, чтобы можно было отпралять команды серверу в
    pthread_yield(server);                               //его же консоли 

    char c;
    while(1)
    {
        scanf("%c", &c);
        if(c == '#')// работа прерывается по символу #
        {
            printf("SERVER LOG: server off.\n");
            close(client);
            return 0;
        }
    }

}

void *server_routine()
{
    pthread_t t[100];
    int last_handle = 0;

    struct sockaddr_in server_address;
    client = socket(AF_INET, SOCK_STREAM, 0);//созаем сокет сервера
    if(client < 0)
    {
        printf("SERVER ERROR: establishing socket error.");
        exit(0);
    }

    printf("SERVER LOG: socket for server was successfully created\n");

    server_address.sin_port = htons(DEFAULT_PORT);//устанавливаем порт 
    server_address.sin_family = AF_INET;//устанавливаем семейство адресов 
    server_address.sin_addr.s_addr = htons(INADDR_ANY);//разрешаем любые адреса

    int ret = bind(client, (struct sockaddr *)&server_address, sizeof(server_address)); //устаннавливаем имя для сокета сервера
    if(ret < 0)
    {
        printf("SERVER ERROR: binding connection. Socket was already been establishing");
        return 0;

    }   

    socklen_t size = sizeof(server_address);
    printf("SERVER LOG: Listening clients... \n");
    listen(client, SOMAXCONN);//ждем подключения клиента

    while(1)
    {
        server = accept(client, (struct sockaddr *)&server_address, &size);//как только подключился какой то клиент создаем сокет для общения с ним
                                                                           //это делаем т.к. может подключаться много клиентов
        if(server < 0)
        {
            printf("SERVER ERROR: Cant accepting client.\n");
        }
        
        strcpy(buffer, "server connected\n");
        send(server, buffer, BUFFER_SIZE, 0);//отправляем сообщение клиенту, что было установлено соединение 
        servs[last_handle] = server;
        printf("CLIENT №%d: Connected!\n", server);
        pthread_create(&t[last_handle], NULL, &clientHandler, &servs[last_handle]);//запускаем обработку запросов клиента в отдельном потоке
                                                                                   //чтобы можно было обрабатывать работу нескольких клиентов одновременно
        pthread_yield(t[last_handle]);
        last_handle++;
    }
}

void* clientHandler(void* server)
{
    int *ser = (int*)server;
    char buf[BUFFER_SIZE];
    int a[100][100];
    int n, k;
    while(1)
    {
        if(recv(*ser, buf, BUFFER_SIZE, 0) == -1)//получаем от клиента запрос
            break;
        if(buf[0] == '#')//если клиент прислал # , то отключаем его
        {
            printf("CLIENT №%d: was unconnected\n",*ser);
            break;
        }
        send(*ser, buf, BUFFER_SIZE, 0);//отправляем клиенту подтверждение о получении запроса

        printf("CLIENT №%d: %s", *ser, buf);


        int res = task_func(*ser, buf);//выполняем запрос пользователя о вычислении выражения


        strcpy(buf, "OK");
        
        
        
	    snprintf(buf, 1024, "Result = %d", res);//выведем в консоль сервера полученные данные от клиента
        send(*ser, buf, BUFFER_SIZE, 0);//отправим клиенту ответ на его запрос
        recv(*ser, buf, BUFFER_SIZE, 0);//получим от клиента подтверждение о получении ответа

    }
    
}

int prior(char c)//служебная функция чтоб вычислять выражение 
{
    if(c == '+' || c == '-')
        return 2;
    if(c == '*' || c == '/')
        return 1;
    return 3;
}

void enter(int ser, char*str)//перевод выражения в обратную польскую нотацию 
{
    char filename[10];
    snprintf(filename, 10, "%d.txt", ser);
    FILE *f = fopen(filename, "w");
    char buf[100];
    int sp = -1;
    char c;
    int i = 0;
    while(1)
    {

        c = str[i];
        i++;
        if(c >= '0' && c <= '9')
            fprintf(f, "%c ", c);
        switch(c)
        {
            case '(':
                sp++;
                buf[sp] = c;
            break;
            case ')':
                while(buf[sp] != '(')
                {
                    fprintf(f, "%c ", buf[sp--]);
                }
                sp--;
            break;
            case '+':
                while(prior(c) >= prior(buf[sp]))
                {
                    fprintf(f, "%c ", buf[sp--]);
                }
                sp++;
                buf[sp] = c;
            break;
            case '-':
                while(prior(c) >= prior(buf[sp]))
                {
                    fprintf(f, "%c ", buf[sp--]);
                }
                sp++;
                buf[sp] = c;
            break;
            case '*':
                while(prior(c) >= prior(buf[sp]))
                {
                    fprintf(f, "%c ", buf[sp--]);
                }
                sp++;
                buf[sp] = c;
            break;
            case ' ':
            break;
            case '/':
                while(prior(c) >= prior(buf[sp]))
                {
                    fprintf(f, "%c ", buf[sp--]);
                }
                sp++;
                buf[sp] = c;
            break;
            case '\n':
                while(sp >= 0)
                {
                    fprintf(f, "%c ", buf[sp--]);
                    
                }
                fprintf(f, "=");
                fclose(f);
                return;
            break;
            case '\0':
                while(sp >= 0)
                {
                    fprintf(f, "%c ", buf[sp--]);
                    
                }
                fprintf(f, "=");
                fclose(f);
                return;
            break;
                
        }
    }   
    
}


int task_func(int ser, char *str)//выполнение запроса пользователя -> переводим выражение в обратную польскую нотацию, и вычисляем её
{
    char filename[10];
    snprintf(filename, 10, "%d.txt", ser);
    int stack[256];
    char buf[256];
    int sp = 0;
    FILE *f = fopen(filename, "r");
    enter(ser, str);
    while(!feof(f))
    {
        if(fscanf (f, "%s", buf) != 1 )
            break;
        switch(buf[0])
        {
            case '\0':
                break;
            case '=':
                break;
            case '+':
               stack[sp-2] = stack[sp-2] + stack[sp-1];
               sp--;
               break;
            case '-':
               stack[sp-2] = stack[sp-2] - stack[sp-1];
               sp--;
               break;
            case '*':
               stack[sp-2] = stack[sp-1] * stack[sp-2];
               sp--;
               break;
            case '/':
               stack[sp-2] = stack[sp-2] / stack[sp-1];
               sp--;
               break;
            default:
               stack[sp++] = atoi(buf);
        }
    }
    fclose(f);
    //zrremove(filename);
    return stack[sp-1];
}

void line_to_buffer(int* a, int k, char* buf)
{
    char num[4];
    char b[256];
    buf[0] = '\0';
    for(int i = 0; i < k; i++)
    {
        strcpy(b, buf);
        itoa(a[i], num);
        snprintf(buf, 1024, "%s%s ", b, num);
    }
}

void buffer_to_line(int line_id, int *a, char *buf)
{
    char b[10];
    int flag = 0;
    int cur = 0;
    for(int i = 0; i < strlen(buf); i++)
    {
        if(buf[i] == ' ')
        {
            b[flag] = '\0';
            a[cur] = atoi(b);
            cur++;
            flag = 0;
        }else{
            b[flag] = buf[i];
            flag++;
        }
    }
}

int is_num(char *str)
{
    for(int i = 0; i < strlen(str); i++)
        if(str[i] < '0' || str[i] > '9')
            return 0;
    return 1;
}

void itoa(int n, char s[])
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

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}
