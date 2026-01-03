#include <jansson.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 40000

// Add total of all integer values contained in *root
// less those objects that contain "red"
int process_json(json_t *root) {
  int total = 0;
  const char *key;
  json_t *value;
  size_t index;

  switch (json_typeof(root)) {
  case JSON_OBJECT:
    json_object_foreach(root, key, value) {
      if (json_is_string(value)) {
        if (!strcmp(json_string_value(value), "red")) {
          return 0;
        }
      } else {
        total += process_json(value);
      }
    }
    break;
  case JSON_ARRAY:
    json_array_foreach(root, index, value) { total += process_json(value); }
    break;
  case JSON_INTEGER:
    return json_integer_value(root);
    break;
  // Must 'process' all json_types
  case JSON_STRING:
  case JSON_REAL:
  case JSON_TRUE:
  case JSON_FALSE:
  case JSON_NULL:
    return 0;
    break;
  }
  return total;
}

int main() {
  FILE *fp = fopen("ex12.input", "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  fgets(buffer, MAX_LINE, fp);
  fclose(fp);

  // read the json in to jansson json_t struct
  json_error_t error;
  json_t *root = json_loads(buffer, 0, &error);

  if (!root) {
    printf("Error reading json object\n");
    return EXIT_FAILURE;
  }

  int total = process_json(root);
  printf("Total: %d\n", total);

  json_decref(root);
}
