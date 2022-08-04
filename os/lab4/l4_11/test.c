#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <time.h>

const int INF = 99999;

char* buf;


void itoa(int n, char s[]);
void reverse(char s[]);
void line_to_buffer(int k);

int a[100];

int main()
{
    buf = (char *)malloc(1024 * sizeof(char));
    for(int i = 0; i < 10; i++)
    {
        scanf("%d", &a[i]);
    }
    line_to_buffer(10);
    printf("%s", buf);
}   

void line_to_buffer(int k)
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

/*
void sort(int *arr, int n)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = i; j < n; j++)
        {
            if(arr[i] > arr[j])
            {
                swap(&arr[i], &arr[j]);
            }
        }
    }
}

void swap(int* a1, int* a2)
{
    int c = *a1;
    *a1 = *a2;
    *a2 = c;
}

void buffer_to_line(int line_id)
{
    char b[10];
    int flag = 0;
    int cur = 0;
    for(int i = 0; i < strlen(buf); i++)
    {
        if(buf[i] == ' ')
        {
            b[flag] = '\0';
            a[line_id][cur] = atoi(b);
            cur++;
            flag = 0;
        }else{
            b[flag] = buf[i];
            flag++;
        }
    }
}

void line_to_buffer(int line_id, int k)
{
    char num[4];
    char b[256];
    buf[0] = '\0';
    for(int i = 0; i < k; i++)
    {
        strcpy(b, buf);
        itoa(a[line_id][i], num);
        snprintf(buf, 1024, "%s%s ", b, num);
    }
}



void init(int n, int k)
{
    for(int i = 0; i < n; i++)
        for(int j = 0; j < k; j++)
            a[i][j] = rand()%19 - 9;

}*/