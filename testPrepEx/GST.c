#include <stdio.h>
#include <stdlib.h>

struct node {
    int key;
    struct node *left, *right;
};

struct node* newNode(int item) {
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

struct node* insert(struct node* node, int key) {
    if (node == NULL)
        return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);

    return node;
}

void inorder(struct node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

void convertBSTUtil(struct node* root, int* sum) {
    if (root == NULL)
        return;

    convertBSTUtil(root->right, sum);

    *sum += root->key;
    root->key = *sum;

    convertBSTUtil(root->left, sum);
}
    
void convertBST(struct node* root) {
    int sum = 0;
    convertBSTUtil(root, &sum);
}

int main() {
    struct node* root = NULL;
    root = insert(root, 5);
    insert(root, 2);
    insert(root, 13);

    printf("Original BST: ");
    inorder(root);
    printf("\n");

    convertBST(root);

    printf("Greater Sum Tree: ");
    inorder(root);
    printf("\n");

    return 0;
}
