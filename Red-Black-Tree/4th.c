#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct TreeNode {
    int data;
    Color color;
    struct TreeNode* left;
    struct TreeNode* right;
    struct TreeNode* parent;
    int blackHeight;
} TreeNode;

TreeNode* createNode(int data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->data = data;
    newNode->color = RED;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->blackHeight = 0;
    return newNode;
}

void leftRotate(TreeNode** root, TreeNode* x) {
    TreeNode* y = x->right;
    x->right = y->left;
    
    if (y->left != NULL)
        y->left->parent = x;
    
    y->parent = x->parent;
    
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    
    y->left = x;
    x->parent = y;
}

void rightRotate(TreeNode** root, TreeNode* y) {
    TreeNode* x = y->left;
    y->left = x->right;

    if (x->right != NULL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

void insertFixup(TreeNode** root, TreeNode* z) {
    while (z != NULL && z->parent != NULL && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            TreeNode* y = z->parent->parent->right;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        } else {
            TreeNode* y = z->parent->parent->left;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
        
    }

    (*root)->color = BLACK;
}

void insert(TreeNode** root, int data) {
    TreeNode* z = createNode(data);
    TreeNode* y = NULL;
    TreeNode* x = *root;

    while (x != NULL) {
        y = x;
        if (z->data < x->data)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if (y == NULL)
        *root = z;
    else if (z->data < y->data)
        y->left = z;
    else
        y->right = z;
    TreeNode* temp = z;
    while (temp != NULL) {
        temp->blackHeight++;
        temp = temp->parent;
    }
    insertFixup(root, z);
}

int main() {

    TreeNode* root = NULL;

    insert(&root, 10);
    insert(&root, 20);
    insert(&root, 13);
    insert(&root, 11);
    insert(&root, 4);
    insert(&root, 2);
    insert(&root, 25);
    insert(&root, 15);

    printf("Black height of the tree: %d\n", (root != NULL) ? root->blackHeight : 0);

    return 0;
}
