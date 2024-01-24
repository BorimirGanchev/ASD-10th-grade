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

int sum2(struct node* root, int a, int b) {
    if (root->key < a)
        return sum2(root->right, a, b);
    else if (root->key > b)
        return sum2(root->left, a, b);
    else
        return root->key + sum2(root->left, a, b) + sum2(root->right, a, b);
}

int main() {
    struct node* root = NULL;
    root = insert(root, 10);
    insert(root, 5);
    insert(root, 15);
    insert(root, 3);
    insert(root, 7);
    insert(root, 12);
    insert(root, 18);

    int a = 7, b = 15;
    int sum = sum2(root, a, b);

    printf("%d", sum);

    return 0;
}
