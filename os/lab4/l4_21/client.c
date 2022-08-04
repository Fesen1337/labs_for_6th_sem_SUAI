#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <time.h>
#define BUFFER_SIZE 1024

#define SERVER_IP "127.0.0.1"
int DEFAULT_PORT;
char* buffer;
size_t buffer_size = 1024;

void itoa(int n, char s[]);
void reverse(char s[]);
void line_to_buffer(int line_id, int k);
int is_num(char *arr);



int main(int argc, char* argv[])
{
    int n, k;
    char N[100], K[100];
    srand(time(NULL));
    int default_port = 1601;
    if(argc > 1)
        default_port = atoi(argv[1]);

    DEFAULT_PORT = default_port;


    while(1)
    {

        int client = socket(AF_INET, SOCK_STREAM, 0);//создать сокет клиента 
        struct sockaddr_in server_address;
        buffer = (char *)malloc(buffer_size * sizeof(char));
        if(client < 0)
        {
            printf("ERROR: Establishing socket error.\n");
            exit(0);
        }
        server_address.sin_port = htons(DEFAULT_PORT);//задаем порт
        server_address.sin_family = AF_INET;//задаем семейство адресов
        inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);//переводим ip из десятичнонго в двоичный
        printf("LOG: Client socket created\n");
        int ret = connect(client, (struct sockaddr *)&server_address, sizeof(server_address));//подключаемся к серверу
                                                                                              //0 - подключились, -1 - не подключились
        int q = 0;
        while(ret == -1)
        {
            ret = connect(client, (struct sockaddr *)&server_address, sizeof(server_address));
            q++;
            if(q > 100000)
            {
                printf("ERROR: Server is not responding\n");
                return 0;

            }   
                
        }

        recv(client, buffer, BUFFER_SIZE, 0);//получаем подтверждения подключения от сервера
        printf("LOG: Connection established.\n");
        
        while(1)
        {
            printf("ME: ");
            
            fgets(buffer, 1024,stdin);//вводим выражение которое нужно вычислить
            if(buffer[0] == '#')//если ввели # то завершаем работу
            {
                send(client, buffer, BUFFER_SIZE, 0);
                return 0;   
            }
            send(client, buffer, BUFFER_SIZE, 0);//отправляем запрос серверу 
            recv(client, buffer, BUFFER_SIZE, 0);//получаем от сервера подтверждение о получении запроса

            printf("SERVER:\n");
            
            recv(client, buffer, BUFFER_SIZE, 0); //получаем от сервера ответ на запрос 
            printf("%s\n", buffer);
            strcpy(buffer, "OK");
            send(client, buffer, BUFFER_SIZE, 0); //отправляем серверу подтверждение о получении ответа 
        }
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

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

int is_num(char *arr)
{
    int err = 0;
    for(int i = 0; i < strlen(arr); i++)
    {
        if(arr[i] < '0' || arr[i] > '9')
            return 0;
    }
    return 1;
}