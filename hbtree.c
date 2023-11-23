
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "hbtree.h"

// STANDARD FUNCTIONS

int hbtree_create(hbtree_t **tree) {
    *tree = (hbtree_t *) malloc(sizeof(hbtree_t));
    if(!(*tree)) return(0);

    (*tree)->root = NULL;
    (*tree)->size = 0;
    return(1);
}

int tree_is_left_child(hbnode_t *node) {
    if(!node->parent) return(0);
    if(node->parent->left == node) return(1);
    else return(0);
}

int tree_is_right_child(hbnode_t *node) {
    if(!node->parent) return(0);
    if(node->parent->right == node) return(1);
    else return(0);
}

hbnode_t *tree_get_sibling(hbnode_t *node) {
    if(!node->parent) return(NULL);
    if(tree_is_left_child(node)) return(node->parent->right);
    else return(node->parent->left);
}

void tree_swap(hbnode_t *n1, hbnode_t *n2) {
    int temp;

    temp = n1->num;
    n1->num = n2->num;
    n2->num = temp;
}

void tree_print_preorder(hbnode_t *root) {
    if(!root) return;
    printf("%d ", root->num);
    //printf("%d(%d, %d) ", root->num, root->height, root->weight);
    if(root->left) tree_print_preorder(root->left);
    if(root->right) tree_print_preorder(root->right);
}

void tree_free(hbtree_t **tree) {
    if((*tree)->root) tree_root_free((*tree)->root);
    free(*tree);
    *tree = NULL;
}

void tree_root_free(hbnode_t *root) {
    if(root->left) tree_root_free(root->left);
    if(root->right) tree_root_free(root->right);
    free(root);
}

// SEARCH FUNCTIONS

hbnode_t *tree_find_node(hbtree_t *tree, int n) {
    hbnode_t *curr;
    hbnode_t *parent;

    if(!tree->size) return(NULL);
    curr = tree->root;
    while(curr) {
        parent = curr;
        if(curr->num == n) {
            return(curr);
        }
        else if(curr->num > n) {
            curr = curr->left;
            continue;
        }
        else {
            curr = curr->right;
            continue;
        }
    }
    return(parent);
}

hbnode_t *tree_search(hbtree_t *tree, int n) {
    hbnode_t *res;

    res = tree_find_node(tree, n);
    if(res->num == n) return(res);
    else return(NULL); 
}

// ADD FUCTIONS

hbnode_t *tree_add(hbtree_t *tree, int n) {
    hbnode_t *parent;
    hbnode_t *new = NULL;

    parent = tree_find_node(tree, n);
    if(parent && parent->num == n) return(NULL);

    new = (hbnode_t *) malloc(sizeof(hbnode_t));
    new->num = n;
    new->parent = parent;
    new->left = new->right = NULL;
    new->height = 1;
    new->weight = 1;

    if(parent) {
        if(parent->num > n) parent->left = new;
        else parent->right = new;
    }
    else {
        tree->root = new;
    }
    tree->size++;
    hw_update(new);
    avl_check(tree, new, 1);
    //tree_print_picture(tree, 2);
    return(new);
}

void hw_update(hbnode_t *node) {
    hbnode_t *curr = node;

    while(curr) {
        curr->weight = 1;
        if(curr->left) curr->weight = curr->weight + curr->left->weight;
        if(curr->right) curr->weight = curr->weight + curr->right->weight;

        if(curr->left && !curr->right) curr->height = curr->left->height + 1;
        else if(!curr->left && curr->right) curr->height = curr->right->height + 1;
        else if(curr->left && curr->right) curr->height =
        (curr->left->height < curr->right->height) ?
        curr->right->height + 1 : curr->left->height + 1;
        else curr->height = 1;
        curr = curr->parent;
    }
}

// AVL FUNCTIONS

int log_2(int n) {
    int res = 0, temp = 1;

    while(temp < n) {
        temp = temp * 2;
        res++;
    }
    if(temp != n) res--;
    return(res);
}

int check(hbnode_t *curr) {
    int height_left, height_right, height_diff, log;

    log = log_2(curr->weight); if(log < 1) log = 1;
    height_left = (curr->left) ? curr->left->height : 0;
    height_right = (curr->right) ? curr->right->height : 0;
    height_diff = height_left - height_right;
    if(height_diff < 0) height_diff *= -1;

    if(height_diff > log) return(1);
    else return(0);
}

void avl_check(hbtree_t *tree, hbnode_t *node, int is_add) {
    hbnode_t *curr = node;
    //int height_left, height_right, height_diff, log;

    while(curr) {
        /*log = log_2(curr->weight); if(log < 1) log = 1;
        height_left = (curr->left) ? curr->left->height : 0;
        height_right = (curr->right) ? curr->right->height : 0;
        height_diff = height_left - height_right;
        if(height_diff < 0) height_diff *= -1; */

        if(check(curr)) {
            avl_rebalance(tree, curr, !is_add);
            if(is_add) break;
        }
        curr = curr->parent;
    }
}

void avl_rebalance(hbtree_t *tree, hbnode_t *node, int is_del) {
    hbnode_t *n1, *n2, *n3;

    //tree_print_picture(tree, 1);
    n1 = node;

    if(node->left && !node->right) n2 = node->left;
    else if (!node->left && node->right) n2 = node->right;
    else if(node->left->height > node->right->height) n2 = node->left;
    else n2 = node->right;

    if(n2->left && !n2->right) n3 = n2->left;
    else if(!n2->left && n2->right) n3 = n2->right;
    else if(n2->left->height > n2->right->height) n3 = n2->left;
    else if(n2->left->height < n2->right->height) n3 = n2->right;
    else if(tree_is_left_child(n2)) n3 = n2->left;
    else n3 = n2->right;

    if(n1->left == n2) {
        if(n2->left == n3) {
            single_right(n1, n2, n3);
            if(n1 == tree->root) tree->root = n2;
        }
        else {
            double_right(n1, n2, n3);
            if(n1 == tree->root) tree->root = n3;
        }
    }
    else {
        if(n2->right == n3) {
            single_left(n1, n2, n3);
            if(n1 == tree->root) tree->root = n2;
        }
        else {
            double_left(n1, n2, n3);
            if(n1 == tree->root) tree->root = n3;
        }
    }
    if(is_del) {
        if(check(n1)) avl_rebalance(tree, n1, 1);
        if(check(n2)) avl_rebalance(tree, n2, 1);
        if(n3 && check(n3)) avl_rebalance(tree, n3, 1);
    }
    //tree_print_picture(tree, 2);
}

void single_right(hbnode_t *n1, hbnode_t *n2, hbnode_t *n3) {
    hbnode_t *temp = n2->right;

    //printf("making SINGLE RIGHT\n");

    n2->parent = n1->parent;
    if(n2->parent && tree_is_left_child(n1)) n2->parent->left = n2;
    else if(n2->parent && tree_is_right_child(n1)) n2->parent->right = n2;
    n2->right = n1;

    n1->parent = n2;
    n1->left = temp;
    if(temp) temp->parent = n1;

    /*if(n1->right) hw_update(n1->right);
    else if(n1->left) hw_update(n1->left);*/
    hw_update(n1);
    // hw_update(n3);
}

void single_left(hbnode_t *n1, hbnode_t *n2, hbnode_t *n3) {
    hbnode_t *temp = n2->left;

    //printf("making SINGLE LEFT\n");
    //if(n3) printf("n1: %d  n2: %d  n3: %d\n", n1->num, n2->num, n3->num);
    //else printf("n1: %d  n2: %d  n3: NULL\n", n1->num, n2->num);

    n2->parent = n1->parent;
    if(n2->parent && tree_is_right_child(n1)) n2->parent->right = n2;
    else if(n2->parent && tree_is_left_child(n1)) n2->parent->left = n2;
    n2->left = n1;

    n1->parent = n2;
    n1->right = temp;
    if(temp) temp->parent = n1;

    /*if(n1->left) hw_update(n1->left);
    else if(n1->right) hw_update(n1->right);
    else {
        n1->height = 1;
        hw_update(n1);
    }*/
    hw_update(n1);
    //hw_update(n3);
}

void double_right(hbnode_t *n1, hbnode_t *n2, hbnode_t *n3) {
    //hbnode_t *temp;

    //printf("making DOUBLE RIGHT\n");

    /*temp = n3->left;
    n3->parent = n2->parent;
    n3->parent->left = n3;
    n3->left = n2;

    n2->right = temp;
    n2->parent = n3;
    
    if(n2->left) hw_update(n2->left);
    else if(n2->right) hw_update(n2->right);
    else {
        n2->height = 1;
        hw_update(n2);
    }*/
    single_left(n2, n3, n3->right);
    single_right(n1, n3, n2);
}

void double_left(hbnode_t *n1, hbnode_t *n2, hbnode_t *n3) {
    //hbnode_t *temp;

    //printf("making DOUBLE LEFT\n");

    /*temp = n3->right;
    n3->parent = n2->parent;
    n3->parent->right = n3;
    n3->right = n2;

    n2->left = temp;
    n2->parent = n3;

    if(n2->right) hw_update(n2->right);
    else if(n2->left) hw_update(n2->left);
    else {
        n2->height = 1;
        hw_update(n2);
    }
    hw_update(n2);*/
    single_right(n2, n3, n3->left);
    single_left(n1, n3, n2);
}

// DELETE FUNCTIONS

/*void tree_remove_leaf(hbtree_t *tree, hbnode_t *leaf) {
    if(leaf->parent) {
        if(tree_is_left_child(leaf)) leaf->parent->left = NULL;
        else leaf->parent->right = NULL;
        hw_update(leaf->parent);
        avl_check(tree, leaf->parent, 0);
    }
    else {
        tree->root = NULL;
    }
    free(leaf);
    tree->size--;
}*/

hbnode_t *tree_leftest_right(hbnode_t *node) {
    hbnode_t *curr;
    
    for(curr = node->right; curr->left; curr = curr->left);
    return(curr);
}

void tree_remove_node(hbtree_t *tree, hbnode_t *node) {
    hbnode_t *leftest, *parent = node->parent;

    /*printf("\n\nnode to be deleted: %d\n", node->num);
    if(parent) printf("parent: %d\n", parent->num);
    else printf("parent: NULL\n");
    if(node->left) printf("left: %d", node->left->num);
    else printf("left: NULL");
    if(node->right) printf("    right: %d\n", node->right->num);
    else printf("    right: NULL\n\n");
    */

    if(node->left && node->right) {
        leftest = tree_leftest_right(node);
        tree_swap(node, leftest);
        node = leftest;
    }

    if(node->right) node->right->parent = parent;
    if(node->left) node->left->parent = parent;

    if(tree->root == node) tree->root = (node->right) ? node->right : node->left;
    else if(tree_is_left_child(node)) parent->left = (node->right) ? node->right : node->left;
    else parent->right = (node->right) ? node->right : node->left;

    if(parent) hw_update(parent);
    avl_check(tree, parent, 0);
    free(node);
}

/*void tree_remove_node(hbtree_t *tree, hbnode_t *node) {
    hbnode_t *leftest;

    if(!node->left && !node->right) {
        tree_remove_leaf(tree, node);
    }
    else if(!(node->left && node->right) && (node->left || node->right)) {
        if(node->left) {
            node->left->parent = node->parent;
            if(tree_is_left_child(node)) node->parent->left = node->left;
            else if(tree_is_right_child(node)) node->parent->right = node->left;
            else tree->root = node->left;
            hw_update(node->left); //
            avl_check(tree, node->left, 0); //
        }
        else {
            node->right->parent = node->parent;
            if(tree_is_left_child(node)) node->parent->left = node->right;
            else if(tree_is_right_child(node)) node->parent->right = node->right;
            else tree->root = node->right;
            hw_update(node->right); //
            avl_check(tree, node->right, 0); //
        }
        //if(node->parent) hw_update(node->parent);
        free(node);
        tree->size--;
    }
    else {
        leftest = tree_leftest_right(node);
        tree_swap(node, leftest);
        tree_remove_node(tree, leftest);
    }
}*/

int tree_remove_n(hbtree_t *tree, int n) {
    hbnode_t *node;

    node = tree_search(tree, n);
    if(!node) return(0);
    tree_remove_node(tree, node);
    //tree_print_picture(tree, 2);
    return(1);
}

// .dot FILE FUNCTIONS

/*void tree_print_picture(hbtree_t *tree, int picno) {
    int fd;
    char filename[20], buf[6];

    // filename = treeX.dot
    strcpy(filename, "");
    strcpy(buf, "");
    sprintf(buf, "%d", picno);

    strcpy(filename, "tree");
    strcat(filename, buf);
    strcat(filename, ".dot");

    fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);

    write(fd, "graph G {\n", 10 * sizeof(char));
    tree_print_pic_node(tree->root, fd);
    write(fd, "}", sizeof(char));
    close(fd);
}

void tree_print_pic_node(hbnode_t *node, int fd) {
    char num_str[6], child_num_str[6];

    strcpy(num_str, "");
    sprintf(num_str, "%d", node->num);
    
    write(fd, "  ", 2 * sizeof(char));
    write(fd, num_str, strlen(num_str) * sizeof(char));
    write(fd, "\n", sizeof(char));

    if(node->left) {
        strcpy(child_num_str, "");
        sprintf(child_num_str, "%d", node->left->num);  

        write(fd, "  ", 2 * sizeof(char));
        write(fd, num_str, strlen(num_str) * sizeof(char));
        write(fd, " -- ", 4 * sizeof(char));
        write(fd, child_num_str, strlen(child_num_str) * sizeof(char));
        write(fd, "\n", sizeof(char));

        tree_print_pic_node(node->left, fd);
    }

    if(node->right) {
        strcpy(child_num_str, "");
        sprintf(child_num_str, "%d", node->right->num);  

        write(fd, "  ", 2 * sizeof(char));
        write(fd, num_str, strlen(num_str) * sizeof(char));
        write(fd, " -- ", 4 * sizeof(char));
        write(fd, child_num_str, strlen(child_num_str) * sizeof(char));
        write(fd, "\n", sizeof(char));

        tree_print_pic_node(node->right, fd);
    }
}
*/

