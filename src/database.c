#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
//parametre olarak verilen metni boşluk ile böler
//ve parametre olarak verilen diğer diziye atar
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
//parametre olarak verilen metni eşittir ile böler
//ve parametre olarak verilen diğer diziye atar
void EsittireGoreBol(char* bolunecek, char** bolunmus) 
{ 
    int i; 
    for (i = 0; i < 100; i++) { 
        bolunmus[i] = strsep(&bolunecek, "="); 
        if (bolunmus[i] == NULL) 
            break; 
        if (strlen(bolunmus[i]) == 0) 
            i--; 
    } 
}

int main(int argc, char* argv)
{
    char* girdi[100]; //programdan pipe ile gönderilen değeri tutar
    char* bolunmusGirdi[100]; //girdinin bosluk ile bölünmüş hali
    char* istenen[100] = {'\0'}; //istenilen eşitlik
    //pipe oluşunu
    int fd;
    char* myfifo = "/tmp/myfifo"; 
    mkfifo(myfifo, 0666); 

    char tumveriler[1000] = {'\0'}; //sorgudan dönen tüm veriler buraya yazılır
    
    while(1)
    {
        int isnal = 0;
        for(int i = 0;i<1000;i++)
        {
            tumveriler[i] = '\0'; 
        }


        //pipe okuma işlemi
        fd = open(myfifo,O_RDONLY); 
        read(fd, girdi, 100); 
        printf("sorgu: %s\n", girdi); 
        close(fd); 
        
        BoslugaGoreBol(girdi, bolunmusGirdi); //girilen sorgu boşluğa göre bölünür

        EsittireGoreBol(bolunmusGirdi[5], istenen); //sorgunun en son kısmındaki eşitliği böler  


        char str[150]; //dosya okuma işleminde her satır bu değişkende tutulur
        //okunacak dosyayı açar
        FILE* fp;
        fp = fopen(bolunmusGirdi[3], "r");
        
        char bosluk[10] = " "; //* sorgusu için verilerin arasına koyulur
        char altsatir[] = "\n";

        //pipe oluşumu 
        int fd;
        char* myfifo = "/tmp/myfifo"; 
        mkfifo(myfifo, 0666); 
        fd = open(myfifo, O_WRONLY); 
        
        //doysa okuma işlemi
        while (fgets(str,150, fp)) {

            char* parsed[100];  //okunan satırın verilerini ayrı ayrı tutar
            BoslugaGoreBol(str, parsed); //okunan satırı isim ve numara olarak ayırır

            //arama numaraya göre yapılıyorsa kontrolü için gerekli kod
            char *a = parsed[1];
            int b = 0;
            char yeni[strlen(parsed[1])-2];
            while(*a != '\0'){
                yeni[b] = *a;
                b++;
                a++;
            }
            yeni[strlen(parsed[1])-2] = '\0';
            

            if( strcmp(istenen[0],"ad") == 0) //arama ada göre yapılıyorsa 
            {
                if(strcmp(parsed[0], istenen[1]) == 0) //dosyadan okunan kelime ile aranan kelimeyi kıyaslar
                {
                    isnal=1;
                    if(strcmp(bolunmusGirdi[1],"*") == 0) //bulunan verinin hangi kısmını istendiğinin kontrolü
                    {
                        strcat(tumveriler,parsed[0]);
                        strcat(tumveriler,bosluk);
                        strcat(tumveriler,yeni);
                        strcat(tumveriler,altsatir);
                    }
                    else if(strcmp(bolunmusGirdi[1],"ad") == 0)
                    {
                        strcat(tumveriler,parsed[0]);
                        strcat(tumveriler,altsatir);
                    }
                    else if(strcmp(bolunmusGirdi[1],"number") == 0)
                    {
                        strcat(tumveriler,yeni);
                        strcat(tumveriler,altsatir);
                    }
                }
            }
            else if( strcmp(istenen[0],"number") == 0) //arama numaraya göre yapılıyorsa 
            {
                if(strcmp(yeni, istenen[1]) == 0)//dosyadan okunan kelime ile aranan kelimeyi kıyaslar
                {
                    isnal=1;
                    if(strcmp(bolunmusGirdi[1],"*") == 0)//bulunan verinin hangi kısmını istendiğinin kontrolü
                    {
                        strcat(tumveriler,parsed[0]);
                        strcat(tumveriler,bosluk);
                        strcat(tumveriler,yeni);
                        strcat(tumveriler,altsatir);
                    }
                    else if(strcmp(bolunmusGirdi[1],"ad") == 0)
                    {
                        strcat(tumveriler,parsed[0]);
                        strcat(tumveriler,altsatir);
                    }
                    else if(strcmp(bolunmusGirdi[1],"number") == 0)
                    {
                        strcat(tumveriler,yeni);
                        strcat(tumveriler,altsatir);
                    }
                }
            }  
        }
        
        if(isnal != 1)
        {
        	strcat(tumveriler,"null");
        }
         
        
        write(fd, tumveriler, strlen(tumveriler)+1); //sorgu sonucu pipe a yazılır
        close(fd);    
        fclose(fp);
    }
    return 0;

}
