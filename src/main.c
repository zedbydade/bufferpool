#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 100

unsigned long djb2(const char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash;
}

int main() {
  FILE *buffer_file;
  char **buffer;
  int allocate_indexes = 0;
  int buffer_size = 10;
  int string = 1;

  buffer = malloc(sizeof(char*) * buffer_size);

  if (!buffer) {
    printf("Failed to allocate buffer");
  } else {
    while (1) {
      int free_memory = buffer_size - allocate_indexes;
      if (allocate_indexes >= buffer_size) {
        buffer_file = fopen("disk.txt", "a");
        if (buffer_file == NULL) {
          printf("Failed to open file for writing.\n");
          break;
        }
        char memory_string[MAX_STRING_LENGTH];
        printf("Enter string %d: ", string);
        scanf("%s", memory_string);
        unsigned long string_hash = djb2(memory_string); 
        fprintf(buffer_file, "%ld", string_hash);
        fputc('\n', buffer_file);
        fclose(buffer_file);
        string++;
        continue;
      }
      buffer[allocate_indexes] = malloc(MAX_STRING_LENGTH);
      printf("Enter string %d: ", string);
      scanf("%s", buffer[allocate_indexes]);

      allocate_indexes++;
      string++;
    }
  }
  free(buffer);
  return 0;
}
