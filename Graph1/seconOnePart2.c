#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

typedef struct Graph
{
    int numVertices;
    int **adjMatrix;
} Graph;

Graph *init_graph(int numVertices)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->numVertices = numVertices;
    graph->adjMatrix = (int **)malloc(numVertices * sizeof(int *));

    for (int i = 0; i < numVertices; i++)
    {
        graph->adjMatrix[i] = (int *)calloc(numVertices, sizeof(int));
    }

    return graph;
}

void addEdgeDirectional(Graph *graph, int from, int to)
{
    if (from < 0 || from >= graph->numVertices || to < 0 || to >= graph->numVertices)
    {
        printf("Invalid from or to\n");
        return;
    }

    graph->adjMatrix[from][to] = 1;
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
        for (int j = 0; j < graph->numVertices; j++)
        {
            printf("%d ", graph->adjMatrix[i][j]);
        }
        printf("\n");
    }
}

void DFSRecc(Graph *graph, int vertex, bool *visited)
{
    visited[vertex] = true;
    printf("%d ", vertex);

    for (int i = 0; i < graph->numVertices; i++)
    {
        if (graph->adjMatrix[vertex][i] && !visited[i])
        {
            DFSRecc(graph, i, visited);
        }
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
        for (int i = 0; i < graph->numVertices; i++)
        {
            if (graph->adjMatrix[currentVertex][i] && !visited[i])
            {
                visited[i] = true;
                queue[rear++] = i;
            }
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
