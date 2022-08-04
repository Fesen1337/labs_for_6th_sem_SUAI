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
    int n;
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

        recv(client, buffer, BUFFER_SIZE, 0);
        printf("LOG: Connection established.\n");
        printf("Enter your name: ");
        scanf("%s", buffer);
        send(client, buffer, BUFFER_SIZE, 0);

        while(1)
        {
            char N[100];
            printf("ME:");
            scanf("%s", N);

            if(N[0] == '#')
            {
                printf("Disconnect\n");
                strcpy(buffer, "###");
                send(client, buffer, BUFFER_SIZE, 0);
                return 0;
            }

            if(is_num(N) == 0)
            {
                printf("LOCAL ERROR: Something wrong\n");
                continue; 
            }
            
            n = atoi(N);
            
            if(n < 0 || n > 12)
            {
                printf("LOCAL ERROR: Incorrect input\n");
            }else{

                itoa(n, buffer);
                send(client, buffer, BUFFER_SIZE, 0);
                recv(client, buffer, BUFFER_SIZE, 0);
                long long res;
                recv(client, buffer, BUFFER_SIZE, 0);
                res = atoi(buffer);
                strcpy(buffer, "OK");
                send(client, buffer, BUFFER_SIZE, 0);
                printf("SERVER: ");
                printf("%lld\n",res);
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