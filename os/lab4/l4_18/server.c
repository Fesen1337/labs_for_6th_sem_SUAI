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
void task_func(int** a, int n, int k);
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
        printf("CLIENT №%d: Connected!\n", server);
        pthread_create(&t[last_handle], NULL, &clientHandler, &servs[last_handle]);
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
        if(recv(*ser, buf, BUFFER_SIZE, 0) == -1)
            break;
        if(buf[0] == '#')
        {
            printf("CLIENT №%d: was unconnected\n",*ser);
            break;
        }
            
        n = atoi(buf);
        strcpy(buf, "OK");
        send(*ser, buf, BUFFER_SIZE, 0);
        recv(*ser, buf, BUFFER_SIZE, 0);
        k = atoi(buf);
        strcpy(buf, "OK");
        send(*ser, buf, BUFFER_SIZE, 0);
        n--;
        printf("CLIENT №%d: n = %d, k = %d and 2D array\n", *ser, n, k);
        for(int i = 0; i < n; i++)
        {
            recv(*ser, buf, BUFFER_SIZE, 0);
            buffer_to_line(i, (int*)(a+i), buf);
            strcpy(buf, "OK");
            send(*ser, buf, BUFFER_SIZE, 0);
        }

        task_func(a, n, k);
    
        itoa(n, buf);
        send(*ser, buf, BUFFER_SIZE, 0);
        recv(*ser, buf, BUFFER_SIZE, 0);
        for(int i = 0; i < n; i++)
        {
            line_to_buffer(a[i], k, buf);
            send(*ser, buf, BUFFER_SIZE, 0);
            recv(*ser, buf, BUFFER_SIZE, 0);
        }

    }
    
}

void task_func(int** a, int n, int k)
{
    int *p_array=(int*)a;
    int neg_c = 0;
    int pos_c = 0;
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < k; j++)
        {
            if(p_array[i * 100 + j] > 0)
            {
                pos_c++;
            }else{
                neg_c++;
            }
        }
    }
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < k; j++)
        {
            if(neg_c < pos_c)
            {
                if(p_array[i * 100 + j] < 0)
                {
                    p_array[i * 100 + j] = 0;
                }
            }else{
                if(p_array[i * 100 + j] > 0)
                {
                    p_array[i * 100 + j] = 0;
                }
            }
        }
    }

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