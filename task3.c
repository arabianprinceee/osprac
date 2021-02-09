#include <sys/types.h> // системные вызовы
#include <unistd.h>
#include <stdio.h> // ввод вывод

int main(int argc, char *argv[], char *envp[]) {

	printf("\n-------ARGV:\n\n");

    for (int i = 0; i < argc - 1; ++i) {
        printf("I-th argv: %s\n", argv[i]);
    }

    printf("\n-------ENVP:\n\n");
    while(*envp) {
    	printf("%s\n", *envp++);
    }

	return 0;
}
