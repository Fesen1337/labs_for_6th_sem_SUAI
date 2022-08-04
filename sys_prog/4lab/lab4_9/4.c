#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>

struct mArgs
{
    int thread_num;
};
pthread_mutex_t mutex_log;
struct logMes
{
    int thread_nums;
    char mes[1024];
    long int s_time;
    struct tm *m_time;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

char links[7][500];
char names[7][100];

int fact_res[7];
int res = 0;

void* prod(void*);
void write_log(void* log_mes);
void init();

int main(int argc, char* argv[])
{
    pthread_mutex_init(&mutex_log, NULL);
    printf("Download started\n");

    struct mArgs margs[7];
    int i;
    pthread_t t[7];
    init();
    for(i = 0; i < 7; i++)
    {
        margs[i].thread_num = i;
        pthread_create(&t[i], NULL, &prod, &margs[i]);
    }
    for(i = 0; i < 7; i++)
    {
        pthread_join(t[i], NULL);
    }
    printf("Download ended\n");
    pthread_mutex_destroy(&mutex_log);
}

void init()
{
    strcpy(links[0], "https://upload.wikimedia.org/wikipedia/commons/e/e3/Kheops-Pyramid.jpg");
    strcpy(links[1], "https://upload.wikimedia.org/wikipedia/commons/a/ae/Hanging_Gardens_of_Babylon.jpg");
    strcpy(links[2], "https://upload.wikimedia.org/wikipedia/commons/6/66/Le_Jupiter_Olympien_ou_l'art_de_la_sculpture_antique.jpg");
    strcpy(links[3], "https://upload.wikimedia.org/wikipedia/commons/1/1d/Miniaturk_009.jpg");
    strcpy(links[4], "https://upload.wikimedia.org/wikipedia/commons/4/4f/Mausoleum_at_Halicarnassus_by_Ferdinand_Knab_(1886)_cropped.png");
    strcpy(links[5], "https://upload.wikimedia.org/wikipedia/commons/thumb/5/5f/Colosse_de_Rhodes_(Barclay).jpg/800px-Colosse_de_Rhodes_(Barclay).jpg");
    strcpy(links[6], "https://upload.wikimedia.org/wikipedia/commons/thumb/2/22/Lighthouse_-_Thiersch.png/1280px-Lighthouse_-_Thiersch.png");
    
    strcpy(names[0], "./Great_Pyranid_of_Giza.jpg");
    strcpy(names[1], "./Hanging_Gardens_of_Babylon.jpg");
    strcpy(names[2], "./Statue_of_Zeus_at_Olympia.jpg");
    strcpy(names[3], "./Temple_of_Artemis.jpg");
    strcpy(names[4], "./Mausoleum_at_Halicarnassus.png");
    strcpy(names[5], "./Colossus_of_Rhodes.jpg");
    strcpy(names[6], "./Lighthouse_of_Alexandria.png");
    
}

void* prod(void* args)
{
    struct mArgs *margs = (struct mArgs*)args;
    struct logMes l;
    l.thread_nums =  margs->thread_num;
    l.s_time = time(NULL);
    l.m_time = localtime(&l.s_time);
    snprintf(l.mes, 200, "Start downloading file %s", names[margs->thread_num]);
    write_log(&l);

    CURL *curl;
    FILE *fp;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(names[margs->thread_num],"wb");
        curl_easy_setopt(curl, CURLOPT_URL, links[margs->thread_num]);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    l.s_time = time(NULL);
    l.m_time = localtime(&l.s_time);
    snprintf(l.mes, 200, "End downloading file %s", names[margs->thread_num]);
    write_log(&l);
    return 0;
}

void write_log(void* log_mes)
{
    pthread_mutex_lock(&mutex_log);
    
    struct logMes *logm = (struct logMes*)log_mes;
    
    FILE* log;
    log = fopen("log.log", "a");

    char str[128];
    char buf[1024];
    
    strftime(str, 128, "Date: %x %X", logm->m_time);
    snprintf(buf, 2048, "%s: Thread #%d: %s \n", str, logm->thread_nums, logm->mes);

    fprintf(log, "%s", buf);
    fclose(log);

    pthread_mutex_unlock(&mutex_log);
}
