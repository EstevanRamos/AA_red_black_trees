#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "redblacktrees.h"

#define NUM_VALUES 100000

int compare_int(void *a, void *b, void *data)
{
    int *ia = (int *)a;
    int *ib = (int *)b;
    return (*ia > *ib) - (*ia < *ib);
}

static void *copy_key(void *key, void *data)
{
    int *original_key = (int *)key;
    int *new_key = (int *)malloc(sizeof(int));
    if (new_key == NULL)
    {
        fprintf(stderr, "Error: no memory left.\n");
        exit(1);
    }
    *new_key = *original_key;
    return new_key;
}

static void *copy_value(void *value, void *data)
{
    int *original_value = (int *)value;
    int *new_value = (int *)malloc(sizeof(int));
    if (new_value == NULL)
    {
        fprintf(stderr, "Error: no memory left.\n");
        exit(1);
    }
    *new_value = *original_value;
    return new_value;
}

int main()
{
    red_black_tree_t tree = red_black_tree_create();

    srand(time(NULL));
    int insertedValues[NUM_VALUES];

    printf("Inserting values and checking height...\n");
    for (int i = 0; i < NUM_VALUES; i++)
    {
        int value = rand() % NUM_VALUES;
        red_black_tree_insert(tree, &value, &value, compare_int, copy_key, copy_value, NULL);
        insertedValues[i] = i;
    }
    printf("Finished inserting values. Starting deletions...\n");

    FILE *csv_file = fopen("RB_tree_data_removal.csv", "w");

    fprintf(csv_file, "Number of Entries,Tree Height\n");

    for (int i = 0; i < NUM_VALUES; i++)
    {
        red_black_tree_remove(tree, &i, compare_int, free, free, NULL);
        size_t numValues = red_black_tree_number_entries(tree);
        size_t height = red_black_tree_height(tree);
        fprintf(csv_file, "%zu,%zu\n", numValues, height);
    }

    fclose(csv_file);

    red_black_tree_delete(tree, free, free, NULL);

    return 0;
}
