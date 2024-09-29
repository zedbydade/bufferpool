#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE* buffer_file;
    int *buffer;
    int allocate_indexes = 0;

    buffer = malloc(sizeof(int) * 10);

    if (!buffer) {
        printf("Failed to allocate buffer");
    } else {
        while (1) {
            int n;
            int free_memory = 10 - allocate_indexes;
            printf("The array free space is %d \n", free_memory);
            printf("Number of indexes: ");
            scanf("%d", &n);
            if (n > free_memory) {
                printf("It is not possible to allocate that amount of memory \n");
                buffer_file = fopen("buffer.file", "a");
                if (buffer_file == NULL) {
                    printf("Failed to open file for writing.\n");
                    break;
                }
                fprintf(buffer_file, "%d", n);
                fputc('\n', buffer_file);
                fclose(buffer_file);
                continue;
            }
            allocate_indexes += n;
        }
    }
    free(buffer);
    return 0;
}
