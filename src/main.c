#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 100
#define D 2
#define d 2

char *string_to_binary(char *s) {
  if (s == NULL)
    return 0;
  size_t len = strlen(s);
  char *binary = malloc(len * 8 + 1);

  binary[0] = '\0';
  for (size_t i = 0; i < len; ++i) {
    char ch = s[i];
    for (int j = 7; j >= 0; --j) {
      if (ch & (1 << j)) {
        strcat(binary, "1");
      } else {
        strcat(binary, "0");
      }
    }
  }
  return binary;
}

int main() {
  FILE *buffer_file;
  char **buffer;
  int allocate_indexes = 0;
  int buffer_size = 10;
  int string = 1;

  buffer = malloc(sizeof(char *) * buffer_size);

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
        char *string_hash;
        string_hash = string_to_binary(memory_string);
        fprintf(buffer_file, "%s", string_hash);
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
