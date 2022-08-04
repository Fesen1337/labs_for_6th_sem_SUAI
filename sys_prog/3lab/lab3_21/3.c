#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int x = 0;
int y = 0;
int id1 = -2, id2 = -2, id3 = -2;
void handle_sigusr1(int sig)//разбудить 
{
    if(id2 != 0 && id3 != 0)
        x++;
    else
        y++;
}

void handle_sigusr2(int sig)//попросить разбудить
{
    if(id3 != 0)
        kill(id3, SIGUSR1);
}
// это нужно чтобы была возможность достучаться от 2го к третьему.
//потомки ничего не знают друг о друге поэтому для организации общения между ними сигналы переправляются через родителя

int main(int argc, char* argv[])
{
    int n = 10;
    if(argc == 2)
        n = atoi(argv[1]);
    char filename[10];
    strcpy(filename, "1.txt");
    
    int n_symb = 5;
    char symb[] = {'a', 'b', 'c', 'd', 'e'};
    
    FILE *f = fopen(filename, "w");

    for(int i = 0; i < n; i++)//пишем строки в файл
    {
        for(int j = 0; j < 10; j++)
        {
            fprintf(f, "%c", symb[i%5]);
        }
        fprintf(f,"\n");
    }
    fclose(f);

    struct sigaction sa = { 0 };//описываем сигнал 1
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = &handle_sigusr1;//функция которая будет выполняться при поимке сигнала 1
    sigaction(SIGUSR1, &sa, NULL);//ловим сигналы 1

    struct sigaction sa2 = { 0 };//описываем сигнал 2
    sa2.sa_flags = SA_RESTART;
    sa2.sa_handler = &handle_sigusr2;//функция которая будет выполняться при поимке сигнала 2
    sigaction(SIGUSR2, &sa2, NULL);//ловим сигналы 2

    id1 = getpid();

    id2 = fork();//создаем 2й процесс
    if(id2 == 0)//обработка второго процесса
    {
        f = fopen(filename, "r");
        char str[20];
        char arr[100][100];
        int id = 0;
        while(fgets(str, 20, f) != NULL)//читаем файл
        {
            strcpy(arr[id], str);//содержимое запишем в массив arr
            id++;
        }
        fclose(f);

        int err = 0;
        usleep(200);
        kill(getppid(), SIGUSR2);//через сигнал просим главного разбудить 3й
        for(int i = 0; i < id; i++)//перебираем строки
        {
            for(int j = 0; j < id; j++)//для каждой строки ищем дубликат
            {
                err = 0;  
                if(i != j)
                {
                    for(int z = 0; z < 10; z++)
                    {
                        if(arr[i][z] == arr[j][z])
                        {
                            //printf("%d %d\n", i, j);
                            err++;
                        }
                            
                    }
                }
                if(err > 0)//если нашли дубликат
                {
                    //printf("%s", arr[i]);
                    printf("%d string is not unic\n", i+1);
                    kill(getppid(), SIGUSR1);//отправим сигнал об этом главному процессу
                    usleep(2000);
                }
            }
            
        }
        return 0;
    }else{
        id3 = fork();
        if(id3 == 0)//обработка 3го процесса
        {
            //читаем файл и пишем его содержимое в пронумерованном виде в файл buf
            
            
            usleep(200);
            while(y == 0){}
            FILE *f = fopen(filename, "r");
            char str[20];
            FILE *buf;
            buf = fopen("buf.txt", "w");
            int id = 1;
            while(fgets(str, 20, f) != NULL)
            {
                fprintf(buf, "%d)%s",id, str);
                id++;
            }
            fclose(f);
            fclose(buf);
            //стираем исходный файл и пишем в него содержимое файла buf
            f = fopen(filename, "w");
            buf = fopen("buf.txt", "r");

            while(fgets(str, 1024, buf) != NULL)
            {
                fprintf(f, "%s",str);
            }
            fclose(buf);
            fclose(f);
            remove("buf.txt");//удаляем файл buf
            return 0;
        }else{//обработка главного процесса (1го)

            while(wait(NULL) != -1);//ждем завершения работы потомков
            printf("Count not unic strings:%d\n", x);//пишем рез-т
            
        }
    }
    return 0;
}
