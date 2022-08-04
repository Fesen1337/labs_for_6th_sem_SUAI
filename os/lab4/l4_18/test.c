#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <time.h>

char* buf;

void itoa(int n, char s[]);
void reverse(char s[]);
void line_to_buffer(int line_id, int k);

int a[100][100];
void init(int n, int k);
void buffer_to_line(int line_id);


int main()
{
    int a;
    int b;
    char c;
    int err = 0;
    err += scanf("%d", &a);
    err = scanf("%d", &b);
    printf("%d",err);

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

void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* записываем знак */
        n = -n;          /* делаем n положительным числом */
    i = 0;
    do {       /* генерируем цифры в обратном порядке */
        s[i++] = n % 10 + '0';   /* берем следующую цифру */
    } while ((n /= 10) > 0);     /* удаляем */
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

void init(int n, int k)
{
    for(int i = 0; i < n; i++)
        for(int j = 0; j < k; j++)
            a[i][j] = rand()%19 - 9;

}