#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    
    int fd[2], result, fd1[2];
    size_t size;
    char string[] = "Hello, world!", string1[] = "World, hello?";
    char resstring[14], resstring1[14];
    pipe(fd);
    pipe(fd1);
    result = fork();
    
    if (result < 0) {
        
        printf("Can\'t fork child\n");
    }
    else if (result > 0) {
        
        close(fd[0]);
        write(fd[1], string, 14);
        printf("Parent is writting, pipe 1\n");
        close(fd[1]);
        printf("Parent ends\n");
    }
    else {
        
        close(fd1[0]);
        close(fd[1]);
        printf("Child is reading, pipe 1\n");
        read(fd[0], resstring, 14);
        printf("%s\n", resstring);
        close(fd[0]);
        printf("Child is writting, pipe 2\n");
        write(fd1[1], string1, 14);
        close(fd1[1]);
        printf("Child ends\n");
    }
    
    if (result > 0) {
        close(fd1[1]);
        printf("Parent is reading, pipe 2\n");
        read(fd1[0], resstring1, 14);
        printf("%s\n", resstring1);
        close(fd1[0]);
        printf("Parent ends\n");
    }
    
    return 0;
}
