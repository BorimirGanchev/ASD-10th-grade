#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

bool DFSRecc(Graph *graph, int startVertex, int endVertex, bool *visited, int *path, int pathIndex)
{
    visited[startVertex] = true;
    path[pathIndex] = startVertex;

    if (startVertex == endVertex)
    {
        printf("Route fro %d to %d: ", path[0], path[pathIndex]);
        for (int i = 0; i <= pathIndex; i++)
        {
            printf("%d ", path[i]);
        }
        printf("\n");
        return true;
    }

    Node *current = graph->adjList[startVertex];
    while (current != NULL)
    {
        if (!visited[current->val])
        {
            if (DFSRecc(graph, current->val, endVertex, visited, path, pathIndex + 1))
            {
                return true;
            }
        }
        current = current->next;
    }

    return false;
}

void findRoute(Graph *graph, int startVertex, int endVertex)
{
    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    int *path = (int *)malloc(graph->numVertices * sizeof(int));

    if (!DFSRecc(graph, startVertex, endVertex, visited, path, 0))
    {
        printf("Err 404 not found.\n", startVertex, endVertex);
    }

    free(visited);
    free(path);
}

int main()
{
    Graph *graph = init_graph(5);
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 4);

    printGraph(graph);

    findRoute(graph, 0, 3);

    return 0;
}
/*При търсене на път между два върха в граф, BFS обикновено връща по-оптимален път в смисъл, че той е най-късият път между тези два върха, като брой на ребрата. Това се дължи на това, че BFS търси пътища от началния връх към всички достижими върхове по нива, като преминава през всички съседни върхове преди да продължи към върхове на по-голямо разстояние. Така, ако има път с по-малко ребра, BFS ще го открие първо и ще го върне.
И не, DFS не връща винаги един и същ път.
.*/