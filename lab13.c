#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main(void){
    int count = 0;
    char pathname[] = "a";
    char currentPathName[1024];
    char previousPathName[1024];
    
    int fd = open(pathname,O_RDWR|O_CREAT,0666);
    if (fd < 0 || close(fd) != 0){
        printf("Can\'t create or close file\n");
    }

    sprintf(currentPathName, "a%d", count);
    symlink(pathname, currentPathName);
    
    while((fd = open(currentPathName,O_RDONLY, 0666)) >= 0){
        count++;
        strncpy(previousPathName, currentPathName, sizeof(previousPathName));
        sprintf(currentPathName, "a%d", count);
        symlink(previousPathName,currentPathName);

        if(close(fd)< 0) {
            printf("Cant close file\n");
            exit(-1);
        }
    }

    printf("%d\n", count);

    return 0;
}