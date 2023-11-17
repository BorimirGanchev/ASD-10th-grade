#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int val;
    struct Node *next;
} Node;

typedef struct Queue
{
    Node *front;
    Node *rear;
} Queue;

Node *create_node(int val)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->val = val;
    node->next = NULL;

    return node;
}

Queue *init_queue()
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
}

void enqueue(Queue *queue, int val)
{
    Node *newNode = create_node(val);
    
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return;
    }

    queue->rear->next = newNode;
    queue->rear = newNode;
}

int dequeue(Queue *queue)
{
    if (queue->front == NULL) {
        return -1; 
    }

    Node *frontNode = queue->front;
    int val = frontNode->val;

    queue->front = frontNode->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(frontNode);
    return val;
}


void clear_queue(Queue *queue)
{
    while (queue->front != NULL)
    {
        dequeue(queue);
    }

    free(queue);
}

typedef struct Stack
{
    Node *top;
} Stack;

Stack *init_stack()
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->top = NULL;
}

void push(Stack *stack, int val)
{
    Node *newNode = create_node(val);
    newNode->next = stack->top;
    stack->top = newNode;
}


int pop(Stack *stack)
{
    if (stack->top == NULL) {
        return -1;
    }

    Node *topNode = stack->top;
    int val = topNode->val;

    stack->top = topNode->next;
    free(topNode);

    return val;
}


void clear_stack(Stack *stack)
{
    while (stack->top != NULL)
    {
        pop(stack);
    }

    free(stack);
}

void print_queue(Queue *queue)
{
    Node *it = queue->front;
    printf("opashka: ");
    while (it != NULL)
    {
        printf("%d ", it->val);
        it = it->next;
    }
}

void print_stack(Stack *stack)
{
    Node *it = stack->top;
    printf("stak: ");
    while (it != NULL)
    {
        printf("%d ", it->val);
        it = it->next;
    }
}



/*
    най-лош: "O(n^2)"
    най-добър: "O(n)"

    В най-лошия случай, когато всички елементи са начално наредени в низходящ ред в опашката, всеки елемент трябва да бъде преместен от стека обратно в опашката, което води до вложена структура с времева сложност O(n^2), където 'n' е броят на елементите в опашката.


    В най-добрия случай, когато елементите вече са наредени във възходящ ред в опашката, няма нужда да се извършват допълнителни операции, което води до линейна сложност от O(n), тъй като всеки елемент се обработва веднъж и се пушва на стека без допълнителни операции
*/




void sort_queue_with_stack(Queue *queue)
{
    Queue *sortedQueue = init_queue();

    while (queue->front != NULL)
    {
        int current = dequeue(queue);
        while (sortedQueue->front != NULL && sortedQueue->front->val < current)
        {
            enqueue(queue, dequeue(sortedQueue));
        }
        enqueue(sortedQueue, current);
    }

    while (sortedQueue->front != NULL)
    {
        enqueue(queue, dequeue(sortedQueue));
    }
    clear_queue(sortedQueue);
}


int main()
{
    Queue *queue = init_queue();
    enqueue(queue, 4);
    enqueue(queue, 10);
    enqueue(queue, -5);
    enqueue(queue, 13);
    enqueue(queue, -1);
    enqueue(queue, 137);
    enqueue(queue, 15);
    enqueue(queue, 6);
    enqueue(queue, 0);
    enqueue(queue, 0);
    enqueue(queue, 18);

    sort_queue_with_stack(queue);
    print_queue(queue);
    clear_queue(queue);

    return 0;
}
