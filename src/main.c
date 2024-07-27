#include <stdio.h>
#include <stdlib.h>

int main() {
  int *buffer;
  int allocate_indexes = 0;

  buffer = malloc(sizeof(int) * 10);

  if (!buffer) {
    printf("Failed to alocate buffer");
  } else {
    while (1) {
      int n;
      int free_memory = sizeof(*buffer) - allocate_indexes;
      printf("The array free space is %d \n", free_memory);
      printf("Number of indexes: ");
      scanf("%d", &n);
      if (n > free_memory) {
        printf("It is nos possible to allocate that amount of memory \n");
        continue;
      }
      allocate_indexes += n;
    }
  }
}
