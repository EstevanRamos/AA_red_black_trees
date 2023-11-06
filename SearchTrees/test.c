
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include "searchtrees.h"
#include "searchtrees.c"

#define LINE_BUFFER_LEN ((size_t)4096)


static void input_string(char str[], size_t n)
{
  char c;
  size_t i;
  int first;

  if (n < ((size_t)1))
    return;
  first = 1;
  for (i = 0; i < (n - ((size_t)1)); i++)
  {
    scanf("%c", &c);
    if (c == '\n')
    {
      if (first)
      {
        first = 0;
        i = (size_t)0;
        i--;
        continue;
      }
      else
      {
        break;
      }
    }
    str[i] = c;
    first = 0;
  }
  str[i] = '\0';
}

static void delete_string(void *ptr)
{
  free(ptr);
}

static void delete_key(void *ptr, void *data)
{
  delete_string(ptr);
}

static void delete_value(void *ptr, void *data)
{
  delete_string(ptr);
}

static void *copy_string(void *ptr)
{
  char *str = ptr;
  size_t len;
  char *new_str;

  len = strlen(ptr);
  len++;

  new_str = calloc(len, sizeof(char));
  if (new_str == NULL)
  {
    fprintf(stderr, "Error: no memory left.\n");
    exit(1);
  }

  strcpy(new_str, str);

  return new_str;
}

static void *copy_key(void *ptr, void *data)
{
  return copy_string(ptr);
}

static void *copy_value(void *ptr, void *data)
{
  return copy_string(ptr);
}

static int compare_key(void *ptr_a, void *ptr_b, void *data)
{
  char *str_a = ptr_a;
  char *str_b = ptr_b;

  return strcmp(str_a, str_b);
}

int main(int argc, char **argv)
{
  char key[LINE_BUFFER_LEN];
  char value[LINE_BUFFER_LEN];
  char *temp_key, *temp_value;
  search_tree_t tree;

  tree = search_tree_create();

  for (;;)
  {
    printf("The current search tree has %zu entries.\n", search_tree_number_entries(tree));
    printf("The current search tree has height %zu.\n", search_tree_height(tree));
    search_tree_minimum((void **)&temp_key, (void **)&temp_value, tree);
    if ((temp_key != NULL) && (temp_value != NULL))
    {
      printf("The minimum key is \"%s\", the associated value is \"%s\".\n",
             temp_key, temp_value);
    }
    else
    {
      printf("The tree has no minimum key.\n");
    }
    search_tree_maximum((void **)&temp_key, (void **)&temp_value, tree);
    if ((temp_key != NULL) && (temp_value != NULL))
    {
      printf("The maximum key is \"%s\", the associated value is \"%s\".\n",
             temp_key, temp_value);
    }
    else
    {
      printf("The tree has no maximum key.\n");
    }
    printf("Please enter a key to add to the tree. Enter <quit> to stop.\n");
    input_string(key, sizeof(key));
    if (strcmp(key, "<quit>") == 0)
      break;
    printf("Please enter a value associated with the key.\n");
    input_string(value, sizeof(value));
    temp_value = search_tree_search(tree, key, compare_key, NULL);
    if (temp_value != NULL)
    {
      printf("Cannot enter the new key \"%s\" with new value \"%s\" as the tree already contains the key with value \"%s\".\n", key, value, temp_value);
    }
    else
    {
      search_tree_insert(tree, key, value, compare_key, copy_key, copy_value, NULL);
    }
    printf("Please enter a key to search for in the tree.\n");
    input_string(key, sizeof(key));
    temp_value = search_tree_search(tree, key, compare_key, NULL);
    if (temp_value != NULL)
    {
      printf("The tree contains the key \"%s\" with the associated value \"%s\".\n", key, temp_value);
      search_tree_predecessor((void **)&temp_key, (void **)&temp_value,
                              tree, key, compare_key, NULL);
      if ((temp_key == NULL) || (temp_value == NULL))
      {
        printf("The key \"%s\" does not have a predecessor in the tree.\n", key);
      }
      else
      {
        printf("The key \"%s\" has the predecessor key \"%s\" with value \"%s\".\n",
               key, temp_key, temp_value);
      }
      search_tree_successor((void **)&temp_key, (void **)&temp_value,
                            tree, key, compare_key, NULL);
      if ((temp_key == NULL) || (temp_value == NULL))
      {
        printf("The key \"%s\" does not have a successor in the tree.\n", key);
      }
      else
      {
        printf("The key \"%s\" has the successor key \"%s\" with value \"%s\".\n",
               key, temp_key, temp_value);
      }
    }
    else
    {
      printf("The tree does not contain an entry with key \"%s\".\n", key);
    }
    printf("Please enter a key to delete from the tree. Enter <nothing> to delete nothing.\n");
    input_string(key, sizeof(key));
    if (strcmp(key, "<nothing>") != 0)
    {
      search_tree_remove(tree, key,
                         compare_key, delete_key, delete_value, NULL);
    }
  }

  search_tree_delete(tree, delete_key, delete_value, NULL);

  return 0;
}
