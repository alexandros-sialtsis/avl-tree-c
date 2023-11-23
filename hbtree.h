
#ifndef __HBTREE_H_
#define __HBTREE_H_

typedef struct height_balanced_node {
    struct height_balanced_node *parent;
    struct height_balanced_node *left;
    struct height_balanced_node *right;
    int num;
    int height;
    int weight;
} hbnode_t;

typedef struct height_balanced_tree {
    struct height_balanced_node *root;
    int size;
} hbtree_t;

int hbtree_create(hbtree_t **tree);

int tree_is_left_child(hbnode_t *node);

int tree_is_right_child(hbnode_t *node);

hbnode_t *tree_get_sibling(hbnode_t *node);

void tree_swap(hbnode_t *n1, hbnode_t *n2);

hbnode_t *tree_find_node(hbtree_t *tree, int n);

hbnode_t *tree_search(hbtree_t *tree, int n);

hbnode_t *tree_add(hbtree_t *tree, int n);

void tree_remove_leaf(hbtree_t *tree, hbnode_t *leaf);

hbnode_t *tree_leftest_right(hbnode_t *node);

void tree_remove_node(hbtree_t *tree, hbnode_t *node);

int tree_remove_n(hbtree_t *tree, int n);

void tree_print_preorder(hbnode_t *root);

void tree_free(hbtree_t **tree);

void tree_root_free(hbnode_t *root);

// height and weight update
void hw_update(hbnode_t *node);

// floor of log_2(n)
int log_2(int n);

// checks if the avl condition is violated
void avl_check(hbtree_t *tree, hbnode_t *node, int is_add);

int check(hbnode_t *curr);

// rebalances the tree
void avl_rebalance(hbtree_t *tree, hbnode_t *node, int is_del);

// single right avl turn
void single_right(hbnode_t *n1, hbnode_t *n2, hbnode_t *n3);

// single left avl turn
void single_left(hbnode_t *n1, hbnode_t *n2, hbnode_t *n3);

// double right avl turn
void double_right(hbnode_t *n1, hbnode_t *n2, hbnode_t *n3);

// double left avl turn
void double_left(hbnode_t *n1, hbnode_t *n2, hbnode_t *n3);

// makes the .dot file of the tree
void tree_print_picture(hbtree_t *node, int picno);

// make the text for each node
void tree_print_pic_node(hbnode_t *node, int fd);

#endif 


