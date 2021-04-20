#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int add (int semid, struct sembuf *buf){
    buf->sem_op = 2;
    buf->sem_flg = 0;
    buf->sem_num = 0;
    return semop(semid, buf, 1);
}

int delete (int semid, struct sembuf *buf){
    buf->sem_op = -1;
    buf->sem_flg = 0;
    buf->sem_num = 0;
    return semop(semid, buf, 1);
}

int zero (int semid, struct sembuf *buf){
    buf->sem_op = 0;
    buf->sem_flg = 0;
    buf->sem_num = 0;
    return semop(semid, buf, 1);
}


int main()
{
    int fd[2], result;
    size_t size;
    char resstring[14];
    int semid;
    key_t key;
    struct sembuf mybuf;
    int tmp;
    int counter = 0;
    
    if (pipe(fd) < 0) {
        exit(-1);
    }
    
    semctl(semid, 0, IPC_RMID, 0);
    
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        exit(-1);
    }

    printf("Enter number: ");
    scanf("%d", &tmp);
    result = fork();
    
    if (result < 0) {
        printf("haha < 0");
        exit(-1);
    } else if (result > 0) {

        /* Parent process */
        
        // Родитель
        //      цикл пока не достигли числа повторений {
        //          пишем
        //          добавляем
        //          ждем
        //          читаем
        //          *по ходу дела что-то печатаем*
        // }

        while (counter < tmp) {
            size = write(fd[1], "Parent", 6);
            if (size != 6) {
                printf("haha not 6");
                exit(-1);
            }
            
            counter++;
            printf("Parent message\n");
            add(semid, &mybuf);
            zero(semid, &mybuf);
            size = read(fd[0], resstring, 6);
            if (size < 0) {
                printf("haha < 0");
                exit(-1);
            }

            printf("Parent just wrote message: %s\n\n", resstring);
        }
    } else {

        /* Child process */
        
        // Ребенок
        //      цикл пока не достигли числа повторений {
        //          декрементируем
        //          читаем
        //          пишем
        //          декрементируем
        //          *по ходу дела что-то печатаем*
        // }

        while (counter < tmp) {
            delete(semid, &mybuf);
            size = read(fd[0], resstring, 6);
            if (size < 0) {
                printf("haha < 0");
                exit(-1);
            }
            
            printf("Child message: %s\n", resstring);
            size = write(fd[1], "Child", 5);
            if (size < 0) {
                printf("haha < 0");
                exit(-1);
            }
            counter += 1;

            printf("Child just wrote message\n\n");
            delete(semid, &mybuf);
        }
    }
    return 0;
}
