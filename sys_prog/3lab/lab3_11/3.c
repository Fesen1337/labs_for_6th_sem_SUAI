#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define PROCESS_NUM 3
/*
Процесс 1 открывает файл и после этого порождает потомка 2, который в 
cвою очередь порождает потомка 3. Процесс 2 пишет N байт в общий файл, 
посылает сообщение процессу 3, который тоже пишет N байт в файл и посылает сигнал процессу 1, 
который считывает данные из файла и выводит их на экран.
*/


int main(int argc, char* argv[])
{
    int n = 10;//значение количества знаков по умолчанию
    if(argc == 2)
        n = atoi(argv[1]);// либо можно задать при запуске программы
    char nums[] = {'0','1','2','3','4','5','6','7','8','9'};//символы которые будет выводить второй процесс
    char nums2[] = {'a','b','c','d','e','f','g','h','i','j'};//символы которые будет выводить третий процесс
    int main_id;
    FILE *f;
    int id = fork();//запуск второго процесса (1й - это главный)
    if(id == 0)// обработка второго процесса
    {
        kill(getppid(), SIGSTOP);// посылаем сигнал первому процессу, чтобы он подождал, пока 2й и 3й запишут в файл
        main_id = getppid();//на всякий случай еще раз уточняем id главного (1го) процесса.

        int id2 = fork();//запускаем 3й процесс

        if(id2 == 0)//обработка 3го процесса
        {
            f = fopen("1.txt", "a");
            for(int i = 0; i < n; i++)//запись в файл n байт
            {
                fprintf(f, "%c", nums2[i%10]);
            }
            fclose(f);
            kill(main_id, SIGCONT);//3й процесс будет записывать последним, поэтому как он закончит
                                   //будим главный (1й)процесс, чтобы он продолжил работу.
        }else{//обработка 2го процесса.

            kill(id2, SIGSTOP);//перед работой 2го процесса посылаем сигнал 3му процессу, чтобы он подождал пока 2й пишет
            f = fopen("1.txt", "w");
            for(int i = 0; i < n; i++)//запись в файл n байт
            {
                fprintf(f, "%c", nums[i%10]);
            }
            fclose(f);
            kill(id2, SIGCONT);//будим 3й процесс, чтобы он начал запись байт в файл.
        }
    }else{//обработка главного (1го) процесса
        
        //1й процесс благодаря выставленным сигналам начнет работать самым последним
        char buf[1024];
        f = fopen("1.txt", "r");
        fscanf(f, "%s", buf);//он выведет записанную в файл строку.
        printf("\n%s\n", buf);
        fclose(f);


    }
    return 0;
}