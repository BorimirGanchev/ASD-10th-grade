#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Include this header for using boolean values

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

void DFSRecc(Graph *graph, int vertex, bool *visited)
{
    visited[vertex] = true;
    printf("%d ", vertex);
    Node *current = graph->adjList[vertex];
    while (current != NULL)
    {
        if (!visited[current->val])
        {
            DFSRecc(graph, current->val, visited);
        }
        current = current->next;
    }
}

void DFS(Graph *graph, int startVertex)
{
    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    DFSRecc(graph, startVertex, visited);
    free(visited);
}

void BFS(Graph *graph, int startVertex)
{
    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    int queue[graph->numVertices];
    int front = 0, rear = 0;
    visited[startVertex] = true;
    queue[rear++] = startVertex;
    while (front < rear)
    {
        int currentVertex = queue[front++];
        printf("%d ", currentVertex);
        Node *current = graph->adjList[currentVertex];
        while (current != NULL)
        {
            if (!visited[current->val])
            {
                visited[current->val] = true;
                queue[rear++] = current->val;
            }
            current = current->next;
        }
    }
    free(visited);
}

int main()
{
    Graph *graph = init_graph(5);
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 4);

    printf("DFS traversal: ");
    DFS(graph, 0);
    printf("\n");

    printf("BFS traversal: ");
    BFS(graph, 0);
    printf("\n");

    return 0;
}
