#include <sys/types.h> // системные вызовы
#include <unistd.h>
#include <stdio.h> // ввод вывод

int main() {

printf("UID = %d\nGID = %d \n", getuid(), getgid());

return 0;
}
