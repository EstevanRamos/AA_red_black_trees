#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "searchtrees.h"

#define NUM_VALUES 10000

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
    search_tree_t tree = search_tree_create();

    srand(time(NULL));

    printf("Inserting values and checking height...\n");

    FILE *outputFile = fopen("BST_tree_data.csv", "w");

    fprintf(outputFile, "Number of Entries,Tree Height\n"); // CSV header

    for (int i = 0; i < NUM_VALUES; i++)
    {
        int value = rand() % NUM_VALUES;
        search_tree_insert(tree, &value, &value, compare_int, copy_key, copy_value, NULL);
        size_t numValues = search_tree_number_entries(tree);
        size_t height = search_tree_height(tree);

        fprintf(outputFile, "%zu,%zu\n", numValues, height);
    }

    size_t numValues = search_tree_number_entries(tree);
    size_t height = search_tree_height(tree);
    fprintf(outputFile, "%zu,%zu\n", numValues, height);
    fclose(outputFile);

    printf("Finished inserting values.\n");

    search_tree_delete(tree, free, free, NULL);

    return 0;
}
