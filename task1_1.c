#include <sys/types.h> // системные вызовы
#include <unistd.h>
#include <stdio.h> // ввод вывод

int main() {
    
    pid_t pid, ppid;

    pid = getpid();
    
    pid = fork();

    if (pid == -1) { printf("Error"); }
    else if (pid == 0)
    {
        printf("Child: %d \n", getpid());
    }
    else {
        printf("Parent: %d \n", getppid());
    }
    
    return 0;
}
