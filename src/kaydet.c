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

int main(int argc, char* argv)
{
    int i;
	char s[100] = {'\0'}; 
    i = read(3, s, 100); //pipe ile programdan gonderilen sonucu s değişkenine atar

    FILE* fp;
    int j;
    fp = fopen ("sonuc.txt","a"); //sonuc.txt yoksa oluştururulur varsa açılır
    
    fputs(s, fp); //pipetan okunan veri dosyaya yazılır
    
    fclose (fp);
    printf("Sonucunuz kaydedilmiştir.");
    return 0;
}



