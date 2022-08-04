#include "menu.h"

void print_help()
{
    printf("help - manual\n");
    printf("format - format file system\n");
    printf("ls - view all files\n");
    printf("create [filename] - for create file with filename <= 12char\n");
    printf("rm [filename] - for remove file with filename <= 12char\n");
    printf("read [filename] - for read file with filename <= 12char\n");
    printf("write [filename] [buffer] - for write buffer to file with filename <= 12char\n");
    printf("add [filename] [buffer] - for add buffer to file with filename <= 12char\n");    
    printf("exit - for exit\n");
}

void split(char *str, char *com, char *filename, char *buf)
{
    com[0] = '\0';
    filename[0] = '\0';
    buf[0] = '\0';

    int flag = 0;
    int id = 0;
    for(int i = 0; i < strlen(str); i++)
    {
        if(str[i] == ' ' || str[i] == '\n')
        {
            if(flag == 0)
            {
                com[id] = '\0';
                id = 0;
            }else if(flag == 1)
            {
                filename[id] = '\0';
                id = 0;
            }else
            {
                if(str[i] != '\n')
                    buf[id++] = ' ';

            }
            if(str[i] == '\n')
                break;
            flag++;
            
        }else{
            if(flag == 0)
            {
                com[id++] = str[i];
            }else if(flag == 1)
            {
                filename[id++] = str[i];
            }else
            {
                buf[id++] = str[i];
            }
            
        }   
    }
    buf[id] = '\0';
    
}

void action()
{
    size_t s = 4096;
    char buffer[s];
    char com[15], filename[20], buf[1024];
    while(1)
    {
        printf("fs>>");
        fgets(buffer, s, stdin);
        split(buffer, com, filename, buf);
        if(strcmp(com, "exit") == 0)
        {
            break;
        }else if(strcmp(com, "help") == 0)
        {   
            print_help();
        }else if(strcmp(com, "format") == 0)
        {
            format();
            printf("LOG: format successfull\n");
        }else if(strcmp(com, "ls") == 0)
        {
            get_file_list();
        }else if(strcmp(com, "create") == 0)
        {
            if(strlen(filename) == 0)
            {
                printf("ERROR: Empty filename\n");
            }else{
                create_file(filename);
            }
        }else if(strcmp(com, "rm") == 0)
        {
            rm_file(filename);
        }else if(strcmp(com, "read") == 0)
        {

            read_file(filename);
            printf("\n");
        }else if(strcmp(com, "write") == 0)
        {
            write_to_file(filename, buf);
        }else if(strcmp(com, "add") == 0)
        {
            add_to_file(filename, buf);
        }else{
            print_help();
        }
    }

}
