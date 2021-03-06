#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<time.h>

#define BUF_SIZE 4096

void Split(char* str, char* sep, char** result);

void execution_pipe(const char* word);

int main(int argc, char *argv[]){
    int str_fd                = 0,
        num_w                 = 0;
    long int read_str         = 0;
    char str[BUF_SIZE]        = "";
    char sep[2]               = "\n";
    char* words[BUF_SIZE + 1] = {};

    str_fd = open(argv[1] ,O_RDONLY);

    // TODO: Вы прочитали из консоли строку и вывели её потом на экран. Зачем? Код должен работать для любого BUF_SIZE, согласны? Задайте его 1 вместо 4096. Работать не будет. 
    while ((read_str = read(str_fd, str, BUF_SIZE)) != 0)
                write(1, str, read_str);

    Split(str, sep, words);

    num_w = atoi(words[0]);
    for(int i = 1; i <= num_w; ++i){
        sleep(atoi(words[i + num_w]));
        execution_pipe(words[i]);
    }

    close(str_fd);
}

// TODO: переменная word не используется в ф-и. Точно код хоть на каком-то примере работает?
void execution_pipe(const char* word){
    unsigned time_out   = 5;
    long start_time = 0,
         now_time;
    int fd[2];
    pipe(fd);
    if (fork() == 0){
        close(fd[0]);
        start_time = time(NULL);
        write(fd[1], &start_time, 1);
        execlp("word",NULL);
    }
    // TODO: уверены, что код заходит вот в эту ветку? Едва ли в вашем случае fork хоть когда-нибудь вернет вам 1.
    if (fork() == 1){
        close(fd[1]);
        read(fd[0], &start_time, 1);
        now_time = time(NULL);
        while(now_time - start_time < 5){
            now_time = time(NULL);
        }
        // TODO: я не понимаю задумку - kill(fork(), 1) будет как-то странно работать
        kill(fork(), 1);
    }

}

void Split(char* str, char* sep, char** result){
    
    assert(str);
    assert(result);
    assert(sep);

    int i = 0;
    char* tmp = strtok(str, sep);
    while (tmp != NULL){ 
        result[i] = tmp;
        i++;
        tmp = strtok (NULL, sep); 
    }
}
