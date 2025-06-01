#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Defind the structure List
struct List {
    int id;
    struct List *next;
};

// Defind the structure Node
struct Node {
    int risk_level;
    struct List *id;
    int count;
    struct Node *left_child;
    struct Node *right_child;

    // I just add this field for balancing
    int height; // Height of tree for balancing <- AVL tree
};

// Defind the structure Tree
struct Tree {
    struct Node *root;
    struct Node *min;
    struct Node *max;
    int max_data;
    int current_nbr_data;
};

// Function prototypes
struct Tree *create_empty_tree(int max_data);
void insert(struct Tree *tree, int risk_level, int id);
struct Node *find_min(struct Tree *tree);
struct Node *find_max(struct Tree *tree);
void delete_min(struct Tree *tree);
void delete_max(struct Tree *tree);
void free_tree(struct Tree *tree);

// Balancing functions
int max(int a, int b);
void calc_local_height(struct Node* root);
int height_diff(struct Node* root);
struct Node* balance(struct Node* root);
struct Node* right_shift(struct Node* root);
struct Node* left_shift(struct Node* root);
struct Node* left_right_shift(struct Node* root);
struct Node* right_left_shift(struct Node* root);

#endif // TREE_H