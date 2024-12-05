#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* load_entire_file(char* filename, size_t* plen) {
  char* text = NULL;
  size_t len = 0;
  FILE* f = fopen(filename, "rb");

  if (f == 0)
    return NULL;

  fseek(f, 0, SEEK_END);
  len = (size_t)ftell(f);

  if (plen)
    *plen = len;

  text = (char*)malloc(len + 1);

  if (text == NULL)
    return NULL;

  fseek(f, 0, SEEK_SET);
  fread(text, 1, len, f);
  fclose(f);
  text[len] = '\0';

  return text;
}

static size_t count_lines_in_file(const char* lines) {
  size_t num_lines = 0;
  while (*lines++) {
    if (*lines == '\n' || *lines == '\0') {
      ++num_lines;
    }
  }
  return num_lines;
}

static int compare(const void* a, const void* b) {
  return (*(const int*)a - *(const int*)b);
}

int main(void) {
  size_t file_size = 0;
  char* file = load_entire_file("input.txt", &file_size);
  if (file == NULL) {
    printf("Failed to load the file input.txt");
    return 0;
  }

  size_t num_lines = count_lines_in_file(file);
  int* first_col = (int*)malloc(num_lines * sizeof(int));
  int* second_col = (int*)malloc(num_lines * sizeof(int));

  char* current = file;
  size_t i = 0;
  while (i < num_lines) {
    first_col[i] = atoi(current);

    while (*current != ' ')
      current++;

    second_col[i] = atoi(current);

    while (*current != '\n')
      current++;

    current++;
    i++;
  }

  // sort the aray so we can early out. See "Fast early out"
  qsort(second_col, num_lines, sizeof(int), compare);

  for (i = 0; i < num_lines; i++) {
    int left_current = first_col[i];
    int count = 0;
    for (size_t j = 0; j < num_lines; j++) {
      int right_current = second_col[j];

      // Fast early out
      if (right_current > left_current) {
        break;
      } else if (second_col[j] == left_current) {
        ++count;
      }
    }
    first_col[i] *= count;
  }

  int total = 0;
  for (i = 0; i < num_lines; i++) {
    total += first_col[i];
  }

  printf("total=%u", total);

  free(first_col);
  free(second_col);
  free(file);

  return 0;
}
