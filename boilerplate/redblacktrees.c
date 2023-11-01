
#include <stdio.h>
#include <stdlib.h>

typedef enum {
  RED_BLACK_TREE_COLOR_RED,
  RED_BLACK_TREE_COLOR_BLACK
} color_t;

typedef struct __tree_node_struct_t * tree_node_t;
struct __tree_node_struct_t {
  void *key;
  void *value;
  color_t color;
  tree_node_t parent;
  tree_node_t left;
  tree_node_t right;
};

struct __red_black_tree_struct_t {
  tree_node_t root;
};

#include "redblacktrees.h"

static void error_no_mem() {
  fprintf(stderr, "Error: no memory left.\n");
  exit(1);
}

red_black_tree_t red_black_tree_create(){
  red_black_tree_t tree;
  tree = calloc(1,sizeof(*tree));
  if(tree == NULL){
    error_no_mem();
  }
  tree->root = NULL;
  return tree; 
}

void red_black_tree_delete(red_black_tree_t tree,
                           void (*delete_key)(void *, void *),
                           void (*delete_value)(void *, void *),
                           void *data) {
}

size_t red_black_tree_number_entries(red_black_tree_t tree) {
  __red_black_tree_number_entries_aux(tree->root);
}

static size_t __red_black_tree_number_entries_aux(tree_node_t node){
  size_t l, r, n;
  if(node == NULL) return ((size_t) 0);
  l = __search_tree_number_entries_aux(node->left);
  r = __search_tree_number_entries_aux(node->right);

  n = (l + r) + ((size_t) 1);

  return n;
}

size_t red_black_tree_height(red_black_tree_t tree) {
  return __red_black_tree_height_aux(tree->root);
}

static size_t __red_black_tree_height_aux(red_black_tree_t node){
  size_t l, r, h;

  if (node == NULL) return ((size_t) 0);

  l = __search_tree_height_aux(node->left);
  r = __search_tree_height_aux(node->right);

  h = ((l > r) ? l : r) + ((size_t) 1);

  return h;
}

void *red_black_tree_search(red_black_tree_t tree,
                            void *key,
                            int (*compare_key)(void *, void *, void *),
                            void *data) {
  tree_node_t node;
  node = __red_black_tree_search_aux(tree->root,key,compare_key,data);
  if(node == NULL) return NULL;
  return node->value;
}

static tree_node_t __red_black_tree_search_aux(tree_node_t node,
                            void *key,
                            int (*compare_key)(void *, void *, void *),
                            void *data){
  int cmp;
  if( node == NULL) return NULL;//base case
  cmp = compare_key(key, node->key, data);

  if (cmp == 0) return node;
  if (cmp < 0) return __red_black_tree_search_aux(node->left,key,compare_key,data);
  __red_black_tree_search_aux(node->right,key,compare_key,data);
}

void red_black_tree_minimum(void **min_key,
                            void **min_value,
                            red_black_tree_t tree) {
  tree_node_t node;

  if (tree->root == NULL) {
    *min_key = NULL;
    *min_value = NULL;
    return;
  }

  for (node=tree->root; node->left!=NULL; node=node->left);

  *min_key = node->key;
  *min_value = node->value;
}

void red_black_tree_maximum(void **max_key,
                            void **max_value,
                            red_black_tree_t tree) {
  tree_node_t node;

  if (tree->root == NULL) {
    *max_key = NULL;
    *max_value = NULL;
    return;
  }

  for (node=tree->root; node->right!=NULL; node=node->right);

  *max_key = node->key;
  *max_value = node->value;
}

void red_black_tree_predecessor(void **prec_key,
                                void **prec_value,
                                red_black_tree_t tree,
                                void *key,
                                int (*compare_key)(void *, void *, void *),
                                void *data) {
  tree_node_t x, y;

  x = __red_black_tree_search_aux(tree->root,key,compare_key,data);

  if (x == NULL) {
    *prec_key = NULL;
    *prec_value = NULL;
    return;
  }

  if (x->left != NULL) {
    for (y=x->left; y->right!=NULL; y=y->right);
    *prec_key = y->key;
    *prec_value = y->value;
    return;
  }

  for (y=x->parent; ((y!=NULL) && (x==y->left));) {
    x = y;
    y = y->parent;
  }

  if (y == NULL) {
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
                              void *data) {
 tree_node_t x, y;

  x = __red_black_tree_search_aux(tree->root,key,compare_key,data);

  if (x == NULL) {
    *succ_key = NULL;
    *succ_value = NULL;
    return;
  }

  if (x->right != NULL) {
    for (y=x->right; y->left!=NULL; y=y->left);
    *succ_key = y->key;
    *succ_value = y->value;
    return;
  }

  for (y=x->parent; ((y!=NULL) && (x==y->right));) {
    x = y;
    y = y->parent;
  }

  if (y == NULL) {
    *succ_key = NULL;
    *succ_value = NULL;
    return;
  }
  *succ_key = y->key;
  *succ_value = y->value;

}

void red_black_tree_insert(red_black_tree_t tree,
                           void *key,
                           void *value,
                           int (*compare_key)(void *, void *, void *),
                           void *(*copy_key)(void *, void *),
                           void *(*copy_value)(void *, void *),
                           void *data) {
  tree_node_t x, y, z;

  if (__red_black_tree_search_aux(tree->root,
                               key,
                               compare_key,
                               data) != NULL) return;

  z = __red_black_tree_insert_aux(key, value,
                               copy_key, copy_value,
                               data);

  if (tree->root == NULL) {
    z->color = RED_BLACK_TREE_COLOR_BLACK;
    tree->root = z;
    return;
  }

  x = tree->root;
  y = NULL;
  while (x!=NULL) {
    y = x;
    if (compare_key(z->key, x->key, data) < 0) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  z->parent = y;
  if (y == NULL) {
    tree->root = z;
  } else {
    if (compare_key(z->key, y->key, data) < 0) {
      y->left = z;
    } else {
      y->right = z;
    }
  }
  

}
static __red_black_tree_insert_fix(){

}

static tree_node_t __red_black_tree_insert_aux(void *key,
                                            void *value,
                                            void *(*copy_key)(void *, void *),
                                            void *(*copy_value)(void *, void *),
                                            void *data) {
  tree_node_t new_node;
  new_node = calloc(1, sizeof(*new_node));
  if (new_node == NULL) {
    error_no_mem();
  }

  new_node->key = copy_key(key, data);
  new_node->value = copy_value(value, data);
  new_node->color = RED_BLACK_TREE_COLOR_RED;
  new_node->parent = NULL;
  new_node->left = NULL;
  new_node->right = NULL;

  return new_node;
}

void red_black_tree_remove(red_black_tree_t tree,
                           void *key,
                           int (*compare_key)(void *, void *, void *),
                           void (*delete_key)(void *, void *),
                           void (*delete_value)(void *, void *),
                           void *data) {
  // STUB
}
