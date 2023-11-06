
#include <stdio.h>
#include <stdlib.h>

typedef enum
{
  RED_BLACK_TREE_COLOR_RED,
  RED_BLACK_TREE_COLOR_BLACK
} color_t;

typedef struct __tree_node_struct_t *tree_node_t;
struct __tree_node_struct_t
{
  void *key;
  void *value;
  color_t color;
  tree_node_t parent;
  tree_node_t left;
  tree_node_t right;
};

struct __red_black_tree_struct_t
{
  tree_node_t root;
};

#include "redblacktrees.h"

// static void error_no_mem() {
//   fprintf(stderr, "Error: no memory left.\n");
//   exit(1);
// }

red_black_tree_t red_black_tree_create()
{
  red_black_tree_t tree;
  tree = calloc(1, sizeof(*tree));
  if (tree == NULL)
  {
    fprintf(stderr, "Error: no memory left.\n");
    exit(1);
  }
  tree->root = NULL;
  return tree;
}

static void left_rotate(red_black_tree_t tree, tree_node_t x);
static void right_rotate(red_black_tree_t tree, tree_node_t y);

void red_black_tree_delete(red_black_tree_t tree,
                           void (*delete_key)(void *, void *),
                           void (*delete_value)(void *, void *),
                           void *data)
{
}

static size_t __red_black_tree_number_entries_aux(tree_node_t node)
{
  size_t l, r, n;
  if (node == NULL)
    return ((size_t)0);
  l = __red_black_tree_number_entries_aux(node->left);
  r = __red_black_tree_number_entries_aux(node->right);
  n = (l + r) + ((size_t)1);
  return n;
}

size_t red_black_tree_number_entries(red_black_tree_t tree)
{
  return __red_black_tree_number_entries_aux(tree->root);
}

static tree_node_t __red_black_tree_search_aux(tree_node_t node,
                                               void *key,
                                               int (*compare_key)(void *, void *, void *),
                                               void *data)
{
  int cmp;
  if (node == NULL)
    return NULL; // base case
  cmp = compare_key(key, node->key, data);
  if (cmp == 0)
    return node;
  if (cmp < 0)
    return __red_black_tree_search_aux(node->left, key, compare_key, data);
  return __red_black_tree_search_aux(node->right, key, compare_key, data);
}

void *red_black_tree_search(red_black_tree_t tree,
                            void *key,
                            int (*compare_key)(void *, void *, void *),
                            void *data)
{
  tree_node_t node;
  node = __red_black_tree_search_aux(tree->root, key, compare_key, data);
  if (node == NULL)
    return NULL;
  return node->value;
}

void red_black_tree_minimum(void **min_key,
                            void **min_value,
                            red_black_tree_t tree)
{
  tree_node_t node;

  if (tree->root == NULL)
  {
    *min_key = NULL;
    *min_value = NULL;
    return;
  }

  for (node = tree->root; node->left != NULL; node = node->left)
    ;

  *min_key = node->key;
  *min_value = node->value;
}

void red_black_tree_maximum(void **max_key,
                            void **max_value,
                            red_black_tree_t tree)
{
  tree_node_t node;

  if (tree->root == NULL)
  {
    *max_key = NULL;
    *max_value = NULL;
    return;
  }

  for (node = tree->root; node->right != NULL; node = node->right)
    ;

  *max_key = node->key;
  *max_value = node->value;
}

void red_black_tree_predecessor(void **prec_key,
                                void **prec_value,
                                red_black_tree_t tree,
                                void *key,
                                int (*compare_key)(void *, void *, void *),
                                void *data)
{
  tree_node_t x, y;

  x = __red_black_tree_search_aux(tree->root, key, compare_key, data);

  if (x == NULL)
  {
    *prec_key = NULL;
    *prec_value = NULL;
    return;
  }

  if (x->left != NULL)
  {
    for (y = x->left; y->right != NULL; y = y->right)
      ;
    *prec_key = y->key;
    *prec_value = y->value;
    return;
  }

  for (y = x->parent; ((y != NULL) && (x == y->left));)
  {
    x = y;
    y = y->parent;
  }

  if (y == NULL)
  {
    *prec_key = NULL;
    *prec_value = NULL;
    return;
  }
  *prec_key = y->key;
  *prec_value = y->value;
}

void red_black_tree_successor(void **succ_key,
                              void **succ_value,
                              red_black_tree_t tree,
                              void *key,
                              int (*compare_key)(void *, void *, void *),
                              void *data)
{
  tree_node_t x, y;

  x = __red_black_tree_search_aux(tree->root, key, compare_key, data);

  if (x == NULL)
  {
    *succ_key = NULL;
    *succ_value = NULL;
    return;
  }

  if (x->right != NULL)
  {
    for (y = x->right; y->left != NULL; y = y->left)
      ;
    *succ_key = y->key;
    *succ_value = y->value;
    return;
  }

  for (y = x->parent; ((y != NULL) && (x == y->right));)
  {
    x = y;
    y = y->parent;
  }

  if (y == NULL)
  {
    *succ_key = NULL;
    *succ_value = NULL;
    return;
  }
  *succ_key = y->key;
  *succ_value = y->value;
}
static tree_node_t __red_black_tree_insert_aux(void *key,
                                               void *value,
                                               void *(*copy_key)(void *, void *),
                                               void *(*copy_value)(void *, void *),
                                               void *data)
{
  tree_node_t new_node;
  new_node = calloc(1, sizeof(*new_node));
  if (new_node == NULL)
  {
    fprintf(stderr, "Error: no memory left.\n");
    exit(1);
  }

  new_node->key = copy_key(key, data);
  new_node->value = copy_value(value, data);
  new_node->color = RED_BLACK_TREE_COLOR_RED;
  new_node->parent = NULL;
  new_node->left = NULL; // Initialize left and right pointers
  new_node->right = NULL;

  return new_node;
}

static size_t __red_black_tree_height_aux(tree_node_t node)
{
  size_t l, r, h;

  if (node == NULL)
    return ((size_t)0);

  l = __red_black_tree_height_aux(node->left);
  r = __red_black_tree_height_aux(node->right);

  h = ((l > r) ? l : r) + ((size_t)1);

  return h;
}

size_t red_black_tree_height(red_black_tree_t tree)
{
  return __red_black_tree_height_aux(tree->root);
}

static void __red_black_tree_insert_fix(red_black_tree_t tree, tree_node_t z)
{
  while (z != tree->root && z->parent->color == RED_BLACK_TREE_COLOR_RED)
  {
    if (z->parent == z->parent->parent->left)
    {
      tree_node_t y = z->parent->parent->right;
      if (y != NULL && y->color == RED_BLACK_TREE_COLOR_RED)
      {
        // red uncle
        z->parent->color = RED_BLACK_TREE_COLOR_BLACK;
        y->color = RED_BLACK_TREE_COLOR_BLACK;
        z->parent->parent->color = RED_BLACK_TREE_COLOR_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->right)
        {
          // black uncle, z is on right side
          z = z->parent;
          left_rotate(tree, z);
        }
        // black uncle, z is on left side
        z->parent->color = RED_BLACK_TREE_COLOR_BLACK;
        z->parent->parent->color = RED_BLACK_TREE_COLOR_RED;
        right_rotate(tree, z->parent->parent);
      }
    }
    else
    {
      // same for right side
      tree_node_t y = z->parent->parent->left;
      if (y != NULL && y->color == RED_BLACK_TREE_COLOR_RED)
      {
        // red uncle
        z->parent->color = RED_BLACK_TREE_COLOR_BLACK;
        y->color = RED_BLACK_TREE_COLOR_BLACK;
        z->parent->parent->color = RED_BLACK_TREE_COLOR_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->left)
        {
          // black uncle, z is on left
          z = z->parent;
          right_rotate(tree, z);
        }
        // black uncle, z is on right
        z->parent->color = RED_BLACK_TREE_COLOR_BLACK;
        z->parent->parent->color = RED_BLACK_TREE_COLOR_RED;
        left_rotate(tree, z->parent->parent);
      }
    }
  }
  tree->root->color = RED_BLACK_TREE_COLOR_BLACK;
}

void red_black_tree_insert(red_black_tree_t tree,
                           void *key,
                           void *value,
                           int (*compare_key)(void *, void *, void *),
                           void *(*copy_key)(void *, void *),
                           void *(*copy_value)(void *, void *),
                           void *data)
{
  tree_node_t x, y, z;

  if (__red_black_tree_search_aux(tree->root, key, compare_key, data) != NULL)
    return;

  z = __red_black_tree_insert_aux(key, value, copy_key, copy_value, data);

  if (tree->root == NULL)
  {
    z->color = RED_BLACK_TREE_COLOR_BLACK;
    tree->root = z;
    return;
  }

  x = tree->root;
  y = NULL;
  while (x != NULL)
  {
    y = x;
    if (compare_key(z->key, x->key, data) < 0)
    {
      x = x->left;
    }
    else
    {
      x = x->right;
    }
  }
  z->parent = y;
  if (y == NULL)
  {
    tree->root = z;
  }
  else
  {
    if (compare_key(z->key, y->key, data) < 0)
    {
      y->left = z;
    }
    else
    {
      y->right = z;
    }
  }
  __red_black_tree_insert_fix(tree, z);
}

static void left_rotate(red_black_tree_t tree, tree_node_t x)
{
  tree_node_t y = x->right;
  x->right = y->left;
  if (y->left != NULL)
  {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == NULL)
  {
    tree->root = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

static void right_rotate(red_black_tree_t tree, tree_node_t y)
{
  tree_node_t x = y->left;
  y->left = x->right;
  if (x->right != NULL)
  {
    x->right->parent = y;
  }
  x->parent = y->parent;
  if (y->parent == NULL)
  {
    tree->root = x;
  }
  else if (y == y->parent->right)
  {
    y->parent->right = x;
  }
  else
  {
    y->parent->left = x;
  }
  x->right = y;
  y->parent = x;
}

void red_black_tree_remove(red_black_tree_t tree,
                           void *key,
                           int (*compare_key)(void *, void *, void *),
                           void (*delete_key)(void *, void *),
                           void (*delete_value)(void *, void *),
                           void *data)
{
  // STUB
}
