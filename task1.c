#include <sys/types.h> // системные вызовы
#include <unistd.h>
#include <stdio.h> // ввод вывод

int main() {

printf("PID = %d\nPPID = %d \n", getpid(), getppid());

return 0;
}
