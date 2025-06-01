#include "../include/tree.h"

// Create empty tree
struct Tree *create_empty_tree(int max_data) {
    struct Tree *tree = (struct Tree *)malloc(sizeof(struct Tree));
    tree->root = NULL;
    tree->min = NULL;
    tree->max = NULL;
    tree->max_data = max_data;
    tree->current_nbr_data = 0;
    return tree;
}

// Insert function
struct Node* insert_intern(struct Node* root,int risk_level, int id, bool* left, bool* right, struct Node** add){
    if(root == NULL){
        struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
        new_node->risk_level = risk_level;
        new_node->id = (struct List *)malloc(sizeof(struct List));
        new_node->id->id = id;
        new_node->id->next = NULL;
        new_node->count = 1;
        new_node->left_child = NULL;
        new_node->right_child = NULL;
        calc_local_height(new_node);
        (*add) = new_node;
        return new_node;
    }
    if(root->risk_level == risk_level){
        struct List * new_first_list = malloc(sizeof(struct List));
        new_first_list->next = root->id;
        new_first_list->id = id;
        root->id = new_first_list;
        (*add) = root;
        return root;
    }
    if(root->risk_level > risk_level){
        *right = false;
        root->left_child = insert_intern(root->left_child,risk_level, id,left, right, add);
    }
    if(root->risk_level < risk_level){
        *left = false;
        root->right_child = insert_intern(root->right_child,risk_level, id,left, right, add);
    }
    calc_local_height(root);
    root = balance(root);
    return root;

}

void insert(struct Tree *tree, int risk_level, int id){
    if(tree->current_nbr_data == tree->max_data){
        delete_min(tree);
    }
    struct Node* res;
    bool left = true;
    bool right = true;
    tree->root = insert_intern(tree->root,risk_level, id, &left, &right, &res);
    // if(left){
    //     tree->min = res;
    // }
    // if(right){
    //     tree->max = res;
    // }
    if (tree->min)
    {
        if (tree->min->risk_level > risk_level)
        {
            tree->min = res;
        }
    }
    else {
        tree->min = res;
    }

    if (tree->max)
    {
        if (tree->max->risk_level < risk_level)
        {
            tree->max = res;
        }
    }
    else {
        tree->max = res;
    }
    
    tree->current_nbr_data++;
}


// Recherche du noeud avec le risque minimum
struct Node *find_min(struct Tree *tree) {
    return tree->min; // just a pointer to the minimum node
}

// Recherche du noeud avec le risque maximum
struct Node *find_max(struct Tree *tree) {
    return tree->max; // just a pointer to the maximum node
}


struct Node* delete_min_intern(struct Node* root, struct Node** new_min,bool* deleted){
    if(root->left_child == NULL){
        if(root->id->next == NULL){
            free(root->id);
            struct Node* ret = root->right_child;
            free(root);
            (*deleted) = true;
            if(ret != NULL){
                (*new_min) = ret;
            }
            return ret;
        }
        else{
            struct List* suppr = root->id;
            root->id = root->id->next;
            free(suppr);
            (*deleted) = false;
            return root;
        }
    }
    else{
        (*new_min) = root;
        root->left_child = delete_min_intern(root->left_child, new_min, deleted);
        calc_local_height(root);
        root = balance(root);
        return root;
    }
}

// Suppression du noeud avec le risque minimum
void delete_min(struct Tree *tree) {
    if(tree->current_nbr_data == 0){
        return;
    }
    if(tree->current_nbr_data == 1){
        free(tree->root->id);
        free(tree->root);
        tree->max = NULL;
        tree->min = NULL;
        tree->root = NULL;
    }
    else{
        struct Node* new_min;
        bool deleted;
        tree->root = delete_min_intern(tree->root,&new_min,&deleted);
        if(deleted){
            while(new_min->left_child != NULL){
                new_min = new_min ->left_child;
            }
            tree->min = new_min;
        }
    }
    tree->current_nbr_data--;
}

struct Node* delete_max_intern(struct Node* root, struct Node** new_max,bool* deleted)
{
    if(root->right_child == NULL){
        if(root->id->next == NULL){
            free(root->id);
            struct Node* ret = root->left_child;
            free(root);
            (*deleted) = true;
            if(ret != NULL){
                (*new_max) = ret;
            }
            return ret;
        }
        else{
            struct List* suppr = root->id;
            root->id = root->id->next;
            free(suppr);
            (*deleted) = false;
            return root;
        }
    }
    else{
        (*new_max) = root;
        root->right_child = delete_max_intern(root->right_child, new_max, deleted);
        calc_local_height(root);
        root = balance(root);
        return root;
   }
};

// Suppression du noeud avec le risque maximum
void delete_max(struct Tree *tree) {
    if(tree->current_nbr_data == 0){
        return;
    }
    if(tree->current_nbr_data == 1){
        free(tree->root->id);
        free(tree->root);
        tree->max = NULL;
        tree->min = NULL;
        tree->root = NULL;
    }
    else{
        struct Node* new_max;
        bool deleted;
        tree->root = delete_max_intern(tree->root, &new_max, &deleted);
        if(deleted){
            while(new_max->right_child != NULL){
                new_max = new_max ->right_child;
            }
            tree->max = new_max;
        }
    }
    tree->current_nbr_data--;
}

// Libération de la mémoire
void free_node(struct Node* node){
    if(node != NULL){
        free_node(node->left_child);
        free_node(node->right_child);
        struct List* suppr;
        while(node->id != NULL){
            suppr = node->id;
            node->id = node->id->next;
            free(suppr);
        }
        free(node);
    }
}

void free_tree(struct Tree *tree) {
    free_node(tree->root);
    free(tree);
}



int max(int a, int b){
    return a>b?a:b;
}

//the height of left and right child need to be corect
void calc_local_height(struct Node* root){
    int lheight = 0;
    int rheight = 0;
    if(root->right_child != NULL){
        rheight = root->right_child->height;
    }
    if(root->left_child != NULL){
        lheight = root->left_child->height;
    }
    root->height = 1 + max(lheight,rheight);
}

//root not null and root has a left child
struct Node* right_shift(struct Node* root){
    struct Node* new_root = root->left_child;
    root->left_child = new_root->right_child;
    new_root->right_child = root;
    calc_local_height(root);
    calc_local_height(new_root);
    return new_root;
}

//root not null and root has a left child
struct Node* left_shift(struct Node* root){
    struct Node* new_root = root->right_child;
    root->right_child = new_root->left_child;
    new_root->left_child = root;
    calc_local_height(root);
    calc_local_height(new_root);
    return new_root;
}

struct Node* left_right_shift(struct Node* root){
    root->left_child = left_shift(root->left_child);
    return right_shift(root);
}

struct Node* right_left_shift(struct Node* root){
    root->right_child = right_shift(root->right_child);
    return left_shift(root);
}


int height_diff(struct Node* root){
    int lheight = 0;
    int rheight = 0;
    if(root->right_child != NULL){
        rheight = root->right_child->height;
    }
    if(root->left_child != NULL){
        lheight = root->left_child->height;
    }
    return lheight-rheight;
}

struct Node* balance(struct Node* root){
    int diff = height_diff(root);
    if(diff == 2 && height_diff(root->left_child) == 1){
        return right_shift(root);
    }
    if(diff == -2 && height_diff(root->right_child) == -1){
        return left_shift(root);
    }
     if(diff == 2 && height_diff(root->left_child) == -1){
        return left_right_shift(root);
    }
    if(diff == -2 && height_diff(root->right_child) == 1){
        return right_left_shift(root);
    }
    return root;
}