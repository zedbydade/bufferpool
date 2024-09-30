#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 100
#define D 3

typedef struct Directory {
  char *id;
  int count;
  int initialized;
} Directory;

void print_directory_array(Directory *directories, int size) {
  for (int i = 0; i < size; i++) {
    if (directories[i].initialized) {
      printf("Directory %d:\n", i);
      if (directories[i].id == NULL) {
        printf("  ID: (null)\n");
      } else {
        printf("  ID: %s\n", directories[i].id);
      }
      printf("  Count: %d\n", directories[i].count);
      printf("  Initialized: %d\n", directories[i].initialized);
      printf("\n");
    }
  }
}

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

int binary_string_to_int(char *binary_str) {
  int result = 0;
  while (*binary_str) {
    result = (result << 1) + (*binary_str - '0');
    binary_str++;
  }
  return result;
}

char *get_lsbs_from_binary_string(char *binary_str, int depth) {
  size_t len = strlen(binary_str);
  char *lsbs = malloc(depth + 1);

  strncpy(lsbs, binary_str + (len - depth), depth);
  lsbs[depth] = '\0';

  return lsbs;
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
    Directory directories[4];

    for (int i = 0; i < 4; i++) {
      directories[i].id = NULL;
      directories[i].count = 0;
      directories[i].initialized = 0;
    }

    while (1) {
      buffer_file = fopen("disk.txt", "a");
      if (buffer_file == NULL) {
        printf("Failed to open file for writing.\n");
        break;
      }
      char memory_string[MAX_STRING_LENGTH];

      printf("Enter string %d: ", string);
      scanf("%s", memory_string);

      char *string_binary;
      string_binary = string_to_binary(memory_string);

      char *lsbs = get_lsbs_from_binary_string(string_binary, D);

      int initialized_directory;
      initialized_directory = 0;

      printf("Binary string: %s\n", string_binary);
      printf("LSBs (last %d bits): %s\n", D, lsbs);

      for (int i = 0; i < 4; i++) {
        if (directories[i].id != NULL && strcmp(directories[i].id, lsbs) == 0) {
          directories[i].count++;
          initialized_directory = 1;
        }
      }

      if (initialized_directory == 0) {
        for (int i = 0; i < 4; i++) {
          if (directories[i].id != 0) {
            continue;
          }
          directories[i].id = strdup(lsbs);
          directories[i].initialized = 1;
          directories[i].count = 1;
          printf("Id: %s\n", directories[i].id);
          printf("Initialized: %d\n", directories[i].initialized);
          printf("Count: %d\n", directories[i].count);
          break;
        }
      }

      print_directory_array(directories, 4);

      string++;
    }
  }
  free(buffer);
  return 0;
}
