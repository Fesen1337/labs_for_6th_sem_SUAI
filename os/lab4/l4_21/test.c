#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int prior(char c)
{
    if(c == '+' || c == '-')
        return 2;
    if(c == '*' || c == '/')
        return 1;
    return 3;
}

void enter()
{
    FILE *f = fopen("1.txt", "w");
    char buf[100];
    int sp = -1;
    char c;
    while(1)
    {
        scanf("%c", &c);
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
                
        }
    }   
    
}


int main()
{

    int stack[256];
    char buf[256];
    int sp = 0;
    FILE *f = fopen("1.txt", "r");
    enter();
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
               printf("%d\n", stack[sp-2]);
               sp--;
               break;
            case '-':
               stack[sp-2] = stack[sp-2] - stack[sp-1];
               printf("%d\n", stack[sp-2]);
               sp--;
               break;
            case '*':
               stack[sp-2] = stack[sp-1] * stack[sp-2];
               printf("%d\n", stack[sp-2]);
               sp--;
               break;
            case '/':
               stack[sp-2] = stack[sp-2] / stack[sp-1];
               printf("%d\n", stack[sp-2]);
               sp--;
               break;
            default:
               stack[sp++] = atoi(buf);
        }
    }
    fclose(f);
    printf("Result = %d\n",stack[sp-1]);
    return 0;
}