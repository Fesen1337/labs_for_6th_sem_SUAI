#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define DEFAULT_PORT 1601

char* buffer;
size_t buffer_size = 1024;

void init(int n, int k);
void print(int n, int k);
void itoa(int n, char s[]);
void reverse(char s[]);
int is_num(char *arr);

int main(int argc, char* argv[])
{
    int n, b, q;
    while(1)
    {

        int client = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in server_address;
        buffer = (char *)malloc(buffer_size * sizeof(char));
        if(client < 0)
        {
            printf("ERROR: Establishing socket error.\n");
            exit(0);
        }
        server_address.sin_port = htons(DEFAULT_PORT);
        server_address.sin_family = AF_INET;
        inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);
        printf("LOG: Client socket created\n");

        int ret = connect(client, (struct sockaddr *)&server_address, sizeof(server_address));
        int x = 0;
        while(ret == -1)
        {
            ret = connect(client, (struct sockaddr *)&server_address, sizeof(server_address));
            x++;
            if(x > 100000)
            {
                printf("ERROR: Server is not responding\n");
                return 0;

            }   
                
        }

        recv(client, buffer, BUFFER_SIZE, 0);
        printf("LOG: Connection established.\n");

        strcpy(buffer, "Connection established");
        send(client, buffer, BUFFER_SIZE, 0);

        while(1)
        {
            char N[100];
            char B[100];
            char Q[100];

            printf("ME: enter n:");
            scanf("%s", N);
            printf("ME: enter b:");
            scanf("%s", B);
            printf("ME: enter q:");
            scanf("%s", Q);
            if(N[0] == '#')
            {
                printf("Disconnect\n");
                strcpy(buffer, "###");
                send(client, buffer, BUFFER_SIZE, 0);
                return 0;
            }

            if(is_num(N) == 0 || is_num(B) == 0 || is_num(Q) == 0)
            {
                printf("LOCAL ERROR: Something wrong\n");
                continue; 
            }
            
            n = atoi(N);
            b = atoi(B);
            q = atoi(Q);

            if(n < 0)
            {
                printf("LOCAL ERROR: Incorrect input\n");
            }else{
                itoa(n, buffer);
                send(client, buffer, BUFFER_SIZE, 0);
                recv(client, buffer, BUFFER_SIZE, 0);
                
                itoa(b, buffer);
                send(client, buffer, BUFFER_SIZE, 0);
                recv(client, buffer, BUFFER_SIZE, 0);

                itoa(q, buffer);
                send(client, buffer, BUFFER_SIZE, 0);
                recv(client, buffer, BUFFER_SIZE, 0);

                recv(client, buffer, BUFFER_SIZE, 0);
                n = atoi(buffer);
                strcpy(buffer, "OK");
                send(client, buffer, BUFFER_SIZE, 0);
                printf("SERVER: ");
                for(int i = 0; i < n; i++)
                {
                    recv(client, buffer, BUFFER_SIZE, 0);
                    printf("%s ", buffer);
                    strcpy(buffer, "OK");
                    send(client, buffer, BUFFER_SIZE, 0);
                }
                printf("\n");
            }
            
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