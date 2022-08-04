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

int a[100][100];
void init(int n, int k);
void print(int n, int k);
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
        
        while(1)
        {
            printf("ME: ");
            
            scanf("%s", N);
            scanf("%s", K);
            if(is_num(N) == 1)
                n = atoi(N);
            if(is_num(K) == 1)
                k = atoi(K);

            if(is_num(N) == 0 || is_num(K) == 0)
            {
                printf("ERROR: Incorrect data, try again\n");
                   
            }else if(n >= 100 || k >= 100 || n <= 0 || k <= 0)
            {
                if(n == 999 && k == 999)
                {
                    strcpy(buffer, "#goodbye");
                    send(client, buffer, BUFFER_SIZE, 0);
                    return 0;
                }
                printf("ERROR: Incorrect data, try again\n");
            }else{
                init(n, k);
                print(n, k);

                itoa(n + 1, buffer);

                send(client, buffer, BUFFER_SIZE, 0);
                recv(client, buffer, BUFFER_SIZE, 0);
                
                for(int i = 0; i <= n; i++)
                {   
                    if(i == 0)
                    {
                        itoa(k, buffer);
                        send(client, buffer, BUFFER_SIZE, 0);
                        recv(client, buffer, BUFFER_SIZE, 0);
                    }else{
                        line_to_buffer(i-1, k);
                        send(client, buffer, BUFFER_SIZE, 0);
                        recv(client, buffer, BUFFER_SIZE, 0);
                    }
                }

                recv(client, buffer, BUFFER_SIZE, 0);
                n = atoi(buffer);
                strcpy(buffer, "OK");
                send(client, buffer, BUFFER_SIZE, 0);
                printf("SERVER:\n");
                for(int i = 0; i < n; i++)
                {
                    recv(client, buffer, BUFFER_SIZE, 0);
                    printf("%s\n", buffer);
                    strcpy(buffer, "OK");
                    send(client, buffer, BUFFER_SIZE, 0);
                }
            }

            
            
        }
    }


}


void line_to_buffer(int line_id, int k)
{
    char num[4];
    char b[256];
    buffer[0] = '\0';
    for(int i = 0; i < k; i++)
    {
        strcpy(b, buffer);
        itoa(a[line_id][i], num);
        snprintf(buffer, 1024, "%s%s ", b, num);
    }
}


void init(int n, int k)
{
    for(int i = 0; i < n; i++)
        for(int j = 0; j < k; j++)
            a[i][j] = rand()%19 - 9;

}

void print(int n, int k)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < k; j++)
            printf("%d ", a[i][j]);
        printf("\n");
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