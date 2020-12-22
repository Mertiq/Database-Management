#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include<sys/wait.h> 
#include <unistd.h> 
#include<readline/readline.h> 
#include<readline/history.h> 
#include <stdlib.h>
#include <signal.h>

int girdiAl(char* girdi) 
{ 
    char* satir; 
    //readline ile kullanıcıdan veri alınır
    satir = readline("\nSorgu Giriniz: "); 
    if (strlen(satir) != 0) { 
        //alınan inputu parametre olarak verilen değişkene kopyalar
        strcpy(girdi, satir); 
        return 0; 
    } else { 
        return 1; 
    } 
} 

void BoslugaGoreBol(char* bolunecek, char** bolunmus) 
{ 
    int i; 
    for (i = 0; i < 100; i++) { 
        bolunmus[i] = strsep(&bolunecek, " "); 
        if (bolunmus[i] == NULL) 
            break; 
        if (strlen(bolunmus[i]) == 0) 
            i--; 
    } 
}

int kontrol(char* girdi)
{
    char temp[100] = {'\0'};
    strcat(temp,girdi);
    char* bolunmus[100];
    BoslugaGoreBol(temp,bolunmus);


    if(strcmp(bolunmus[0], "select") != 0)
        return 1;
    else if(strcmp(bolunmus[1], "*") != 0 && strcmp(bolunmus[1], "ad") != 0 && strcmp(bolunmus[1], "number") != 0)
        return 1;
    else if(strcmp(bolunmus[2], "from") != 0)
        return 1;
    else if(strcmp(bolunmus[4], "where") != 0)
        return 1;
    else
        return 0;
}

int main()
{
    char girdi[100] = {'\0'}; //sorguyu tutar
    char sonuc[100]; //sorgu sonunda dönen değeri tutar

    //named pipe oluşumu
    int fd;
    char* myfifo = "/tmp/myfifo"; 
    mkfifo(myfifo, 0666); 

    while(1)
    {
        girdiAl(girdi); //sorguyu alır

        if(kontrol(girdi))
        {
            printf("hatalı bir sorgu girdiniz");
            continue;
        }

        //pipe yazma işlemi
        fd = open(myfifo, O_WRONLY); 
        write(fd, girdi, strlen(girdi)+1); 
        close(fd); 

        //pipe okuma işlemi
        fd = open(myfifo,O_RDONLY); 
        read(fd, sonuc, 100);
        if(strcmp(sonuc,"null") == 0){
            printf("ilgili sonuç bulunamadı!");
            continue;
        }
        printf("SONUC: \n%s\n", sonuc); //okunan verileri konsola basar 
        close(fd); 
        
        //okunan verinin dosyaya kaydedilmesi
        char* x; 
        x = readline("\nsorgu sonucu kaydedilsin mi?e/h :  "); 
        if(x[0] == 'e')
        {
            //okunan verilerin kaydedilmesi
            //unnamed pipe
            int pipefd[2];
            if (pipe(pipefd) < 0) 
            {
                //pipe oluşmazsa hata verir ve program sonlanır
                perror("pipe");
                exit(1);
            }
            int c;
            int f = fork(); // fork oluşumu
            if (f == 0) 
            {
                //unnamed pipe ile yazma işlemi yapılır
                write(pipefd[1], sonuc, strlen(sonuc) +1);
                //fork ile kaydet programı çalıştırılır
                c = execv("kaydet", NULL);
                perror("");
                close(pipefd[1]);
                close(pipefd[0]);
            } else 
            {
                wait(&c);
                close(pipefd[1]);
                close(pipefd[0]);
            }
        }
        else if(x[0] == 'h')
        {
            //kaydedilmesi istenmezse tekrar sorgu istenilir
            printf("Sonucunuz kaydedilmemiştir");
            continue;
        }
        else{
            printf("yanlış bir seçim yaptınız\nsonucunuz kaydedilmeyecek\n");
        }
       
    }
    return 0;
}
