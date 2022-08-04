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
void task_func(int n, int b, int q, int*ser);
void* server_routine();

int main(int argc, char* argv[])
{
    pthread_t server;
    pthread_create(&server, NULL, &server_routine, NULL);
    pthread_yield(server);
    char c;
    while(1)
    {
        scanf("%c", &c);
        if(c == '#')
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
    client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0)
    {
        printf("SERVER ERROR: establishing socket error.");
        exit(0);
    }

    printf("SERVER LOG: socket for server was successfully created\n");

    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    int ret = bind(client, (struct sockaddr *)&server_address, sizeof(server_address)); 
    if(ret < 0)
    {
        printf("SERVER ERROR: binding connection. Socket was already been establishing");
        return 0;

    }   

    socklen_t size = sizeof(server_address);
    printf("SERVER LOG: Listening clients... \n");
    listen(client, SOMAXCONN);

    while(1)
    {
        server = accept(client, (struct sockaddr *)&server_address, &size);
        if(server < 0)
        {
            printf("SERVER ERROR: Cant accepting client.\n");
        }
        
        strcpy(buffer, "server connected\n");
        send(server, buffer, BUFFER_SIZE, 0);
        servs[last_handle] = server;
        pthread_create(&t[last_handle], NULL, &clientHandler, &servs[last_handle]);
        pthread_yield(t[last_handle]);
        last_handle++;
    }
}

void* clientHandler(void* server)
{
    int *ser = (int*)server;
    char buf[BUFFER_SIZE];
    int a[100];
    int n, b, q;
    recv(*ser, buf, BUFFER_SIZE, 0);
    printf("Hello, client №%d!\n", *ser);
    while(1)
    {
        if(recv(*ser, buf, BUFFER_SIZE, 0) == -1)
            break;
        if(buf[0] == '#')
        {
            printf("Goodbye, client№%d\n",*ser);
            break;
        }
            
        n = atoi(buf);
        strcpy(buf, "OK");
        send(*ser, buf, BUFFER_SIZE, 0);

        recv(*ser, buf, BUFFER_SIZE, 0);
        b = atoi(buf);
        strcpy(buf, "OK");
        send(*ser, buf, BUFFER_SIZE, 0);
        
        recv(*ser, buf, BUFFER_SIZE, 0);
        q = atoi(buf);
        strcpy(buf, "OK");
        send(*ser, buf, BUFFER_SIZE, 0);

        printf("Client №%d: n = %d, b = %d, q = %d\n", *ser, n, b, q);
        task_func(n, b, q, ser);

    }
    
}

void task_func(int n, int b, int q, int *ser)
{
    char buf[1024];
    itoa(n, buf);
    send(*ser, buf, BUFFER_SIZE, 0);
    recv(*ser, buf, BUFFER_SIZE, 0);

    int res[1000];

    res[0] = b;
    for(int i = 1; i < n; i++)
    {
        res[i] = q*res[i - 1];
    }

    for(int i = 0; i < n; i++)
    {
        itoa(res[i], buf);
        send(*ser, buf, BUFFER_SIZE, 0);
        recv(*ser, buf, BUFFER_SIZE, 0);
    }
    
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

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) 
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

