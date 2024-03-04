#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int val;
    struct Node *next;
} Node;

typedef struct Graph
{
    int numVertices;
    Node **adjList;
} Graph;

Node *init_node(int val)
{
    Node *node = (Node *)malloc(sizeof(Node));
    node->val = val;
    node->next = NULL;

    return node;
}

Graph *init_graph(int numVertices)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->adjList = (Node **)calloc(numVertices, sizeof(Node *));

    return graph;
}

void addEdgeDirectional(Graph *graph, int from, int to)
{
    if (from < 0 || from >= graph->numVertices || to < 0 || to >= graph->numVertices)
    {
        printf("Invalid from or to\n");
        return;
    }

    Node *newNode = init_node(to);

    newNode->next = graph->adjList[from];
    graph->adjList[from] = newNode;
}

void addEdge(Graph *graph, int from, int to)
{
    addEdgeDirectional(graph, from, to);
    addEdgeDirectional(graph, to, from);
}

void removeEdgeDirectional(Graph *graph, int from, int to)
//Където from е индекс на единия връх, a to е индекса на другия връх и идеята е да махнем връзката между ъях двете
{
    //Проверявам дали дадените върхове са адекватни
    if (from < 0 || from >= graph->numVertices || to < 0 || to >= graph->numVertices)
    {
        printf("Err\n");
        return;
    }

    Node *current = graph->adjList[from];
    Node *prev = NULL;

    //Влизаме ако current е различно от нул
    while (current != NULL)
    {
        //идеята е да намерим върха до който искаме да стигнем за да изтрием edge-a
        if (current->val == to)
        {
            if (prev != NULL)
            {
                prev->next = current->next;
            }
            else
            {
                graph->adjList[from] = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Nqma kak da e tozi edge\n");
}

void removeEdge(Graph *graph, int from, int to)
{
    removeEdgeDirectional(graph, from, to);
    removeEdgeDirectional(graph, to, from);
}

void printGraph(Graph *graph)
{
    for (int i = 0; i < graph->numVertices; i++)
    {
        Node *current = graph->adjList[i];
        printf("Neighbours of %d:\n", i);
        while (current != NULL)
        {
            printf("%d ", current->val);
            current = current->next;
        }
        printf("\n");
    }
}

int main()
{
    Graph *graph = init_graph(4);
    addEdge(graph, 1, 2);
    addEdgeDirectional(graph, 2, 4);
    addEdgeDirectional(graph, 3, 4);
    addEdgeDirectional(graph, 2, 3);
    addEdgeDirectional(graph, 1, 3);
    addEdgeDirectional(graph, 1, 1);

    printf("Initial Graph:\n");
    printGraph(graph);

    removeEdge(graph, 1, 2);
    printf("\nGraph after removing edge (1, 2):\n");
    printGraph(graph);

    return 0;
}
