#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    int fd[2];
    int pipeSize = 0, tmpSize = 1;
    char* string = malloc(sizeof(char));
    
    if (pipe(fd) >= 0) {
        fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);
        
        tmpSize = write(fd[1], string, 1);
        
        while (tmpSize == 1) {
            pipeSize += 1;
            tmpSize = write(fd[1], string, 1);
        }
        
        printf("pipe size: %d\n", pipeSize);
        
        free(string);
        
        if (close(fd[0]) < 0)
            printf("Can't close input stream\n");
        if (close(fd[1]) < 0)
            printf("Can't close output stream\n");
        
        return 0;
    } else {
        printf("Error with pipe!");
        free(string);
        exit(-1);
    }
}
