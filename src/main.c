#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 100
#define MAX_BUCKET_LENGTH 3
#define D 3
#define d 3

typedef struct Bucket {
  int local_depth;
  int count;
  int keys[MAX_BUCKET_LENGTH];
} Bucket;

typedef struct Directory {
  char *id;
  int count;
  int initialized;
  Bucket **bucket;
} Directory;

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

      if (directories[i].bucket != NULL &&
          directories[i].bucket[0]->count != 0) {
        printf("  Buckets:\n");
        for (int j = 0; j < 1; j++) {
          if (directories[i].bucket[j] != NULL) {
            printf("    Bucket %d:\n", j);
            printf("      Local Depth: %d\n",
                   directories[i].bucket[j]->local_depth);
            printf("      Count: %d\n", directories[i].bucket[j]->count);
            printf("      Keys: ");
            for (int k = 0; k < directories[i].bucket[j]->count; k++) {
              printf("%d ", directories[i].bucket[j]->keys[k]);
            }
            printf("\n");
          }
        }
      } else {
        printf("  No buckets.\n");
      }
      printf("\n");
    }
  }
}

void insert(Directory directories[], char *memory_string, int int_key) {
      char *string_binary;
      string_binary = string_to_binary(memory_string);

      char *lsbs = get_lsbs_from_binary_string(string_binary, D);

      int initialized_directory;
      initialized_directory = 0;

      printf("Binary string: %s\n", string_binary);
      printf("LSBs (last %d bits): %s\n", D, lsbs);

      for (int i = 0; i < 4; i++) {
        if (directories[i].id != NULL && strcmp(directories[i].id, lsbs) == 0) {
          int free_position;
          free_position = directories[i].bucket[0]->count;
          directories[i].count++;
          directories[i].bucket[0]->keys[free_position] = int_key;
          directories[i].bucket[0]->count++;
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
          int free_position;
          free_position = directories[i].bucket[0]->count;
          directories[i].count++;
          directories[i].bucket[0]->keys[free_position] = int_key;
          directories[i].bucket[0]->count++;
          break;
        }
      }

      print_directory_array(directories, 4);
};

void find(Directory directories[], char *key, int **value) {
  *value = malloc(sizeof(int));

  if (*value == NULL) {
    perror("malloc failed");
    exit(1);
  }

  char *string_binary;
  string_binary = string_to_binary(key);
  char *lsbs = get_lsbs_from_binary_string(string_binary, D);
  int int_key = atoi(key);
  for (int i = 0; i < 4; i++) {
    if (directories[i].id != NULL && strcmp(directories[i].id, lsbs) == 0) {
      Directory current_directory;
      int key;
      for (int j = 0; j < MAX_BUCKET_LENGTH; j++) {
        Bucket *directory_bucket = directories[i].bucket[0];
        if (directory_bucket->keys[j] == int_key) {
          **value = directory_bucket->keys[j];
        }
      }
    }
  }
};

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

      directories[i].bucket = (Bucket **)malloc(1 * sizeof(Bucket *));

      directories[i].bucket[0] = (Bucket *)malloc(sizeof(Bucket));
      directories[i].bucket[0]->local_depth = 3;
      directories[i].bucket[0]->count = 0;
    }

    while (1) {
      buffer_file = fopen("disk.txt", "a");
      if (buffer_file == NULL) {
        printf("Failed to open file for writing.\n");
        break;
      }
      char action[49];
      char memory_string[MAX_STRING_LENGTH];

      printf("Enter string %d: ", string);
      scanf("%s", memory_string);
      printf("Enter a operation: ");
      scanf("%48s", action);

      int int_key = atoi(memory_string);

      if (strcmp(action, "find") == 0) {
        int *result = NULL;
        find(directories, memory_string, &result);
        if (*result == 0) {
          printf("Value not found.");
        } else {
          printf("The value is: %d", *result);
        }
        printf("\n");
        continue;
      }

      if (strcmp(action, "insert") == 0) {
        insert(directories, memory_string, int_key);
        continue;
      }

      string++;
    }
  }
  free(buffer);
  return 0;
}
