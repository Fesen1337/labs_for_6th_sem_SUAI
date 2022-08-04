#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char *argv[])
{
    long int s_time;
    struct tm *m_time;
    char str[124];
    s_time = time(NULL);
    m_time = localtime(&s_time);
    
    strftime(str, 128, "Date: %x %X", m_time);
    FILE * f = fopen("1.txt", "a");
    fprintf(f, str);
    fprintf(f, " ");
    if(argc >= 2)
    {
        for(int i = 0; i < argc; i++)
        {
            fprintf(f, argv[i]);
            fprintf(f, " ");   
        }
    }
        
    fprintf(f, "\n");
    fclose(f);
}
