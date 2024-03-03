#include <stdlib.h>
#include <stdio.h>

typedef enum Color
{
    RED,
    BLACK
} Color;

typedef struct Node
{
    int val;
    Color color;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} Node;

Node *create_tree_node(int val)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->color = RED;

    return node;
}

void inorderTraversal(Node *root)
{
    if (root != NULL)
    {
        inorderTraversal(root->left);
        printf("%d(%c) ", root->val, (root->color == BLACK ? 'B' : 'R'));

        inorderTraversal(root->right);
    }
}

void preorderTraversal(Node *root)
{
    if (root != NULL)
    {
        printf("%d(%c) ", root->val, (root->color == BLACK ? 'B' : 'R'));
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

/*
     x                               y
    / \     Right Rotation          /  \
   y   T3   - - - - - - - >        T1   x
  / \       < - - - - - - -            / \
 T1  T2                               T2  T3
*/

void leftRotation(Node **root, Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    x->right = T2;
    y->left = x;

    y->parent = x->parent;

    if (x->parent == NULL)
    {
        (*root) = y;
    }
    else if (x->parent->left == x)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }

    x->parent = y;

    if (T2 != NULL)
    {
        T2->parent = x;
    }
}



void rightRotation(Node **root, Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    y->left = T2;
    x->right = y;

    x->parent = y->parent;

    if (y->parent == NULL)
    {
        (*root) = x;
    }
    else if (y->parent->left == y)
    {
        y->parent->left = x;
    }
    else
    {
        y->parent->right = x;
    }

    y->parent = x;

    if (T2 != NULL)
    {
        T2->parent = y;
    }
}

void treeFixup(Node **root, Node *x)
{
    Node *gp = NULL;
    Node *uncle = NULL;

    while (x != (*root) && x->parent->color == RED)
    {
        gp = x->parent->parent;

        if (gp->left == x->parent)
        {
            uncle = gp->right;

            if (uncle != NULL && uncle->color == RED)
            {
                x->parent->color = BLACK;
                uncle->color = BLACK;
                gp->color = RED;

                x = gp;
            }
            else
            {
                if (x->parent->right == x)
                {
                    x = x->parent;
                    leftRotation(root, x);
                }

                x->parent->color = BLACK;
                gp->color = RED;
                rightRotation(root, gp);
            }
        }
        else
        {
            uncle = gp->left;

            if (uncle != NULL && uncle->color == RED)
            {
                x->parent->color = BLACK;
                uncle->color = BLACK;
                gp->color = RED;

                x = gp;
            }
            else
            {
                if (x->parent->left == x)
                {
                    x = x->parent;
                    rightRotation(root, x);
                }

                x->parent->color = BLACK;
                gp->color = RED;
                leftRotation(root, gp);
            }
        }
    }

    (*root)->color = BLACK;
}

void insert(Node **root, int val)
{
    Node *new_node = create_tree_node(val);
    Node *current = *root;

    if (current == NULL)
    {
        new_node->color = BLACK;
        *root = new_node;
        return;
    }
    
    

    while (1)
    {
        if (current->val > val)
        {
            if (current->left == NULL)
            {
                break;
            }
            current = current->left;
        }
        else
        {
            if (current->right == NULL)
            {
                break;
            }
            current = current->right;
        }
    }

    if (current->val > val)
    {
        current->left = new_node;
    }
    else
    {
        current->right = new_node;
    }

    new_node->parent = current;

    treeFixup(root, new_node);
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}


int getBH(Node *root)
{
    if (root == NULL)
        return 0;

    int leftBH = getBH(root->left);
    int rightBH = getBH(root->right);

    int currentBH = (root->color == BLACK) ? 1 : 0;

    return currentBH + max(leftBH, rightBH);
}


//Не работи да се върна!!!!!!!!!!!!!!!!!!!
void findMaxYsecond(Node* root, int targetBH, Node** maxNode) {
    if (root == NULL || *maxNode != NULL) {
        return;
    }

    if (root->color == BLACK) {
        targetBH--;
    }

    if (targetBH == 0) {
        *maxNode = root;
        return;
    }
    findMaxYsecond(root->right, targetBH, maxNode);
    if (*maxNode != NULL) {
        return;
    }

    if (root->color == BLACK) {
        targetBH--;
    }

    if (targetBH == 0) {
        *maxNode = root;
        return;
    }

    findMaxYsecond(root->left, targetBH, maxNode);
}

Node* findMaxY(Node* root, int targetBH) {
    Node* maxNode = NULL;
    findMaxYsecond(root, targetBH, &maxNode);
    return maxNode;
}

Node* joinTrees(Node* t1, Node* t2, int x) {
    //printf("debugcheck");
    int bh1 = getBH(t1);
    int bh2 = getBH(t2);

    
    if (bh2 > bh1) {
        Node* temp = t1;
        t1 = t2;
        t2 = temp;
    }

    Node* y = findMaxY(t1, bh2);


    Node* Ty = y->right;
    y->right = NULL;

    //printf("debugcheck");

    Node* newX = create_tree_node(x);
    newX->color = RED; 

    newX->left = Ty;
    if (Ty != NULL) {
        Ty->parent = newX;
        //printf("debugcheck");
    }

    newX->right = t2;
    if (t2 != NULL) {
        t2->parent = newX;
    }

    y->right = newX;
    newX->parent = y;

    if (newX->parent->color == RED) {
        treeFixup(&t1, newX);
        //printf("debugcheck");
    }

    return t1;
}


int isRBTreesecond(Node* root, int blackHeight, int currentBlackHeight, int* isValid)
{
    if (root == NULL)
    {
        if (blackHeight == -1)
        {
            blackHeight = currentBlackHeight;
        }
        else if (blackHeight != currentBlackHeight)
        {
            *isValid = 0;
            return 0;
        }

        return 1;
    }

    if (root->color == RED)
    {
        if (root->left != NULL && root->left->color == RED)
        {
            *isValid = 0;
            return 0;
        }

        if (root->right != NULL && root->right->color == RED)
        {
            *isValid = 0;
            return 0;
        }
    }

    int leftValid = isRBTreesecond(root->left, blackHeight, currentBlackHeight + (root->color == BLACK), isValid);
    int rightValid = isRBTreesecond(root->right, blackHeight, currentBlackHeight + (root->color == BLACK), isValid);

    return leftValid && rightValid;
}

int isRBTree(Node* root)
{
    if (root == NULL)
    {
        return 1;
    }

    int isValid = 1; 
    if (root->color != BLACK)
    {
        isValid = 0;
    }

    isRBTreesecond(root, -1, 0, &isValid);

    return isValid;
}



int main()
{
    struct Node *root = NULL;

    insert(&root, 7);
    insert(&root, 3);
    insert(&root, 18);
    insert(&root, 28);
    insert(&root, 32);
    insert(&root, 52);
    insert(&root, 41);
    insert(&root, 35);
    insert(&root, 65);
    insert(&root, 27);
    insert(&root, 17);
    insert(&root, 99);
    insert(&root, 88);


    struct Node *root2 = NULL;

    insert(&root2, 101);
    insert(&root2, 150);
    insert(&root2, 180);
    insert(&root2, 280);
    insert(&root2, 320);
    insert(&root2, 520);
    insert(&root2, 410);

    struct Node *joinned = joinTrees(root, root2, 100);

    inorderTraversal(joinned);

    if (isRBTree(joinned)) {
        printf("\nThe tree is valid");
    }
    else{
        printf("\nThe tree is not valid");
    }

    //не съм сигурен за тук, да се върна
    int blackHeight = getBH(root);

    printf("\nBlack Height: %d\n", blackHeight);

    return 0;
}




/*
Задача1:
 какъв да бъде цвета на новия node и как да оправим нередности?
 (ако сте писали кода се подразбира, но за който не е може все пак да се опита да отговори)

 каква е сложността на join-а?
O(log n), където n е броят на елементите в по-голямото от двете дървета (t1 и t2). 
Защото като търсим елемента y с черна височина, равна на черната височина на t2, и използването на свойствата на Red-Black Tree изискват сложност O(log n).

Задача2:
 за AVL дървото:

 za RB дървото:

Задача3:
1. Всеки възел е или червен, или черен.
2. Коренът е винаги черен
3. Всички листа (NULL възли) са черни.
4. Ако един възел е червен, то и двете мъ деца са черни.
5. Всеки път от даден възел до неговите листа (NULL възли) съдържа еднакъв брой черни възли.





Задача4:
Колко възможни ротации има в едно BST дърво с n елемента и
защо?




Броят на възможните ротации зависи от конкретната структура на дървото и последователността на вмъкването или изтриването на възлите. В общия случай обаче, възможните ротации в едно BST с n елемента могат да бъдат от O(log n) до O(n), в зависимост от баланса на дървото.
*/



